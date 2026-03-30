#include "model.h"

static constexpr glm::mat4 DIAG(1.0f);
static constexpr glm::vec3 XUNIT(1.0f, 0.0f, 0.0f);
static constexpr glm::vec3 YUNIT(0.0f, 1.0f, 0.0f);
static constexpr glm::vec3 ZUNIT(0.0f, 0.0f, 1.0f);

using namespace amk;

model::model(shader &s, cameraManager &camera) //
    : m_model(1.0f), m_pos(0.0f), m_axis_rotations(0.0f) {}

model::model(shader &s, cameraManager &camera, const mash &m) //
    : m_model(1.0f), m_pos(0.0f), m_axis_rotations(0.0f) {
    add_mash(m);
}

void model::add_mash(const mash &m) {
    mashes.push_back(m);
}

void model::add_mash(const std::vector<mash> &m) {
    mashes.insert(mashes.end(), m.begin(), m.end());
}

void model::set_model_pos(const glm::vec3 &pos) {
    m_pos = pos;
}

void model::set_model_scale(const glm::vec3 &scale) {
    m_scale = scale;
}

void model::set_model_rot(const GLfloat deg, model::axis a) {
    switch (a) {
    case X:
        m_axis_rotations.x = deg;
    case Y:
        m_axis_rotations.y = deg;
    case Z:
        m_axis_rotations.z = deg;
    }
}

void model::set_model_rot(const glm::vec3 &degs) {
    m_axis_rotations = degs;
}

void model::draw_model() {
    m_cur_shader->send_PVM(camera->get_PV() * calc_model_mat());
    for (auto &mash : mashes) {
        mash.draw_mash(*m_cur_shader);
    }
}

void model::draw_model(shader &s) {
    if (s != *m_cur_shader) m_cur_shader = &s;
    draw_model();
}

glm::mat4 model::calc_model_mat() const {
    glm::mat4 ret = glm::scale(DIAG, m_scale);
    ret = glm::rotate(ret, glm::radians(m_axis_rotations.x), XUNIT);
    ret = glm::rotate(ret, glm::radians(m_axis_rotations.y), YUNIT);
    ret = glm::rotate(ret, glm::radians(m_axis_rotations.z), ZUNIT);
    ret = glm::translate(ret, m_pos);
    return ret;
}
