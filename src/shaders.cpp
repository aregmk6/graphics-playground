#include "shaders.h"
#include <GLFW/glfw3.h>
#include <fstream>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

using namespace amk;
using std::filesystem::path;

char shader::shader_src_buff[shader_buff_size];
const char *shader::shader_src_ptr = shader_src_buff;

void shader::get_shader_data(const path &p) const {
    std::ifstream ifs(p, std::ios::in | std::ios::ate);
    if (!ifs) {
        std::cerr << "Shader file failed to open" << std::endl;
        exit(EXIT_FAILURE);
    }
    auto file_size = ifs.tellg();
    if (file_size > shader_buff_size) {
        std::cerr << "Shader source code too large" << std::endl;
    }
    ifs.seekg(0);
    ifs.read(shader_src_buff, file_size);
    shader_src_buff[file_size] = '\0';

    ifs.close();
}

shader::shader(const path &vert, const path &frag) {
    path project_root = path(PROJECT_ROOT_PATH);
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    program_id = glCreateProgram();
    int error_res_check = 1;

    get_shader_data(project_root / vert);
    glShaderSource(vertexShader, 1, &shader_src_ptr, NULL);
    glCompileShader(vertexShader);
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &error_res_check);
    ERR_CHECK(error_res_check, "vertex shader compile");

    get_shader_data(project_root / frag);
    glShaderSource(fragmentShader, 1, &shader_src_ptr, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &error_res_check);
    ERR_CHECK(error_res_check, "fragment shader compile");

    glAttachShader(program_id, vertexShader);
    glAttachShader(program_id, fragmentShader);
    glLinkProgram(program_id);
    glGetProgramiv(program_id, GL_LINK_STATUS, &error_res_check);
    ERR_CHECK(error_res_check, "program link");

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // all shaders will have a PVM uniform
    pvm_location = insert_uniform("u_PVM");
    // assign samplers by convention
    assign_tex_sampler("TextureData1", 0);
    assign_tex_sampler("TextureData2", 1);
}

GLuint shader::get_id() const {
    return program_id;
}

bool shader::operator==(const shader &other) const {
    return program_id == other.program_id;
}

bool shader::operator!=(const shader &other) const {
    return !(*this == other);
}

void shader::use() const {
    if (program_id != cur_active_shader) {
        glUseProgram(program_id);
        cur_active_shader = program_id;
    }
}

void shader::unuse() const {
    glUseProgram(0);
}

GLint shader::insert_uniform(const std::string &uni) {
    auto it = uniform_map.find(uni);
    if (it != uniform_map.end()) return it->second;

    GLint uni_loc = glad_glGetUniformLocation(program_id, uni.c_str());

    if (uni_loc == -1) {
        std::cerr << "uniform doesn't exist" << std::endl;
        exit(EXIT_FAILURE);
    }

    uniform_map[uni] = uni_loc;
    return uni_loc;
}

GLint shader::get_uniform_location(const std::string &uni) {
    auto it = uniform_map.find(uni);
    if (it != uniform_map.end()) return it->second;

    return insert_uniform(uni);
}

void shader::assign_tex_sampler(GLint sampler_location, GLuint index) {
    use();
    glad_glUniform1i(sampler_location, index);
}

void shader::assign_tex_sampler(const std::string &uni, GLuint index) {
    GLint sampler_loc = get_uniform_location(uni);
    assign_tex_sampler(sampler_loc, index);
}

void shader::assign_float_uniform(GLint uni_location, GLfloat val) {
    use();
    glad_glUniform1f(uni_location, val);
}

void shader::assign_float_uniform(const std::string &uni, GLfloat val) {
    GLint uni_location = get_uniform_location(uni);
    assign_float_uniform(uni_location, val);
}

void shader::assign_mat_uniform(GLint uni_location, const glm::mat4 &mat) {
    use();
    glad_glUniformMatrix4fv(uni_location, 1, GL_FALSE, glm::value_ptr(mat));
}

void shader::assign_mat_uniform(const std::string &uni, const glm::mat4 &mat) {
    GLint uni_location = get_uniform_location(uni);
    assign_mat_uniform(uni_location, mat);
}

void shader::send_PVM(const glm::mat4 &mat) {
    use();
    glad_glUniformMatrix4fv(pvm_location, 1, GL_FALSE, glm::value_ptr(mat));
}
