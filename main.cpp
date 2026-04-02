#include "buffers.h"
#include "callbacks.h"
#include "camera.h"
#include "glad/glad.h"
#include "glm/ext/vector_float3.hpp"
#include "model.h"
#include "shaders.h"
#include "stb_image.h"
#include "utilities.h"
#include <GLFW/glfw3.h>
#include <filesystem>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <memory>
#include <utility>
#include <vector>

static GLfloat lastFrameTime = 0.0f;
static GLfloat x_rotation_deg = 0.0f;
static GLfloat y_rotation_deg = 0.0f;
static GLfloat mix_ratio = 0.0f;

static amk::cameraManager camera;
static GLFWwindow *initialize_renderer();
static void destroy_renderer(GLFWwindow *window);
static void process_input(GLFWwindow *window);
static void draw_map(amk::shader &shader, amk::model &model);

int main() {
    GLFWwindow *window = initialize_renderer();
    amk::callbackManager callback_manager{window, camera};
    amk::shader shader("./shaders/shader.vert", "./shaders/shader.frag");
    std::cout << "shader done" << std::endl;
    amk::lightShader emissive("./shaders/light.vert", "./shaders/light.frag");
    std::cout << "emissive done" << std::endl;
    auto light = std::make_unique<amk::lightModel>(camera, emissive);
    auto material1 =
        std::make_unique<amk::model>(camera, shader, amk::mesh{{255, 255, 0}});
    auto material2 =
        std::make_unique<amk::model>(camera, shader, amk::mesh{{255, 0, 0}});
    auto material3 =
        std::make_unique<amk::model>(camera, shader, amk::mesh{{0, 255, 0}});
    auto material4 =
        std::make_unique<amk::model>(camera, shader, amk::mesh{{0, 0, 255}});
    material1->add_light_model(*light);
    material2->add_light_model(*light);
    material3->add_light_model(*light);
    material4->add_light_model(*light);
    std::vector<std::unique_ptr<amk::model>> models;
    models.push_back(std::move(light));
    models.push_back(std::move(material1));
    models.push_back(std::move(material2));
    models.push_back(std::move(material3));
    models.push_back(std::move(material4));

    static constexpr glm::vec3 pos[] = {
        {0.0f, 5.0f, 0.0f},
        {0.0f, 0.0f, -5.0f},
        {5.0f, 0.0f, 0.0f},
        {-5.0f, 0.0f, 0.0f},
        {0.0f, 0.0f, 5.0f},

    };

    models[0]->set_model_scale({0.5f, 0.5f, 0.5f});

    /* ---------------------------- loop ---------------------------- */

    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.0f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        process_input(window);

        int indx = 0;
        for (auto &model : models) {
            model->set_model_pos(pos[indx]);
            if (indx != 0) {
                model->set_model_rot(x_rotation_deg, amk::model::x);
                model->set_model_rot(y_rotation_deg, amk::model::y);
            }
            model->draw_model();
            indx = (indx + 1) % models.size();
        }

        /* getting ready for next frame */
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    /* -------------------------------------------------------------- */

    destroy_renderer(window);
    return 0;
}

// const std::vector<glm::vec3> model_pos{{-1.0f, 0.0f, -1.0f},
//                                        {0.0f, 0.0f, -3.0f},
//                                        {1.0f, 1.0f, -0.5f},
//                                        {4.0f, 2.0f, -9.0f},
//                                        {8.0f, 3.0f, -1.5f},
//                                        {2.0f, -4.0f, -0.5f},
//                                        {3.0f, 6.0f, -0.5f},
//                                        {-5.0f, 10.0f, 3.0f}};

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
    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) {
        x_rotation_deg += 5.0f;
    } else if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) {
        x_rotation_deg -= 5.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
        y_rotation_deg += 5.0f;
    } else if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) {
        y_rotation_deg -= 5.0f;
    }
}
