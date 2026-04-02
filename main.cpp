#include "buffers.h"
#include "callbacks.h"
#include "camera.h"
#include "glad/glad.h"
#include "glm/ext/vector_float3.hpp"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
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

static amk::cameraManager camera;
static GLFWwindow *startup();
static void shutdown(GLFWwindow *window);
static void process_input(GLFWwindow *window);
static void draw_map(amk::shader &shader, amk::model &model);

int main() {
    GLFWwindow *window = startup();
    amk::callbackManager callback_manager{window, camera};
    amk::shader shader("./shaders/shader.vert", "./shaders/shader.frag");
    amk::lightShader emissive("./shaders/light.vert", "./shaders/light.frag");

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

    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    bool show_demo_window = false;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    /* ---------------------------- loop ---------------------------- */

    static float f = 0.0f;
    static int counter = 0;
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        glClearColor(
            clear_color.x, clear_color.y, clear_color.z, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        if (show_demo_window) ImGui::ShowDemoWindow(&show_demo_window);

        // 2. Show a simple window that we create ourselves. We use a Begin/End
        // pair to create a named window.
        {

            ImGui::Begin("Hello, world!"); // Create a window called "Hello,
                                           // world!" and append into it.

            ImGui::Text(
                "This is some useful text."); // Display some text (you can use
                                              // a format strings too)
            ImGui::Checkbox("Demo Window",
                            &show_demo_window); // Edit bools storing our window
                                                // open/close state
            ImGui::Checkbox("Another Window", &show_another_window);

            ImGui::SliderFloat(
                "float",
                &f,
                0.0f,
                1.0f); // Edit 1 float using a slider from 0.0f to 1.0f
            ImGui::ColorEdit3(
                "clear color",
                (float *)&clear_color); // Edit 3 floats representing a color

            if (ImGui::Button(
                    "Button")) // Buttons return true when clicked (most widgets
                               // return true when edited/activated)
                counter++;
            ImGui::SameLine();
            ImGui::Text("counter = %d", counter);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
                        1000.0f / io.Framerate,
                        io.Framerate);
            ImGui::End();
        }

        // 3. Show another simple window.
        if (show_another_window) {
            ImGui::Begin(
                "Another Window",
                &show_another_window); // Pass a pointer to our bool variable
                                       // (the window will have a closing button
                                       // that will clear the bool when clicked)
            ImGui::Text("Hello from another window!");
            if (ImGui::Button("Close Me")) show_another_window = false;
            ImGui::End();
        }

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

        // ImGui rendering
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    /* -------------------------------------------------------------- */

    shutdown(window);
    return 0;
}

GLFWwindow *startup() {
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
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    io.ConfigFlags |=
        ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(
        window, true); // Second param install_callback=true will install
                       // GLFW callbacks and chain to existing ones.
    ImGui_ImplOpenGL3_Init();

    return window;
}

void shutdown(GLFWwindow *window) {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();
}

void process_input(GLFWwindow *window) {
    GLfloat curFrameTime = glfwGetTime();
    GLfloat dt = curFrameTime - lastFrameTime;
    lastFrameTime = curFrameTime;

    camera.update_cam_pos(window, dt);

    // per model logic
    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) {
        x_rotation_deg += 5.0f;
    } else if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) {
        x_rotation_deg -= 5.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
        y_rotation_deg += 5.0f;
    } else if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) {
        y_rotation_deg -= 5.0f;
    }
}
