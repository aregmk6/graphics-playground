#ifndef SHADERS_H
#define SHADERS_H

#include "glad/glad.h"
#include "utilities.h"
#include <filesystem>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <unordered_map>

namespace amk {

class shader {
    GLuint program_id = 0;
    std::unordered_map<std::string, GLint> uniform_map;

    static char shader_src_buff[shader_buff_size];
    static const char *shader_src_ptr;

    void get_shader_data(const std::filesystem::path &p) const;

  public:
    shader(shader &&) = delete;
    shader(const shader &) = delete;
    shader(const std::filesystem::path &vert,
           const std::filesystem::path &frag);
    void use() const;
    GLint insert_uniform(const std::string &uni);
    GLint get_uniform_location(const std::string &uni);
    void assign_sampler(GLint sampler_location, GLuint index);
    void assign_sampler(const std::string &uni, GLuint index);
    void assign_float_uniform(GLint uni_location, GLfloat val);
    void assign_float_uniform(const std::string &uni, GLfloat val);
    void assign_mat_uniform(GLint uni_location, const glm::mat4 &mat);
    void assign_mat_uniform(const std::string &uni, const glm::mat4 &mat);
};

} // namespace amk

#endif
