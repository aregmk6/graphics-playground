#ifndef MASH_H
#define MASH_H

#include "buffers.h"
#include "shaders.h"
#include "texture.h"
#include <vector>

namespace amk {

class mash {

    VBO m_vbo;
    EBO m_ebo;
    VAO m_vao;
    std::vector<texture> m_textures;
    shader *m_cur_shader;

  public:
    mash(const std::vector<vertex> &vertices, //
         const std::vector<GLuint> &indices,  //
         const std::vector<texture> &textures);

    void draw_mash();

    void draw_mash(shader &s);
};

}; // namespace amk

#endif
