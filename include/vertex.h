#ifndef VERTEX_H
#define VERTEX_H

#include <glm/glm.hpp>

namespace amk {

struct vertex {
    glm::vec3 verPos;
    glm::vec3 normal;
    glm::vec2 texCoord;
};

}; // namespace amk

#endif
