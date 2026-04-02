#ifndef MODEL_H
#define MODEL_H

#include "camera.h"
#include "glm/ext/vector_float3.hpp"
#include "mesh.h"
#include "shaders.h"

namespace amk {

class lightModel;

class model {
  private:
    struct material_options {
        glm::vec3 ambient;
        GLfloat shine;
    };

    material_options material_opt;
    std::vector<const lightModel *> lightModels;

  protected:
    virtual void set_options();

    shader *m_cur_shader;
    cameraManager *camera;
    std::vector<mesh> meshes;

    glm::mat4 m_model;

    glm::vec3 m_scale;
    glm::vec3 m_axis_rotations;
    glm::vec3 m_pos;

    glm::mat4 calc_model_mat() const;

  public:
    enum axis { x, y, z };

    model(cameraManager &camera, shader &s);

    model(cameraManager &camera, shader &s, const mesh &m);

    model(cameraManager &camera, shader &s, const std::vector<mesh> &m_v);

    void add_mesh(const mesh &m);

    void add_mesh(const std::vector<mesh> &m);

    void set_model_pos(const glm::vec3 &pos);

    void set_model_scale(const glm::vec3 &scale);

    void set_model_rot(const GLfloat deg, model::axis a);

    void set_model_rot(const glm::vec3 &degs);

    virtual void draw_model();

    void draw_model(shader &s);

    void add_light_model(const lightModel &lm);
};

class lightModel : public model {
  public:
    lightModel(cameraManager &camera, shader &s);

    virtual void draw_model() override;

  protected:
    virtual void set_options() override;

  private:
    struct light_options {
        glm::vec3 ambient;
        glm::vec3 diffuse;
        glm::vec3 specular;
    };

    light_options light_opt;

  public:
    const light_options &get_light_options() const;
};

}; // namespace amk

#endif
