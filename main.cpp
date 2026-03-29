#include "texture.h"
#include "vertex.h"
#define STB_IMAGE_IMPLEMENTATION

#include "buffers.h"
#include "glad/glad.h"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/vector_float3.hpp"
#include "glm/geometric.hpp"
#include "glm/trigonometric.hpp"
#include "shaders.h"
#include "utilities.h"
#include <GLFW/glfw3.h>
#include <cmath>
#include <filesystem>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>

static void error_callback(int error_num, const char *info);
static void key_callback(GLFWwindow *window, int key, int scancode, int action,
                         int mods);
static void mouse_pos_callback(GLFWwindow *window, double xpos, double ypos);
void mouse_enter_callback(GLFWwindow *window, int entered);

static GLfloat mix_ratio = 0.0f;
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
        mix_ratio += 0.01f;
        if (mix_ratio >= 1.0f) mix_ratio = 1.0f;
    } else if (glfwGetKey(window, GLFW_KEY_COMMA) == GLFW_PRESS) {
        mix_ratio -= 0.01f;
        if (mix_ratio <= 0.0f) mix_ratio = 0.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
        rotation_deg = 0.0f;
    } else if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) {
        rotation_deg -= 1.0f;
        if (rotation_deg <= -180.0f) rotation_deg = -180.0f;
    }
}

std::vector<amk::vertex> get_vertex_data() {
    // for now I'll hardcode it.
    std::vector<amk::vertex> //
        vertcies{
            {{0.5f, 0.5f, 0.5f}, {1.0f, 1.0f}},   // top right
            {{0.5f, -0.5f, 0.5f}, {1.0f, 0.0f}},  // bottom right
            {{-0.5f, -0.5f, 0.5f}, {0.0f, 0.0f}}, // bottom left
            {{-0.5f, 0.5f, 0.5f}, {0.0f, 1.0f}},  // top left

            {{0.5f, 0.5f, -0.5f}, {1.0f, 1.0f}},   // top right
            {{0.5f, -0.5f, -0.5f}, {1.0f, 0.0f}},  // bottom right
            {{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f}}, // bottom left
            {{-0.5f, 0.5f, -0.5f}, {0.0f, 1.0f}},  // top left

            {{0.5f, 0.5f, -0.5f}, {1.0f, 1.0f}},  // top right
            {{0.5f, -0.5f, -0.5f}, {1.0f, 0.0f}}, // bottom right
            {{0.5f, -0.5f, 0.5f}, {0.0f, 0.0f}},  // bottom left
            {{0.5f, 0.5f, 0.5f}, {0.0f, 1.0f}},   // top left

            {{-0.5f, 0.5f, -0.5f}, {1.0f, 1.0f}},  // top right
            {{-0.5f, -0.5f, -0.5f}, {1.0f, 0.0f}}, // bottom right
            {{-0.5f, -0.5f, 0.5f}, {0.0f, 0.0f}},  // bottom left
            {{-0.5f, 0.5f, 0.5f}, {0.0f, 1.0f}},   // top left

            {{0.5f, 0.5f, -0.5f}, {1.0f, 1.0f}},  // top right
            {{0.5f, 0.5f, 0.5f}, {1.0f, 0.0f}},   // bottom right
            {{-0.5f, 0.5f, 0.5f}, {0.0f, 0.0f}},  // bottom left
            {{-0.5f, 0.5f, -0.5f}, {0.0f, 1.0f}}, // top left

            {{0.5f, -0.5f, -0.5f}, {1.0f, 1.0f}}, // top right
            {{0.5f, -0.5f, 0.5f}, {1.0f, 0.0f}},  // bottom right
            {{-0.5f, -0.5f, 0.5f}, {0.0f, 0.0f}}, // bottom left
            {{-0.5f, -0.5f, -0.5f}, {0.0f, 1.0f}} // top left
        };
    return vertcies;
}

std::vector<GLuint> get_index_data() {
    constexpr int vert_per_face = 4;
    constexpr int order[] = {0, 1, 3, 1, 2, 3};
    std::vector<GLuint> vertcies{};
    for (int i = 0; i < 6; ++i) {
        for (int j = 0; j < 6; ++j) {
            vertcies.push_back(i * vert_per_face + order[j]);
        }
    }

    return vertcies;
}

static const std::vector<glm::vec3> model_pos = {glm::vec3(-1.0f, 0.0f, -1.0f),
                                                 glm::vec3(0.0f, 0.0f, -3.0f),
                                                 glm::vec3(1.0f, 1.0f, -0.5f),
                                                 glm::vec3(4.0f, 2.0f, -9.0f),
                                                 glm::vec3(8.0f, 3.0f, -1.5f),
                                                 glm::vec3(2.0f, -4.0f, -0.5f),
                                                 glm::vec3(3.0f, 6.0f, -0.5f),
                                                 glm::vec3(-5.0f, 10.0f, 3.0f)};

int main() {
    ERR_CHECK(glfwInit(), "GLFW Init");

    glfwSetErrorCallback(error_callback);

    GLFWwindow *window = glfwCreateWindow(
        window_width, window_height, window_name, nullptr, nullptr);
    ERR_CHECK(window, "Create Window");

    glfwMakeContextCurrent(window);

    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_pos_callback);
    glfwSetCursorEnterCallback(window, mouse_enter_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    glad_glEnable(GL_DEPTH_TEST);

    amk::shader shader("./shaders/shader.vert", "./shaders/shader.frag");
    shader.use();
    GLuint mixLoc, pvmLoc;
    pvmLoc = shader.insert_uniform("u_PVM");
    mixLoc = shader.insert_uniform("u_mix");
    shader.assign_sampler("TextureData1", 0);
    shader.assign_sampler("TextureData2", 1);

    amk::texture bricks("./textures/bricks.jpg");
    amk::texture devious_apple("./textures/apple.png", GL_RGBA, true);
    bricks.set_parameters(GL_REPEAT, GL_REPEAT, GL_NEAREST, GL_LINEAR);
    devious_apple.set_parameters(
        GL_CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER, GL_NEAREST, GL_LINEAR);

    auto vertices = get_vertex_data();
    auto indices = get_index_data();

    amk::VBO vbo(vertices);
    amk::EBO ebo(indices);
    amk::VAO vao(vbo, ebo);

    // transformations
    glm::mat4 m_model(1.0f), m_view(1.0f), PV{}, PVM{};
    glm::mat4 m_perspect =
        glm::perspective(45.0f, 800.0f / 600.0f, 0.1f, 100.0f);

    GLint cur_buff_width, cur_buff_height;
    while (!glfwWindowShouldClose(window)) {
        glfwGetFramebufferSize(window, &cur_buff_width, &cur_buff_height);
        glViewport(0, 0, cur_buff_width, cur_buff_height);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        process_input(window);
        m_view = glm::lookAt(cameraPos, cameraPos + cameraFront, up);
        PV = m_perspect * m_view;

        glad_glActiveTexture(GL_TEXTURE0);
        bricks.bind();
        glad_glActiveTexture(GL_TEXTURE1);
        devious_apple.bind();

        shader.use();
        shader.assign_float_uniform(mixLoc, mix_ratio);

        vao.bind();
        for (int i = 0; i < 8; ++i) {
            m_model = glm::translate(glm::mat4(1.0f), model_pos[i]);
            m_model = glm::rotate(m_model,
                                  glm::radians(rotation_deg),
                                  glm::vec3(1.0f, 0.0f, 0.0f));
            PVM = PV * m_model;
            shader.assign_mat_uniform(pvmLoc, PVM);
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

// -----------------------------------------------------------------------------------------------------------
//
// 						call back functions
//
// -----------------------------------------------------------------------------------------------------------

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
