#include "mesh.h"

#include "glm/ext/quaternion_geometric.hpp"
#include "glm/ext/vector_float3.hpp"
#include "texture.h"

using namespace amk;

static constexpr int diffuse_idx  = 0;
static constexpr int specular_idx = 1;

std::vector<amk::vertex> get_vertex_data();
std::vector<amk::vertex> get_solid_color_vertex_data();
std::vector<GLuint> get_index_data();

mesh::mesh()
    : m_vbo(get_vertex_data()), m_ebo(get_index_data()), m_vao(m_vbo, m_ebo)
{
  m_textures.push_back(texture("./textures/bricks.jpg", texture::diffusion));
  m_textures.push_back(texture("./textures/apple.png", texture::specular));
  m_textures[0].set_parameters(GL_REPEAT, GL_REPEAT, GL_NEAREST, GL_LINEAR);
  m_textures[1].set_parameters(
      GL_CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER, GL_NEAREST, GL_LINEAR);
}

mesh::mesh(const glm::vec<3, GLubyte> &rgb)
    : m_vbo(get_vertex_data()), m_ebo(get_index_data()), m_vao(m_vbo, m_ebo)
{
  m_textures.push_back(texture(rgb, texture::diffusion));
  m_textures.push_back(texture(rgb, texture::specular));
  m_textures[0].set_parameters(GL_REPEAT, GL_REPEAT, GL_NEAREST, GL_NEAREST);
  m_textures[1].set_parameters(GL_REPEAT, GL_REPEAT, GL_NEAREST, GL_NEAREST);
}

mesh::mesh(const std::vector<vertex> &vertices,
           const std::vector<GLuint> &indices,
           const std::vector<texture> &textures)
    : m_vbo(vertices), m_ebo(indices), m_vao(m_vbo, m_ebo), m_textures(textures)
{
}

void mesh::draw_mesh(shader &s)
{
  for (auto &tex : m_textures) {
    switch (tex.get_type()) {
    case texture::textureType::diffusion:
      glad_glActiveTexture(GL_TEXTURE0);
      tex.bind();
      break;
    case texture::textureType::specular:
      glad_glActiveTexture(GL_TEXTURE1);
      tex.bind();
      break;
    }
  }
  s.use();
  m_vao.bind();
  glDrawElements(GL_TRIANGLES, m_ebo.nr_of_indices(), GL_UNSIGNED_INT, nullptr);
}

std::vector<GLuint> get_index_data()
{
  constexpr int vert_per_face = 4;
  // constexpr int order[]       = {0, 1, 3, 1, 2, 3};
  constexpr int order[] = {3, 2, 1, 0, 3, 1};
  std::vector<GLuint> vertcies{};
  for (int i = 0; i < 6; ++i) {
    for (int j = 0; j < 6; ++j) {
      vertcies.push_back(i * vert_per_face + order[j]);
    }
  }

  return vertcies;
}

std::vector<amk::vertex> get_vertex_data()
{
  // for now I'll hardcode it.
  std::vector<glm::vec3> //
      pos{
          { 1.0f,  1.0f,  1.0f}, // top right
          { 1.0f, -1.0f,  1.0f}, // bottom right
          {-1.0f, -1.0f,  1.0f}, // bottom left
          {-1.0f,  1.0f,  1.0f}, // top left

          {-1.0f,  1.0f, -1.0f}, // top right
          {-1.0f, -1.0f, -1.0f}, // bottom right
          { 1.0f, -1.0f, -1.0f}, // bottom left
          { 1.0f,  1.0f, -1.0f}, // top left

          { 1.0f,  1.0f, -1.0f}, // top right
          { 1.0f, -1.0f, -1.0f}, // bottom right
          { 1.0f, -1.0f,  1.0f}, // bottom left
          { 1.0f,  1.0f,  1.0f}, // top left

          {-1.0f,  1.0f,  1.0f}, // top right
          {-1.0f, -1.0f,  1.0f}, // bottom right
          {-1.0f, -1.0f, -1.0f}, // bottom left
          {-1.0f,  1.0f, -1.0f}, // top left

          { 1.0f,  1.0f, -1.0f}, // top right
          { 1.0f,  1.0f,  1.0f}, // bottom right
          {-1.0f,  1.0f,  1.0f}, // bottom left
          {-1.0f,  1.0f, -1.0f}, // top left

          {-1.0f, -1.0f, -1.0f}, // top right
          {-1.0f, -1.0f,  1.0f}, // bottom right
          { 1.0f, -1.0f,  1.0f}, // bottom left
          { 1.0f, -1.0f, -1.0f}  // top left
  };

  std::vector<glm::vec3> normals{};
  glm::vec3 norm{0.0f};
  for (auto it = pos.begin(); it != pos.end(); it += 4) {
    norm = glm::vec3{0.0f};
    for (int i = 0; i < 4; ++i) {
      norm += *(it + i);
    }
    for (int i = 0; i < 4; ++i) {
      normals.push_back(glm::normalize(norm));
    }
  }

  std::vector<glm::vec2> texCoord{
      {1.0f, 1.0f},
      {1.0f, 0.0f},
      {0.0f, 0.0f},
      {0.0f, 1.0f},

      {1.0f, 1.0f},
      {1.0f, 0.0f},
      {0.0f, 0.0f},
      {0.0f, 1.0f},

      {1.0f, 1.0f},
      {1.0f, 0.0f},
      {0.0f, 0.0f},
      {0.0f, 1.0f},

      {1.0f, 1.0f},
      {1.0f, 0.0f},
      {0.0f, 0.0f},
      {0.0f, 1.0f},

      {1.0f, 1.0f},
      {1.0f, 0.0f},
      {0.0f, 0.0f},
      {0.0f, 1.0f},

      {1.0f, 1.0f},
      {1.0f, 0.0f},
      {0.0f, 0.0f},
      {0.0f, 1.0f}
  };

  std::vector<vertex> vertices;
  for (int i = 0; i < pos.size(); ++i) {
    vertices.push_back({pos[i], normals[i], texCoord[i]});
  }

  return vertices;
}

std::vector<amk::vertex> get_solid_color_vertex_data()
{
  // for now I'll hardcode it.
  std::vector<glm::vec3> //
      pos{
          { 1.0f,  1.0f,  1.0f}, // top right
          { 1.0f, -1.0f,  1.0f}, // bottom right
          {-1.0f, -1.0f,  1.0f}, // bottom left
          {-1.0f,  1.0f,  1.0f}, // top left

          { 1.0f,  1.0f, -1.0f}, // top right
          { 1.0f, -1.0f, -1.0f}, // bottom right
          {-1.0f, -1.0f, -1.0f}, // bottom left
          {-1.0f,  1.0f, -1.0f}, // top left

          { 1.0f,  1.0f, -1.0f}, // top right
          { 1.0f, -1.0f, -1.0f}, // bottom right
          { 1.0f, -1.0f,  1.0f}, // bottom left
          { 1.0f,  1.0f,  1.0f}, // top left

          {-1.0f,  1.0f, -1.0f}, // top right
          {-1.0f, -1.0f, -1.0f}, // bottom right
          {-1.0f, -1.0f,  1.0f}, // bottom left
          {-1.0f,  1.0f,  1.0f}, // top left

          { 1.0f,  1.0f, -1.0f}, // top right
          { 1.0f,  1.0f,  1.0f}, // bottom right
          {-1.0f,  1.0f,  1.0f}, // bottom left
          {-1.0f,  1.0f, -1.0f}, // top left

          { 1.0f, -1.0f, -1.0f}, // top right
          { 1.0f, -1.0f,  1.0f}, // bottom right
          {-1.0f, -1.0f,  1.0f}, // bottom left
          {-1.0f, -1.0f, -1.0f}  // top left
  };

  std::vector<glm::vec3> normals{};
  glm::vec3 norm{0.0f};
  for (auto it = pos.begin(); it != pos.end(); it += 4) {
    norm = glm::vec3{0.0f};
    for (int i = 0; i < 4; ++i) {
      norm += *(it + i);
    }
    for (int i = 0; i < 4; ++i) {
      normals.push_back(glm::normalize(norm));
    }
  }

  std::vector<glm::vec2> texCoord;
  for (int i = 0; i < pos.size(); ++i) {
    texCoord.push_back({0.0f, 0.0f});
  }

  std::vector<vertex> vertices;
  for (int i = 0; i < pos.size(); ++i) {
    vertices.push_back({pos[i], normals[i], texCoord[i]});
  }

  return vertices;
}
