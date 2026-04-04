#ifndef UTILS_H
#define UTILS_H

#include <glm/glm.hpp>
#include <iostream>

#define CLEAR_SCREEN                                                           \
  glClearColor(gui.clear_color.x,                                              \
               gui.clear_color.y,                                              \
               gui.clear_color.z,                                              \
               gui.clear_color.w);                                             \
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)

#define ERR_CHECK(arg, name)                                                   \
  do {                                                                         \
    if (!arg) {                                                                \
      std::cerr << "failure: " << name << std::endl;                           \
      glfwTerminate();                                                         \
      exit(EXIT_FAILURE);                                                      \
    }                                                                          \
  } while (0)

constexpr char window_name[]  = "jump";
constexpr int window_width    = 640;
constexpr int window_height   = 480;
constexpr int pitch_max_limit = 89;
constexpr int pitch_min_limit = -89;

#endif
