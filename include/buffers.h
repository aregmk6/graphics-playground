#ifndef BUFFERS_H
#define BUFFERS_H

#include "glad/glad.h"
#include "utilities.h"
#include "vertex.h"
#include <vector>

namespace amk {

class EBO {
    GLuint ebo_id;

  public:
    EBO();

    EBO(const std::vector<GLuint> &indices);

    void bind();

    void unbind();

    void set_data(const std::vector<GLuint> &indices);
};

class VBO {
    GLuint vbo_id;

  public:
    VBO();

    VBO(const std::vector<vertex> &vertices);

    void bind();

    void unbind();

    void set_data(const std::vector<vertex> &vertices);
};

class VAO {
    GLuint vao_id;
    VBO v_buffer;
    EBO e_buffer;

  public:
    VAO();

    VAO(VBO &vbo, EBO &ebo);

    void bind();

    void unbind();

    /* VBO must have data inside of it */
    void add_vbo(VBO &vbo);

    /* EBO must have data inside of it */
    void add_ebo(EBO &ebo);
};

}; // namespace amk

#endif
