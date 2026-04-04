#ifndef MODEL_H
#define MODEL_H

#include "camera.h"
#include "glm/ext/vector_float3.hpp"
#include "mesh.h"
#include "shaders.h"

namespace amk
{

class lightModel; // forward declare

class model
{
public:
  enum axis { x, y, z };

  struct srp_data { // scale - rotation - position
    glm::vec3 scale;
    glm::vec3 rotation;
    glm::vec3 pos;
  };

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

  void send_srp(srp_data &data);

  void get_srp(glm::vec3 &pos, glm::vec3 &rot, glm::vec3 &scale) const;

protected:
  virtual void set_options();

  shader *m_cur_shader;
  cameraManager *camera;
  std::vector<mesh> meshes;

  glm::vec3 m_scale;
  glm::vec3 m_axis_rotations;
  glm::vec3 m_pos;

  glm::mat4 calc_model_mat() const;

private:
  struct material_options {
    glm::vec3 ambient;
    GLfloat shine;
  };

  material_options material_opt;
  std::vector<const lightModel *> lightModels;

  friend class modelManager;
};

class lightModel : public model
{
private:
  struct light_options {
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
  };

public:
  lightModel(cameraManager &camera, shader &s);

  virtual void draw_model() override;

  const light_options &get_light_options() const;

protected:
  virtual void set_options() override;

private:
  light_options light_opt;
};

}; // namespace amk

#endif
