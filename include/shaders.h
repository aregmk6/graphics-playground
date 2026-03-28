#ifndef SHADERS_H
#define SHADERS_H

#include "glad/glad.h"
#include "utilities.h"
#include <filesystem>
#include <unordered_map>

namespace amk {

class shader {
    GLuint program_id = 0;
    std::unordered_map<std::string, GLint> uniform_map;

    static char shader_src_buff[shader_buff_size];
    static const char *shader_src_ptr;

    void get_shader_data(const std::filesystem::path &p) const;

  public:
    shader(const std::filesystem::path &vert,
           const std::filesystem::path &frag);
    void use() const;
    GLint insert_uniform(const std::string &uni);
    GLint get_uniform_location(const std::string &uni);
    void assign_sampler(GLint sampler_location, GLuint index);
    void assign_sampler(const std::string &uni, GLuint index);
};

} // namespace amk

#endif
