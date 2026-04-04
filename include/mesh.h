#ifndef MESH_H
#define MESH_H

#include "buffers.h"
#include "shaders.h"
#include "texture.h"

#include <vector>

namespace amk
{

class mesh
{
  VBO m_vbo;
  EBO m_ebo;
  VAO m_vao;
  std::vector<texture> m_textures;

  GLuint cur_shader_id = 0;

public:
  mesh();
  mesh(const glm::vec<3, GLubyte> &rgb);
  mesh(const std::vector<vertex> &vertices, //
       const std::vector<GLuint> &indices,  //
       const std::vector<texture> &textures);

  void draw_mesh(shader &s);
};

}; // namespace amk

#endif
