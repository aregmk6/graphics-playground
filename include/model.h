#ifndef MODEL_H
#define MODEL_H

#include "camera.h"
#include "glm/ext/vector_float3.hpp"
#include "mesh.h"
#include "shaders.h"

namespace amk {

class model {
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

    void draw_model();

    void draw_model(shader &s);
};

}; // namespace amk

#endif
