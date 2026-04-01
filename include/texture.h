#ifndef TEXTURE_H
#define TEXTURE_H

#include "glad/glad.h"
#include <filesystem>
#include <glm/glm.hpp>

namespace amk {

using std::filesystem::path;

class texture {
  public:
    enum textureType { diffusion, specular };

    texture();

    texture(const glm::vec<3, GLubyte> &rgb);

    texture(const path &p, textureType type, GLenum tex_target = GL_TEXTURE_2D);

    GLuint id() const;
    textureType get_type() const;

    void set_solid_color_texture(const glm::vec<3, GLubyte> &rgb);

    void set_texture(const path &p, textureType type,
                     GLenum tex_target = GL_TEXTURE_2D);

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
