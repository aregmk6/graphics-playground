#include "mesh.h"
#include "texture.h"

using namespace amk;

static constexpr int diffuse_idx = 0;
static constexpr int specular_idx = 1;

std::vector<amk::vertex> get_vertex_data();
std::vector<GLuint> get_index_data();

mesh::mesh()
    : m_vbo(get_vertex_data()), m_ebo(get_index_data()), m_vao(m_vbo, m_ebo) {
    m_textures.push_back(texture("./textures/bricks.jpg", texture::diffusion));
    m_textures.push_back(texture("./textures/apple.png", texture::specular));
    m_textures[0].set_parameters(GL_REPEAT, GL_REPEAT, GL_NEAREST, GL_LINEAR);
    m_textures[1].set_parameters(
        GL_CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER, GL_NEAREST, GL_LINEAR);
}

mesh::mesh(const std::vector<vertex> &vertices,  //
           const std::vector<GLuint> &indices,   //
           const std::vector<texture> &textures) //
    : m_vbo(vertices), m_ebo(indices), m_vao(m_vbo, m_ebo),
      m_textures(textures) {}

void mesh::draw_mesh(shader &s) {
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
    glDrawElements(
        GL_TRIANGLES, m_ebo.nr_of_indices(), GL_UNSIGNED_INT, nullptr);
}

std::vector<GLuint> get_index_data() {
    constexpr int vert_per_face = 4;
    constexpr int order[] = {0, 1, 3, 1, 2, 3};
    std::vector<GLuint> vertcies{};
    for (int i = 0; i < 6; ++i) {
        for (int j = 0; j < 6; ++j) {
            vertcies.push_back(i * vert_per_face + order[j]);
        }
    }

    return vertcies;
}

std::vector<amk::vertex> get_vertex_data() {
    // for now I'll hardcode it.
    std::vector<amk::vertex> //
        vertcies{
            {{0.5f, 0.5f, 0.5f}, {1.0f, 1.0f}},   // top right
            {{0.5f, -0.5f, 0.5f}, {1.0f, 0.0f}},  // bottom right
            {{-0.5f, -0.5f, 0.5f}, {0.0f, 0.0f}}, // bottom left
            {{-0.5f, 0.5f, 0.5f}, {0.0f, 1.0f}},  // top left

            {{0.5f, 0.5f, -0.5f}, {1.0f, 1.0f}},   // top right
            {{0.5f, -0.5f, -0.5f}, {1.0f, 0.0f}},  // bottom right
            {{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f}}, // bottom left
            {{-0.5f, 0.5f, -0.5f}, {0.0f, 1.0f}},  // top left

            {{0.5f, 0.5f, -0.5f}, {1.0f, 1.0f}},  // top right
            {{0.5f, -0.5f, -0.5f}, {1.0f, 0.0f}}, // bottom right
            {{0.5f, -0.5f, 0.5f}, {0.0f, 0.0f}},  // bottom left
            {{0.5f, 0.5f, 0.5f}, {0.0f, 1.0f}},   // top left

            {{-0.5f, 0.5f, -0.5f}, {1.0f, 1.0f}},  // top right
            {{-0.5f, -0.5f, -0.5f}, {1.0f, 0.0f}}, // bottom right
            {{-0.5f, -0.5f, 0.5f}, {0.0f, 0.0f}},  // bottom left
            {{-0.5f, 0.5f, 0.5f}, {0.0f, 1.0f}},   // top left

            {{0.5f, 0.5f, -0.5f}, {1.0f, 1.0f}},  // top right
            {{0.5f, 0.5f, 0.5f}, {1.0f, 0.0f}},   // bottom right
            {{-0.5f, 0.5f, 0.5f}, {0.0f, 0.0f}},  // bottom left
            {{-0.5f, 0.5f, -0.5f}, {0.0f, 1.0f}}, // top left

            {{0.5f, -0.5f, -0.5f}, {1.0f, 1.0f}}, // top right
            {{0.5f, -0.5f, 0.5f}, {1.0f, 0.0f}},  // bottom right
            {{-0.5f, -0.5f, 0.5f}, {0.0f, 0.0f}}, // bottom left
            {{-0.5f, -0.5f, -0.5f}, {0.0f, 1.0f}} // top left
        };
    return vertcies;
}
