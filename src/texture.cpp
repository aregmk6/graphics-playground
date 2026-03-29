#include "texture.h"
#include "utilities.h"
#include <iostream>

using namespace amk;
using std::filesystem::path;

texture::texture() {
    glad_glGenTextures(1, &texture_id);
};

texture::texture(const path &p, GLint internal_format, bool flip_mode,
                 GLenum tex_target) {
    glad_glGenTextures(1, &texture_id);
    set_texture(p, internal_format, flip_mode, tex_target);
}

GLuint texture::id() const {
    return texture_id;
}

void texture::set_texture(const path &p, GLint internal_format, bool flip_mode,
                          GLenum tex_target) {

    if (!target_set) {
        m_tex_target = tex_target;
        target_set = true;
    }

    bind();

    stbi_set_flip_vertically_on_load(flip_mode);
    int img_width, img_height, channel_nr;
    path full_path = path(PROJECT_ROOT_PATH) / p;
    GLubyte *img = stbi_load( //
        full_path.c_str(),
        &img_width,
        &img_height,
        &channel_nr,
        0);
    ERR_CHECK(img, "stbi_load");

    glad_glTexImage2D(m_tex_target, //
                      0,
                      internal_format,
                      img_width,
                      img_height,
                      0,
                      internal_format,
                      GL_UNSIGNED_BYTE,
                      img);
    glad_glGenerateMipmap(m_tex_target);
    stbi_image_free(img);

    unbind();
}

void texture::set_parameters(GLint s_wrap_param, GLint t_wrap_param,
                             GLint min_fliter_param, GLint mag_filter_param) {

    bind();

    glad_glTexParameteri(m_tex_target, GL_TEXTURE_WRAP_S, s_wrap_param);
    glad_glTexParameteri(m_tex_target, GL_TEXTURE_WRAP_T, t_wrap_param);
    glad_glTexParameteri(m_tex_target, GL_TEXTURE_MIN_FILTER, min_fliter_param);
    glad_glTexParameteri(m_tex_target, GL_TEXTURE_MAG_FILTER, mag_filter_param);

    unbind();
}

void texture::bind() {
    if (!target_set) {
        ERR_CHECK(target_set, "target not set for texture");
    }
    glad_glBindTexture(m_tex_target, texture_id);
}

void texture::unbind() {
    glad_glBindTexture(m_tex_target, 0);
}
