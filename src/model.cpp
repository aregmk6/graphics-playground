#include "model.h"

static constexpr glm::mat4 DIAG(1.0f);
static constexpr glm::vec3 XUNIT(1.0f, 0.0f, 0.0f);
static constexpr glm::vec3 YUNIT(0.0f, 1.0f, 0.0f);
static constexpr glm::vec3 ZUNIT(0.0f, 0.0f, 1.0f);

using namespace amk;

model::model(cameraManager &camera, shader &s) //
    : m_cur_shader(&s),                        //
      camera(&camera),                         //
      meshes{mesh{}},                          //
      m_model(1.0f),                           //
      m_scale(1.0f),                           //
      m_axis_rotations(0.0f),                  //
      m_pos(0.0f) {}

model::model(cameraManager &camera, shader &s, const mesh &m) //
    : m_cur_shader(&s),                                       //
      camera(&camera),                                        //
      m_model(1.0f),                                          //
      m_scale(1.0f),                                          //
      m_axis_rotations(0.0f),                                 //
      m_pos(0.0f) {
    add_mesh(m);
}

model::model(cameraManager &camera, shader &s, const std::vector<mesh> &m_v)
    : m_cur_shader(&s),       //
      camera(&camera),        //
      m_model(1.0f),          //
      m_scale(1.0f),          //
      m_axis_rotations(0.0f), //
      m_pos(0.0f) {
    add_mesh(m_v);
}

void model::add_mesh(const mesh &m) {
    meshes.push_back(m);
}

void model::add_mesh(const std::vector<mesh> &m) {
    meshes.insert(meshes.end(), m.begin(), m.end());
}

void model::set_model_pos(const glm::vec3 &pos) {
    m_pos = pos;
}

void model::set_model_scale(const glm::vec3 &scale) {
    m_scale = scale;
}

void model::set_model_rot(const GLfloat deg, model::axis a) {
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

void model::set_model_rot(const glm::vec3 &degs) {
    m_axis_rotations = degs;
}

void model::draw_model() {
    m_cur_shader->send_PVM(camera->get_PV() * calc_model_mat());
    for (auto &mesh : meshes) {
        mesh.draw_mesh(*m_cur_shader);
    }
}

void model::draw_model(shader &s) {
    if (s != *m_cur_shader) m_cur_shader = &s;
    draw_model();
}

glm::mat4 model::calc_model_mat() const {
    glm::mat4 ret = glm::translate(DIAG, m_pos);
    ret = glm::rotate(ret, glm::radians(m_axis_rotations.x), XUNIT);
    ret = glm::rotate(ret, glm::radians(m_axis_rotations.y), YUNIT);
    ret = glm::rotate(ret, glm::radians(m_axis_rotations.z), ZUNIT);
    ret = glm::scale(ret, m_scale);
    return ret;
}
