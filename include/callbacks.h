#ifndef CALLBACKS_H
#define CALLBACKS_H

#include "camera.h"
#include <GLFW/glfw3.h>

namespace amk {

class callbackManager {
    inline static cameraManager *cam = nullptr;
    inline static GLFWwindow *win = nullptr;
    inline static bool inside = false;
    inline static bool mouse_on = true;

  public:
    callbackManager(GLFWwindow *window, cameraManager &camera);

    static void error_callback(int error_num, const char *info);

    static void key_callback(GLFWwindow *window, int key, int scancode,
                             int action, int mods);

    static void mouse_pos_callback(GLFWwindow *window, double xpos,
                                   double ypos);

    static void mouse_enter_callback(GLFWwindow *window, int entered);

    static void framebuffer_size_callback(GLFWwindow *window, int width,
                                          int height);
};

} // namespace amk

#endif
