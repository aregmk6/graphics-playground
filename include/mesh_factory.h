#ifndef MESH_FACTORY_H
#define MESH_FACTORY_H

#include "glad/glad.h"
#include "mesh.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace amk {

class meshFactory {
    meshFactory() {}

  public:
    meshFactory(const meshFactory &) = delete;
    meshFactory &operator=(const meshFactory &) = delete;
    ~meshFactory() {}

    meshFactory &get_instance() const {
        static meshFactory instance;
        return instance;
    }

    mesh make_cube() const;
    mesh make_sphere() const;
};

} // namespace amk

#endif
