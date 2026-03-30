#ifndef CALLBACKS_H
#define CALLBACKS_H

#include "camera.h"
#include <GLFW/glfw3.h>

namespace amk {

class callbackManager {
    inline static cameraManager *cam = nullptr;

  public:
    callbackManager(cameraManager &camera);

    static void error_callback(int error_num, const char *info);

    static void key_callback(GLFWwindow *window, int key, int scancode,
                             int action, int mods);

    static void mouse_pos_callback(GLFWwindow *window, double xpos,
                                   double ypos);

    static void mouse_enter_callback(GLFWwindow *window, int entered);
};

} // namespace amk

#endif
