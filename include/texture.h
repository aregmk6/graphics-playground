#ifndef TEXTURE_H
#define TEXTURE_H

#include "glad/glad.h"
#include <filesystem>

namespace amk {

using std::filesystem::path;

class texture {
  public:
    enum textureType { DIFFUSION, SPECULAR };

    texture();

    texture(const path &p, GLint internal_format = GL_RGB,
            bool flip_mode = false, GLenum tex_target = GL_TEXTURE_2D);

    GLuint id() const;
    textureType get_type() const;

    void set_texture(const path &p, GLint internal_format = GL_RGB,
                     bool flip_mode = false, GLenum tex_target = GL_TEXTURE_2D);

    void set_parameters(GLint s_wrap_param, GLint t_wrap_param,
                        GLint min_fliter_param, GLint mag_filter_param);

    void bind();

    void unbind();

  private:
    GLuint texture_id;
    GLenum m_tex_target;
    bool target_set = false;
    textureType m_type;
};

}; // namespace amk

#endif
