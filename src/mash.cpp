#include "mash.h"

using namespace amk;

static constexpr int diffuse_idx = 0;
static constexpr int specular_idx = 1;

mash::mash(const std::vector<vertex> &vertices,  //
           const std::vector<GLuint> &indices,   //
           const std::vector<texture> &textures) //
    : m_vbo(vertices), m_ebo(indices), m_vao(m_vbo, m_ebo),
      m_textures(textures) {}

void mash::draw_mash() {
    for (auto &tex : m_textures) {
        switch (tex.get_type()) {
        case texture::textureType::DIFFUSION:
            glad_glActiveTexture(GL_TEXTURE0);
            tex.bind();
        case texture::textureType::SPECULAR:
            glad_glActiveTexture(GL_TEXTURE1);
            tex.bind();
        }
    }
    m_cur_shader->use();
    m_vao.bind();
    glDrawElements(
        GL_TRIANGLES, m_ebo.nr_of_indices(), GL_UNSIGNED_INT, nullptr);
}

void mash::draw_mash(shader &s) {
    if (s != *m_cur_shader) {
        m_cur_shader = &s;
    }
    draw_mash();
}
