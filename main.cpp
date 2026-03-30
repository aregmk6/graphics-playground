#include "buffers.h"
#include "callbacks.h"
#include "camera.h"
#include "glad/glad.h"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/vector_float3.hpp"
#include "glm/trigonometric.hpp"
#include "shaders.h"
#include "stb_image.h"
#include "texture.h"
#include "utilities.h"
#include "vertex.h"
#include <GLFW/glfw3.h>
#include <filesystem>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

static GLfloat lastFrameTime = 0.0f;
static GLfloat curFrameTime = 0.0f;
static GLfloat dt = 0.0f;
static GLfloat mix_ratio = 0.0f;
static GLfloat rotation_deg = 0.0f;

static amk::cameraManager camera;
static amk::callbackManager callbacks{camera};

static void process_input(GLFWwindow *window) {
    curFrameTime = glfwGetTime();
    dt = curFrameTime - lastFrameTime;
    lastFrameTime = curFrameTime;

    camera.update_cam_pos(window, dt);

    // per model logic
    if (glfwGetKey(window, GLFW_KEY_PERIOD) == GLFW_PRESS) {
        mix_ratio += 0.01f;
        if (mix_ratio >= 1.0f) mix_ratio = 1.0f;
    } else if (glfwGetKey(window, GLFW_KEY_COMMA) == GLFW_PRESS) {
        mix_ratio -= 0.01f;
        if (mix_ratio <= 0.0f) mix_ratio = 0.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
        rotation_deg += 5.0f;
    } else if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) {
        rotation_deg -= 5.0f;
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

int main() {
    ERR_CHECK(glfwInit(), "GLFW Init");

    glfwSetErrorCallback(amk::callbackManager::error_callback);

    GLFWwindow *window = glfwCreateWindow(
        window_width, window_height, window_name, nullptr, nullptr);
    ERR_CHECK(window, "Create Window");

    glfwMakeContextCurrent(window);

    glfwSetKeyCallback(window, amk::callbackManager::key_callback);
    glfwSetCursorPosCallback(window, amk::callbackManager::mouse_pos_callback);
    glfwSetCursorEnterCallback(window,
                               amk::callbackManager::mouse_enter_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    glad_glEnable(GL_DEPTH_TEST);

    amk::shader shader("./shaders/shader.vert", "./shaders/shader.frag");
    GLuint mixLoc, pvmLoc;
    pvmLoc = shader.insert_uniform("u_PVM");
    mixLoc = shader.insert_uniform("u_mix");
    shader.assign_tex_sampler("TextureData1", 0);
    shader.assign_tex_sampler("TextureData2", 1);

    amk::texture bricks("./textures/bricks.jpg");
    amk::texture devious_apple("./textures/apple.png", GL_RGBA, true);
    bricks.set_parameters(GL_REPEAT, GL_REPEAT, GL_NEAREST, GL_LINEAR);
    devious_apple.set_parameters(
        GL_CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER, GL_NEAREST, GL_LINEAR);

    amk::VBO vbo(get_vertex_data());
    amk::EBO ebo(get_index_data());
    amk::VAO vao(vbo, ebo);

    // transformations
    glm::mat4 m_model(1.0f), PVM;

    GLint cur_buff_width, cur_buff_height;
    while (!glfwWindowShouldClose(window)) {
        glfwGetFramebufferSize(window, &cur_buff_width, &cur_buff_height);
        glViewport(0, 0, cur_buff_width, cur_buff_height);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        process_input(window);

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
            PVM = camera.get_PV() * m_model;
            shader.assign_mat_uniform(pvmLoc, PVM);
            glDrawElements(
                GL_TRIANGLES, ebo.nr_of_indices(), GL_UNSIGNED_INT, nullptr);
        }

        /* getting ready for next frame */
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
