#ifndef UTILS_H
#define UTILS_H

#include "glad/glad.h"
#include "stb_image.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#define ERR_CHECK(arg, name)                                                   \
    do {                                                                       \
        if (!arg) {                                                            \
            std::cerr << name << " failed" << std::endl;                       \
            glfwTerminate();                                                   \
            exit(EXIT_FAILURE);                                                \
        }                                                                      \
    } while (0)

constexpr char window_name[] = "jump";
constexpr int window_width = 640;
constexpr int window_height = 480;
constexpr int shader_buff_size = 1024 * 8;
constexpr int pitch_max_limit = 89;
constexpr int pitch_min_limit = -89;

#endif
