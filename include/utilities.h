#ifndef UTILS_H
#define UTILS_H

#define ERR_CHECK(arg, name)                                                   \
    do {                                                                       \
        if (!arg) {                                                            \
            std::cerr << name << " failed" << std::endl;                       \
            glfwTerminate();                                                   \
            exit(EXIT_FAILURE);                                                \
        }                                                                      \
    } while (0)

constexpr char w_name[] = "jump";
constexpr int w_width = 640;
constexpr int w_height = 480;
constexpr int shader_buff_size = 1024 * 8;
constexpr int pitch_max_limit = 89;
constexpr int pitch_min_limit = -89;

enum class attrib { COORD = 0, TEX = 1 };

#endif
