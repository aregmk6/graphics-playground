#ifndef UTILS_H
#define UTILS_H

#include <glm/glm.hpp>
#include <iostream>
#include <vector>

static std::vector<glm::vec3> model_pos = {glm::vec3(-1.0f, 0.0f, -1.0f),
                                           glm::vec3(0.0f, 0.0f, -3.0f),
                                           glm::vec3(1.0f, 1.0f, -0.5f),
                                           glm::vec3(4.0f, 2.0f, -9.0f),
                                           glm::vec3(8.0f, 3.0f, -1.5f),
                                           glm::vec3(2.0f, -4.0f, -0.5f),
                                           glm::vec3(3.0f, 6.0f, -0.5f),
                                           glm::vec3(-5.0f, 10.0f, 3.0f)};

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
constexpr int pitch_max_limit = 89;
constexpr int pitch_min_limit = -89;

#endif
