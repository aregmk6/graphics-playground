#include "buffers.h"
#include "callbacks.h"
#include "camera.h"
#include "glad/glad.h"
#include "model.h"
#include "shaders.h"
#include "stb_image.h"
#include "utilities.h"
#include <GLFW/glfw3.h>
#include <filesystem>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

static GLfloat lastFrameTime = 0.0f;
static GLfloat mix_ratio = 0.0f;
static GLfloat rotation_deg = 0.0f;

static amk::cameraManager camera;
static GLFWwindow *initialize_renderer();
static void destroy_renderer(GLFWwindow *window);
static void process_input(GLFWwindow *window);
static void draw_map(amk::shader &shader, amk::model &model);

int main() {
    GLFWwindow *window = initialize_renderer();
    amk::callbackManager callback_manager{window, camera};
    amk::shader shader("./shaders/shader.vert", "./shaders/shader.frag");
    amk::model model{camera, shader};

    GLuint mixLoc = shader.insert_uniform("u_mix");
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        process_input(window);

        shader.assign_float_uniform(mixLoc, mix_ratio);
        draw_map(shader, model);

        /* getting ready for next frame */
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    destroy_renderer(window);
    return 0;
}

void draw_map(amk::shader &shader, amk::model &model) {
    const std::vector<glm::vec3> model_pos{{-1.0f, 0.0f, -1.0f},
                                           {0.0f, 0.0f, -3.0f},
                                           {1.0f, 1.0f, -0.5f},
                                           {4.0f, 2.0f, -9.0f},
                                           {8.0f, 3.0f, -1.5f},
                                           {2.0f, -4.0f, -0.5f},
                                           {3.0f, 6.0f, -0.5f},
                                           {-5.0f, 10.0f, 3.0f}};
    for (int i = 0; i < 8; ++i) {
        model.set_model_pos(model_pos[i]);
        model.set_model_rot(rotation_deg, amk::model::x);
        model.draw_model(shader);
    }
}

GLFWwindow *initialize_renderer() {
    ERR_CHECK(glfwInit(), "GLFW Init");
    GLFWwindow *window = glfwCreateWindow(window_width, //
                                          window_height,
                                          window_name,
                                          nullptr,
                                          nullptr);
    ERR_CHECK(window, "Create Window");
    glfwMakeContextCurrent(window);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    glad_glEnable(GL_DEPTH_TEST);

    return window;
}

void destroy_renderer(GLFWwindow *window) {
    glfwDestroyWindow(window);
    glfwTerminate();
}

void process_input(GLFWwindow *window) {
    GLfloat curFrameTime = glfwGetTime();
    GLfloat dt = curFrameTime - lastFrameTime;
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
