#include "model.h"

#include "glm/ext/vector_float3.hpp"

static constexpr glm::mat4 DIAG(1.0f);
static constexpr glm::vec3 XUNIT(1.0f, 0.0f, 0.0f);
static constexpr glm::vec3 YUNIT(0.0f, 1.0f, 0.0f);
static constexpr glm::vec3 ZUNIT(0.0f, 0.0f, 1.0f);

using namespace amk;

model::model(cameraManager &camera, shader &s) //
    : m_cur_shader(&s),                        //
      camera(&camera),                         //
      meshes{mesh{}},                          //
      m_scale(1.0f),                           //
      m_axis_rotations(0.0f),                  //
      m_pos(0.0f)
{
  set_options();
}

model::model(cameraManager &camera, shader &s, const mesh &m) //
    : m_cur_shader(&s),                                       //
      camera(&camera),                                        //
      m_scale(1.0f),                                          //
      m_axis_rotations(0.0f),                                 //
      m_pos(0.0f)
{
  set_options();
  add_mesh(m);
}

model::model(cameraManager &camera, shader &s, const std::vector<mesh> &m_v)
    : m_cur_shader(&s),       //
      camera(&camera),        //
      m_scale(1.0f),          //
      m_axis_rotations(0.0f), //
      m_pos(0.0f)
{
  set_options();
  add_mesh(m_v);
}

void model::add_mesh(const mesh &m)
{
  meshes.push_back(m);
}

void model::add_mesh(const std::vector<mesh> &m)
{
  meshes.insert(meshes.end(), m.begin(), m.end());
}

void model::set_model_pos(const glm::vec3 &pos)
{
  m_pos = pos;
}

void model::set_model_scale(const glm::vec3 &scale)
{
  m_scale = scale;
}

void model::set_model_rot(const GLfloat deg, model::axis a)
{
  switch (a) {
  case x:
    m_axis_rotations.x = deg;
    break;
  case y:
    m_axis_rotations.y = deg;
    break;
  case z:
    m_axis_rotations.z = deg;
    break;
  }
}

void model::set_model_rot(const glm::vec3 &degs)
{
  m_axis_rotations = degs;
}

void model::draw_model()
{
  glm::mat4 model_mat           = calc_model_mat();
  glm::mat3 inv_trans_model_mat = glm::transpose(glm::inverse(model_mat));

  /* diffuse and specular values are sent through the texture */
  m_cur_shader->send_material_options(material_opt.ambient,
                                      material_opt.shine); // options
  m_cur_shader->send_normal_matrix(inv_trans_model_mat);   // for normal calc

  m_cur_shader->send_model_matrix(model_mat);         // for specular
  m_cur_shader->send_view_pos(camera->get_cam_pos()); // for specular

  for (const auto &lm : lightModels) {
    // light model details:
    m_cur_shader->send_light_options(lm->get_light_options().ambient,
                                     lm->get_light_options().diffuse,
                                     lm->get_light_options().specular);
    m_cur_shader->send_light_pos(lm->m_pos); // for diffusion and specular
  }

  /* any model has to send it's pvm */
  m_cur_shader->send_pvm(camera->get_PV() * model_mat);
  for (auto &mesh : meshes) {
    mesh.draw_mesh(*m_cur_shader);
  }
}

void model::draw_model(shader &s)
{
  if (s != *m_cur_shader)
    m_cur_shader = &s;
  draw_model();
}

void model::add_light_model(const lightModel &lm)
{
  lightModels.push_back(&lm);
}

void model::send_srp(srp_data &data)
{
  m_pos            = data.pos;
  m_axis_rotations = data.rotation;
  m_scale          = data.scale;
}

void amk::model::get_srp(glm::vec3 &pos, glm::vec3 &rot, glm::vec3 &scale) const
{
  pos   = m_pos;
  rot   = m_axis_rotations;
  scale = m_scale;
}

glm::mat4 model::calc_model_mat() const
{
  glm::mat4 mmat = glm::translate(DIAG, m_pos);

  mmat = glm::rotate(mmat, glm::radians(m_axis_rotations.x), XUNIT);
  mmat = glm::rotate(mmat, glm::radians(m_axis_rotations.y), YUNIT);
  mmat = glm::rotate(mmat, glm::radians(m_axis_rotations.z), ZUNIT);
  mmat = glm::scale(mmat, m_scale);

  return mmat;
}

void model::set_options()
{
  material_opt.ambient = glm::vec3(1.0f);
  material_opt.shine   = 32.0f;
}

/* ------------------------ light model ------------------------ */

void lightModel::set_options()
{
  light_opt.ambient  = glm::vec3(0.1f);
  light_opt.diffuse  = glm::vec3(0.7f);
  light_opt.specular = glm::vec3(0.5f);
}

lightModel::lightModel(cameraManager &camera, shader &s)
    : model(camera,
            s,
            mesh{
                {255, 255, 255}
})
{
  set_options();
}

// light model simply draws a solid bright color.
void lightModel::draw_model()
{
  glm::mat4 model_mat = calc_model_mat();
  /* any model has to send it's pvm */
  m_cur_shader->send_pvm(camera->get_PV() * model_mat);

  for (auto &mesh : meshes) {
    mesh.draw_mesh(*m_cur_shader);
  }
}

const lightModel::light_options &lightModel::get_light_options() const
{
  return light_opt;
}
