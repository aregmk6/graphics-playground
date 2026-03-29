#ifndef VERTEX_H
#define VERTEX_H

#include "glad/glad.h"
#include "stb_image.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

namespace amk {

struct vertex {
    glm::vec3 verPos;
    glm::vec2 texCoord;
};

}; // namespace amk

#endif
