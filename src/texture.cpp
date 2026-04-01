#include "texture.h"
#include "camera.h"
#include "stb_image.h"
#include "utilities.h"
#include <GLFW/glfw3.h>

using namespace amk;
using std::filesystem::path;

texture::texture() {
    glad_glGenTextures(1, &texture_id);
}

texture::texture(const glm::vec<3, GLubyte> &rgb, textureType type) {
    glad_glGenTextures(1, &texture_id);
    // default diffusion type
    set_solid_color_texture(rgb, type);
}

texture::texture(const path &p, textureType type, GLenum tex_target) {
    glad_glGenTextures(1, &texture_id);
    set_texture(p, type, tex_target);
}

GLuint texture::id() const {
    return texture_id;
}

texture::textureType texture::get_type() const {
    return m_type;
}

void texture::set_solid_color_texture(const glm::vec<3, GLubyte> &rgb,
                                      textureType type, GLenum tex_target) {
    if (!target_set) {
        m_tex_target = tex_target;
        target_set = true;
    }

    GLubyte pixel[] = {rgb.x, rgb.y, rgb.z, 255};

    m_type = type;

    bind();
    glad_glTexImage2D(m_tex_target, //
                      0,
                      GL_RGBA,
                      1,
                      1,
                      0,
                      GL_RGBA,
                      GL_UNSIGNED_BYTE,
                      pixel);
    unbind();
}

void texture::set_texture(const path &p, textureType type, GLenum tex_target) {
    if (!target_set) {
        m_tex_target = tex_target;
        target_set = true;
    }

    m_type = type;

    bind();

    stbi_set_flip_vertically_on_load(true);
    int img_width, img_height, channel_nr;
    path full_path = path(PROJECT_ROOT_PATH) / p;
    GLubyte *img = stbi_load( //
        full_path.c_str(),
        &img_width,
        &img_height,
        &channel_nr,
        0);
    ERR_CHECK(img, "stbi_load");

    GLint internal_format;
    GLenum external_format;
    switch (channel_nr) {
    case 1:
        internal_format = GL_RED;
        external_format = GL_RED;
        break;
    case 2:
        internal_format = GL_RG;
        external_format = GL_RG;
        break;
    case 3:
        internal_format = GL_RGB;
        external_format = GL_RGB;
        break;
    case 4:
        internal_format = GL_RGBA;
        external_format = GL_RGBA;
        break;
    default:
        internal_format = GL_RGB;
        external_format = GL_RGB;
        break;
    }

    glad_glTexImage2D(m_tex_target, //
                      0,
                      internal_format,
                      img_width,
                      img_height,
                      0,
                      external_format,
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
