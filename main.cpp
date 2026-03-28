#define STB_IMAGE_IMPLEMENTATION

#include "glad/glad.h"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/vector_float3.hpp"
#include "glm/geometric.hpp"
#include "glm/trigonometric.hpp"
#include "stb_image.h"
#include "utilities.h"
#include <GLFW/glfw3.h>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>

static int error_res_check = 1;

static char shader_src_buff[shader_buff_size];
static const char *shader_src_ptr = shader_src_buff;

static void error_callback(int error_num, const char *info);
static void key_callback(GLFWwindow *window, int key, int scancode, int action,
                         int mods);
static void mouse_pos_callback(GLFWwindow *window, double xpos, double ypos);
void mouse_enter_callback(GLFWwindow *window, int entered);

static GLfloat mix_amount = 0.0f;
static GLfloat rotation_deg = 0.0f;
static GLfloat camera_speed = 5.0f;
static GLfloat dt = 0.0f;
static GLfloat dx = dt * camera_speed;
static GLfloat lastFrameTime = 0.0f;
static GLfloat curFrameTime = 0.0f;
static GLfloat lastx = 400, lasty = 300, pitch = 0.0f, yaw = 0.0f;
static GLfloat sensativity = 0.1f;

static glm::vec3 up(0.0f, 1.0f, 0.0f);
static glm::vec3 cameraPos(0.0f, 0.0f, 3.0f);
static glm::vec3 cameraFront(0.0f, 0.0f, -1.0f);

static void process_input(GLFWwindow *window) {
    curFrameTime = glfwGetTime();
    dt = curFrameTime - lastFrameTime;
    dx = dt * camera_speed;
    lastFrameTime = curFrameTime;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        cameraPos += dx * cameraFront;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        cameraPos -= dx * cameraFront;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        cameraPos += dx * glm::normalize(glm::cross(cameraFront, up));
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        cameraPos -= dx * glm::normalize(glm::cross(cameraFront, up));
    }
    if (glfwGetKey(window, GLFW_KEY_PERIOD) == GLFW_PRESS) {
        mix_amount += 0.01f;
        if (mix_amount >= 1.0f) mix_amount = 1.0f;
    } else if (glfwGetKey(window, GLFW_KEY_COMMA) == GLFW_PRESS) {
        mix_amount -= 0.01f;
        if (mix_amount <= 0.0f) mix_amount = 0.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
        rotation_deg = 0.0f;
    } else if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) {
        rotation_deg -= 1.0f;
        if (rotation_deg <= -180.0f) rotation_deg = -180.0f;
    }
}

std::vector<float> get_vertex_data() {
    // for now I'll hardcode it.
    std::vector<float> //
        vertcies{
            0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // top right
            0.5f,  -0.5f, 0.5f,  1.0f, 0.0f, // bottom right
            -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, // bottom left
            -0.5f, 0.5f,  0.5f,  0.0f, 1.0f, // top left

            0.5f,  0.5f,  -0.5f, 1.0f, 1.0f, // top right
            0.5f,  -0.5f, -0.5f, 1.0f, 0.0f, // bottom right
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, // bottom left
            -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f, // top left

            0.5f,  0.5f,  -0.5f, 1.0f, 1.0f, // top right
            0.5f,  -0.5f, -0.5f, 1.0f, 0.0f, // bottom right
            0.5f,  -0.5f, 0.5f,  0.0f, 0.0f, // bottom left
            0.5f,  0.5f,  0.5f,  0.0f, 1.0f, // top left

            -0.5f, 0.5f,  -0.5f, 1.0f, 1.0f, // top right
            -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, // bottom right
            -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, // bottom left
            -0.5f, 0.5f,  0.5f,  0.0f, 1.0f, // top left

            0.5f,  0.5f,  -0.5f, 1.0f, 1.0f, // top right
            0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // bottom right
            -0.5f, 0.5f,  0.5f,  0.0f, 0.0f, // bottom left
            -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f, // top left

            0.5f,  -0.5f, -0.5f, 1.0f, 1.0f, // top right
            0.5f,  -0.5f, 0.5f,  1.0f, 0.0f, // bottom right
            -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, // bottom left
            -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, // top left
        };
    return vertcies;
}

std::vector<GLuint> get_index_data() {
    // for now I'll hardcode it.
    constexpr int vert_per_face = 4;
    std::vector<GLuint> //
        vertcies{
            0,
            1,
            3,
            1,
            2,
            3, //
            vert_per_face + 0,
            vert_per_face + 1,
            vert_per_face + 3,
            vert_per_face + 1,
            vert_per_face + 2,
            vert_per_face + 3, //
            2 * vert_per_face + 0,
            2 * vert_per_face + 1,
            2 * vert_per_face + 3,
            2 * vert_per_face + 1,
            2 * vert_per_face + 2,
            2 * vert_per_face + 3, //
            3 * vert_per_face + 0,
            3 * vert_per_face + 1,
            3 * vert_per_face + 3,
            3 * vert_per_face + 1,
            3 * vert_per_face + 2,
            3 * vert_per_face + 3, //
            4 * vert_per_face + 0,
            4 * vert_per_face + 1,
            4 * vert_per_face + 3,
            4 * vert_per_face + 1,
            4 * vert_per_face + 2,
            4 * vert_per_face + 3, //
            5 * vert_per_face + 0,
            5 * vert_per_face + 1,
            5 * vert_per_face + 3,
            5 * vert_per_face + 1,
            5 * vert_per_face + 2,
            5 * vert_per_face + 3, //
        };
    return vertcies;
}

void get_shader_data(std::filesystem::path p) {
    std::ifstream ifs(p, std::ios::in | std::ios::ate);
    if (!ifs) {
        std::cerr << "Shader file failed to open" << std::endl;
        exit(EXIT_FAILURE);
    }
    auto file_size = ifs.tellg();
    if (file_size > shader_buff_size) {
        std::cerr << "Shader source code too large" << std::endl;
    }
    ifs.seekg(0);
    ifs.read(shader_src_buff, file_size);
    shader_src_buff[file_size] = '\0';

    ifs.close();
}

int main() {
    ERR_CHECK(glfwInit(), "Glfw Init");

    glfwSetErrorCallback(error_callback);

    GLFWwindow *window =
        glfwCreateWindow(w_width, w_height, w_name, nullptr, nullptr);
    ERR_CHECK(window, "Create Window");

    glfwMakeContextCurrent(window);

    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    GLuint shaderProgram = glCreateProgram();

    using std::filesystem::path;
    get_shader_data(path(PROJECT_ROOT_PATH) / "shaders" / "shader.vert");
    glShaderSource(vertexShader, 1, &shader_src_ptr, NULL);
    glCompileShader(vertexShader);
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &error_res_check);
    ERR_CHECK(error_res_check, "vertex shader compile");

    get_shader_data(path(PROJECT_ROOT_PATH) / "shaders" / "shader.frag");
    glShaderSource(fragmentShader, 1, &shader_src_ptr, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &error_res_check);
    ERR_CHECK(error_res_check, "fragment shader compile");

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &error_res_check);
    ERR_CHECK(error_res_check, "program link");

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    int img_width, img_height, channel_nr;

    using std::filesystem::path;
    GLubyte *img;
    img =
        stbi_load(path(path(PROJECT_ROOT_PATH) / "textures/bricks.jpg").c_str(),
                  &img_width,
                  &img_height,
                  &channel_nr,
                  0);
    ERR_CHECK(img, "stbi_load");

    GLuint tex[2];
    glad_glGenTextures(1, tex);
    glad_glGenTextures(1, tex + 1);

    glad_glBindTexture(GL_TEXTURE_2D, tex[0]);

    glad_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glad_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glad_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glad_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glad_glTexImage2D(GL_TEXTURE_2D,
                      0,
                      GL_RGB,
                      img_width,
                      img_height,
                      0,
                      GL_RGB,
                      GL_UNSIGNED_BYTE,
                      img);
    glad_glGenerateMipmap(GL_TEXTURE_2D);

    glad_glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(img);

    stbi_set_flip_vertically_on_load(true);
    img =
        stbi_load(path(path(PROJECT_ROOT_PATH) / "textures/apple.png").c_str(),
                  &img_width,
                  &img_height,
                  &channel_nr,
                  0);
    ERR_CHECK(img, "stbi_load");
    glad_glBindTexture(GL_TEXTURE_2D, tex[1]);

    glad_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glad_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glad_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glad_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glad_glTexImage2D(GL_TEXTURE_2D,
                      0,
                      GL_RGBA,
                      img_width,
                      img_height,
                      0,
                      GL_RGBA,
                      GL_UNSIGNED_BYTE,
                      img);
    glad_glGenerateMipmap(GL_TEXTURE_2D);

    glad_glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(img);
    stbi_set_flip_vertically_on_load(false);

    auto vertices = get_vertex_data();
    auto indices = get_index_data();

    uint VBO, EBO, VAO;
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glGenVertexArrays(1, &VAO);

    // bind VAO
    glBindVertexArray(VAO);

    // bind VBO under the VAO and handle it
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(GLfloat) * vertices.size(),
                 vertices.data(),
                 GL_STATIC_DRAW);

    glVertexAttribPointer(static_cast<int>(attrib::COORD),
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          sizeof(GLfloat) * 5,
                          (void *)0);
    glEnableVertexAttribArray(static_cast<int>(attrib::COORD));
    glVertexAttribPointer(static_cast<int>(attrib::TEX),
                          2,
                          GL_FLOAT,
                          GL_FALSE,
                          sizeof(float) * 5,
                          (void *)(sizeof(GLfloat) * 3));
    glEnableVertexAttribArray(static_cast<int>(attrib::TEX));

    // bind EBO under the VAO and handle it
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 sizeof(GLuint) * indices.size(),
                 indices.data(),
                 GL_STATIC_DRAW);

    // unbinding:
    // first VAO, then EBO.
    // VBO can be unbound after the atrrib pointer call.
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // uniform sampler2D TextureData1;
    // uniform sampler2D TextureData2;

    glUseProgram(shaderProgram);
    GLint samplerLocation;
    samplerLocation = glad_glGetUniformLocation(shaderProgram, "TextureData1");
    glad_glUniform1i(samplerLocation, 0);
    samplerLocation = glad_glGetUniformLocation(shaderProgram, "TextureData2");
    glad_glUniform1i(samplerLocation, 1);

    GLuint mixLocation = glad_glGetUniformLocation(shaderProgram, "mix_amount");

    // transformations
    GLint modelLoc, viewLoc, perspectiveLoc;
    glm::mat4 model(1.0f);
    glm::mat4 view(1.0f);
    glm::mat4 perspective =
        glm::perspective(45.0f, 800.0f / 600.0f, 0.1f, 100.0f);

    // uniforms
    modelLoc = glad_glGetUniformLocation(shaderProgram, "model");
    viewLoc = glad_glGetUniformLocation(shaderProgram, "view");
    perspectiveLoc = glad_glGetUniformLocation(shaderProgram, "perspective");
    glad_glUniformMatrix4fv(
        perspectiveLoc, 1, GL_FALSE, glm::value_ptr(perspective));

    glad_glEnable(GL_DEPTH_TEST);
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_pos_callback);
    glfwSetCursorEnterCallback(window, mouse_enter_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    std::vector<glm::vec3> model_pos = {glm::vec3(-1.0f, 0.0f, -1.0f),
                                        glm::vec3(0.0f, 0.0f, -3.0f),
                                        glm::vec3(1.0f, 1.0f, -0.5f),
                                        glm::vec3(4.0f, 2.0f, -9.0f),
                                        glm::vec3(8.0f, 3.0f, -1.5f),
                                        glm::vec3(2.0f, -4.0f, -0.5f),
                                        glm::vec3(3.0f, 6.0f, -0.5f),
                                        glm::vec3(-5.0f, 10.0f, 3.0f)};

    int cur_buff_width, cur_buff_height;
    while (!glfwWindowShouldClose(window)) {
        process_input(window);
        glfwGetFramebufferSize(window, &cur_buff_width, &cur_buff_height);
        glViewport(0, 0, cur_buff_width, cur_buff_height);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glad_glActiveTexture(GL_TEXTURE0);
        glad_glBindTexture(GL_TEXTURE_2D, tex[0]);
        glad_glActiveTexture(GL_TEXTURE1);
        glad_glBindTexture(GL_TEXTURE_2D, tex[1]);

        glUseProgram(shaderProgram);
        view = glm::lookAt(cameraPos, cameraPos + cameraFront, up);
        glad_glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glad_glUniform1f(mixLocation, mix_amount);
        glBindVertexArray(VAO);
        for (int i = 0; i < 8; ++i) {
            model = glm::translate(glm::mat4(1.0f), model_pos[i]);
            model = glm::rotate(
                model, glm::radians(rotation_deg), glm::vec3(1.0f, 0.0f, 0.0f));
            glad_glUniformMatrix4fv(
                modelLoc, 1, GL_FALSE, glm::value_ptr(model));
            glDrawElements(
                GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);
        }

        /* getting ready for next frame */
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

static void error_callback(int error_num, const char *info) {
    std::cerr << "Error number: " << error_num //
              << ". description: " << info     //
              << std::endl;                    //
}

static void key_callback(GLFWwindow *window, int key, int scancode, int action,
                         int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

static void mouse_pos_callback(GLFWwindow *window, double xpos, double ypos) {
    GLfloat offsetx = xpos - lastx;
    GLfloat offsety = lasty - ypos;
    lastx = xpos;
    lasty = ypos;

    yaw += offsetx * sensativity;
    pitch += offsety * sensativity;

    pitch = pitch > pitch_max_limit   ? pitch = pitch_max_limit
            : pitch < pitch_min_limit ? pitch_min_limit
                                      : pitch;

    cameraFront = glm::normalize(
        glm::vec3(cos(glm::radians(yaw)) * cos(glm::radians(pitch)),
                  sin(glm::radians(pitch)),
                  sin(glm::radians(yaw)) * cos(glm::radians(pitch))));
}

void mouse_enter_callback(GLFWwindow *window, int entered) {
    if (entered) {
        std::cout << "entered window" << std::endl;
    }
}
