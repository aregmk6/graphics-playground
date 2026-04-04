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
    // class definitions
  private:
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

    class programHandler {
      public:
        programHandler();
        programHandler(GLuint id);

        programHandler(programHandler &&other);

        programHandler(programHandler &other) = delete;

        GLuint get_id() const;

      private:
        GLuint mProgram_id;
    };

  public:
    enum class shaderTypes { regular, emissive };
    static constexpr shaderTypes type = shaderTypes::regular;

    shader(shader &&) = default;
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
    void assign_vec3_uniform(GLint uni_location, const glm::vec3 &v);
    void assign_vec3_uniform(const std::string &uni, const glm::vec3 &v);
    void assign_mat3_uniform(GLint uni_location, const glm::mat3 &mat);
    void assign_mat3_uniform(const std::string &uni, const glm::mat3 &mat);
    void assign_mat4_uniform(GLint uni_location, const glm::mat4 &mat);
    void assign_mat4_uniform(const std::string &uni, const glm::mat4 &mat);
    void send_pvm(const glm::mat4 &mat);
    void send_light_pos(const glm::vec3 &pos);
    void send_view_pos(const glm::vec3 &pos);
    void send_model_matrix(const glm::mat4 &mat);
    void send_normal_matrix(const glm::mat3 &mat);
    void send_light_options(glm::vec3 ambient, glm::vec3 diffuse,
                            glm::vec3 specular);
    void send_material_options(glm::vec3 ambient, GLfloat shine);

  protected:
    static constexpr int shader_buff_size = 1024 * 8;
    static char shader_src_buff[shader_buff_size];
    static const char *shader_src_ptr;
    inline static GLuint cur_active_shader = 0;

    shader();
    programHandler mProg_handler;
    GLuint pvm_location = 0;
    GLuint viewPos_location = 0;
    GLuint normalMatrix_location = 0;
    GLuint modelMatrix_location = 0;
    material material_struct_location = {0};
    light light_struct_location = {0};
    std::unordered_map<std::string, GLint> uniform_map;

    void initializer(const std::filesystem::path &vert,
                     const std::filesystem::path &frag);
    void get_shader_data(const std::filesystem::path &p) const;
    virtual void assign_uniforms();
};

class lightShader : public shader {
  public:
    lightShader(lightShader &&) = default;
    lightShader(const lightShader &) = delete;

    lightShader(const std::filesystem::path &vert,
                const std::filesystem::path &frag);

    static constexpr shaderTypes type = shaderTypes::emissive;

  protected:
    virtual void assign_uniforms() override;

  private:
};

}; // namespace amk

#endif
