#define GLFW_INCLUDE_NONE

#include "buffers.h"
#include "callbacks.h"
#include "camera.h"
#include "glad/glad.h"
#include "glm/ext/vector_float3.hpp"
#include "gui.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "model.h"
#include "modelManager.h"
#include "shaders.h"
#include "stb_image.h"
#include "utilities.h"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <memory>
#include <vector>

static GLfloat lastFrameTime = 0.0f;

static amk::cameraManager camera;
static GLFWwindow *startup();
static void shutdown(GLFWwindow *window, amk::guiManager &gui);
static void process_input(GLFWwindow *window, amk::modelManager &model_manager);
static void init_map(amk::modelManager &model_manager);

int main()
{
  GLFWwindow *window = startup();
  static amk::callbackManager callback_manager{window, camera};
  amk::modelManager &model_manager = amk::modelManager::get_instance(camera);
  static amk::guiManager gui(window, camera, model_manager);

  init_map(model_manager);

  /* ---------------------------- loop ---------------------------- */

  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();

    CLEAR_SCREEN;

    process_input(window, model_manager);

    model_manager.draw();

    gui.draw();

    glfwSwapBuffers(window);
  }

  /* -------------------------------------------------------------- */

  shutdown(window, gui);
  return 0;
}

GLFWwindow *startup()
{
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
  glad_glEnable(GL_CULL_FACE);

  return window;
}

void shutdown(GLFWwindow *window, amk::guiManager &gui)
{
  gui.shutdown();
  glfwDestroyWindow(window);
  glfwTerminate();
}

void process_input(GLFWwindow *window, amk::modelManager &model_manager)
{
  GLfloat curFrameTime = glfwGetTime();
  GLfloat dt           = curFrameTime - lastFrameTime;
  lastFrameTime        = curFrameTime;

  camera.update_cam_pos(window, dt);

  // per model logic
  if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) {
    model_manager.cur_model_rot->x += 5.0f;
  } else if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) {
    model_manager.cur_model_rot->x -= 5.0f;
  }
  if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
    model_manager.cur_model_rot->y += 5.0f;
  } else if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) {
    model_manager.cur_model_rot->y -= 5.0f;
  }
}

static void init_map(amk::modelManager &model_manager)
{

  static constexpr glm::vec3 pos[] = {
      { 0.0f, 5.0f,  0.0f},
      { 0.0f, 0.0f, -5.0f},
      { 5.0f, 0.0f,  0.0f},
      {-5.0f, 0.0f,  0.0f},
      { 0.0f, 0.0f,  5.0f},
  };

  int indx = 0;
  for (auto &model : model_manager.mModels) {
    model->set_model_pos(pos[indx]);
    indx = (indx + 1) % model_manager.mModels.size();
  }
}
