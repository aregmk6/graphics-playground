#ifndef CAMERA_H
#define CAMERA_H

#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace amk {

class cameraManager {
    static constexpr glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
    glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);

    GLfloat fov = 45.0f;
    GLfloat aspecty = 800.0f;
    GLfloat aspectx = 600.0f;
    GLfloat close = 0.1f;
    GLfloat far = 100.0f;
    GLfloat rotation_deg = 0.0f;
    GLfloat base_camera_speed = 7.0f;
    GLfloat sprint_multiplier = 3.0f;
    GLfloat lastx = 400;
    GLfloat lasty = 300;
    GLfloat pitch = 0.0f;
    GLfloat yaw = 0.0f;
    GLfloat sensativity = 0.1f;

    glm::mat4 mat_perspect =
        glm::perspective(fov, aspecty / aspectx, close, far);

    glm::mat4 mat_view = glm::lookAt(cameraPos, cameraPos + cameraFront, up);

  public:
    void update_cam_pos(GLFWwindow *const window, GLfloat dt);

    void update_cam_front(GLFWwindow *const window, double xpos, double ypos);

    glm::vec3 get_cam_pos() const;

    glm::mat4 get_PV();
};

} // namespace amk

#endif
