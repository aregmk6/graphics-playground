#ifndef MODEL_H
#define MODEL_H

#include "camera.h"
#include "glm/ext/vector_float3.hpp"
#include "mash.h"
#include "shaders.h"

namespace amk {

class model {
    shader *m_cur_shader;
    cameraManager *camera;
    std::vector<mash> mashes;
    glm::mat4 m_model;

    glm::vec3 m_scale;
    glm::vec3 m_axis_rotations;
    glm::vec3 m_pos;

    glm::mat4 calc_model_mat() const;

  public:
    enum axis { X, Y, Z };

    model(shader &s, cameraManager &camera);

    model(shader &s, cameraManager &camera, const mash &m);

    void add_mash(const mash &m);

    void add_mash(const std::vector<mash> &m);

    void set_model_pos(const glm::vec3 &pos);

    void set_model_scale(const glm::vec3 &scale);

    void set_model_rot(const GLfloat deg, model::axis a);

    void set_model_rot(const glm::vec3 &degs);

    void draw_model();

    void draw_model(shader &s);
};

}; // namespace amk

#endif
