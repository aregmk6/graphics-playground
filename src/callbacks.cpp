#include "callbacks.h"

#include "camera.h"

#include <GLFW/glfw3.h>
#include <iostream>

using namespace amk;

callbackManager::callbackManager(GLFWwindow *window, cameraManager &camera)
{
  cam = &camera;
  win = window;

  GLint width, height;
  glfwGetFramebufferSize(win, &width, &height);
  glViewport(0, 0, width, height);

  glfwSetErrorCallback(error_callback);
  glfwSetKeyCallback(win, key_callback);
  glfwSetCursorPosCallback(win, mouse_pos_callback);
  glfwSetCursorEnterCallback(win, mouse_enter_callback);
  glfwSetFramebufferSizeCallback(win, framebuffer_size_callback);
}

void callbackManager::error_callback(int error_num, const char *info)
{
  std::cerr << "Error number: " << error_num //
            << ". description: " << info     //
            << std::endl;                    //
}

void callbackManager::key_callback(
    GLFWwindow *window, int key, int scancode, int action, int mods)
{
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, GLFW_TRUE);
  }
  if (key == GLFW_KEY_R && action == GLFW_PRESS) {
    int mode = glfwGetInputMode(window, GLFW_CURSOR);
    switch (mode) {
    case GLFW_CURSOR_NORMAL:
      glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
      mouse_on = true;
      break;
    case GLFW_CURSOR_DISABLED:
      mouse_on = false;
      glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
      break;
    }
  }
}

void callbackManager::mouse_pos_callback(GLFWwindow *window,
                                         double xpos,
                                         double ypos)
{
  if (mouse_on) {
    if (inside) {
      cam->set_last_coords(xpos, ypos);
      inside = false;
    }
    cam->update_cam_front(window, xpos, ypos);
  }
}

void callbackManager::mouse_enter_callback(GLFWwindow *window, int entered)
{
  if (entered) {
    inside = true;
  }
}

void callbackManager::framebuffer_size_callback(GLFWwindow *window,
                                                int width,
                                                int height)
{
  glViewport(0, 0, width, height);
  cam->update_aspect_ratio(width, height);
}
