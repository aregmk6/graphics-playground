#include "callbacks.h"
#include "camera.h"
#include <GLFW/glfw3.h>
#include <iostream>

using namespace amk;

callbackManager::callbackManager(cameraManager &camera) {
    cam = &camera;
}

void callbackManager::error_callback(int error_num, const char *info) {
    std::cerr << "Error number: " << error_num //
              << ". description: " << info     //
              << std::endl;                    //
}

void callbackManager::key_callback(GLFWwindow *window, int key, int scancode,
                                   int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

void callbackManager::mouse_pos_callback(GLFWwindow *window, double xpos,
                                         double ypos) {
    cam->update_cam_front(window, xpos, ypos);
}

void callbackManager::mouse_enter_callback(GLFWwindow *window, int entered) {
    if (entered) {
        std::cout << "entered window" << std::endl;
    }
}
