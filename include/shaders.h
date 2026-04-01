#ifndef SHADERS_H
#define SHADERS_H

#include "glad/glad.h"
#include "glm/matrix.hpp"
#include "utilities.h"
#include <filesystem>
#include <glm/glm.hpp>
#include <unordered_map>

namespace amk {

class shader {
    struct material {
        GLuint ambient;
        GLuint shine;
    };

    struct light {
        GLuint lightPosCoord;
        GLuint ambient;
        GLuint diffuse;
        GLuint specular;
    };

    static constexpr int shader_buff_size = 1024 * 8;
    static char shader_src_buff[shader_buff_size];
    static const char *shader_src_ptr;
    inline static GLuint cur_active_shader = 0;

    GLuint program_id = 0;
    GLuint pvm_location = 0;
    GLuint viewPos_location = 0;
    GLuint fragPos_location = 0;
    GLuint normalMatrix_location = 0;
    material material_locations = {0};
    light light_locations = {0};
    std::unordered_map<std::string, GLint> uniform_map;

    void get_shader_data(const std::filesystem::path &p) const;

  public:
    shader(shader &&) = delete;
    shader(const shader &) = delete;
    shader(const std::filesystem::path &vert,
           const std::filesystem::path &frag);
    GLuint get_id() const;
    bool operator==(const shader &other) const;
    bool operator!=(const shader &other) const;
    void use() const;
    void unuse() const;
    GLint insert_uniform(const std::string &uni);
    GLint get_uniform_location(const std::string &uni);
    void assign_tex_sampler(GLint sampler_location, GLuint index);
    void assign_tex_sampler(const std::string &uni, GLuint index);
    void assign_float_uniform(GLint uni_location, GLfloat val);
    void assign_float_uniform(const std::string &uni, GLfloat val);
    void assign_mat_uniform(GLint uni_location, const glm::mat4 &mat);
    void assign_mat_uniform(const std::string &uni, const glm::mat4 &mat);
    void send_PVM(const glm::mat4 &mat);
};

} // namespace amk

#endif
