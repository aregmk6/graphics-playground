#include "camera.h"

#include "utilities.h"

using namespace amk;

void cameraManager::update_cam_pos(GLFWwindow *const window, GLfloat dt)
{
  GLfloat camera_speed;
  if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
    camera_speed = base_camera_speed * sprint_multiplier;
  } else {
    camera_speed = base_camera_speed;
  }
  GLfloat dx = dt * camera_speed;

  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
    cameraPos += dx * cameraFront;
  }
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
    cameraPos -= dx * cameraFront;
  }
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
    cameraPos += dx * glm::normalize(glm::cross(cameraFront, up));
  }
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
    cameraPos -= dx * glm::normalize(glm::cross(cameraFront, up));
  }
}

void cameraManager::update_cam_front(GLFWwindow *const window,
                                     double xpos,
                                     double ypos)
{
  GLfloat offsetx = xpos - lastx;
  GLfloat offsety = lasty - ypos;
  lastx           = xpos;
  lasty           = ypos;

  yaw += offsetx * sensativity;
  pitch += offsety * sensativity;

  if (pitch > pitch_max_limit)
    pitch = pitch_max_limit;
  if (pitch < pitch_min_limit)
    pitch = pitch_min_limit;

  cameraFront = glm::normalize(
      glm::vec3(cos(glm::radians(yaw)) * cos(glm::radians(pitch)),
                sin(glm::radians(pitch)),
                sin(glm::radians(yaw)) * cos(glm::radians(pitch))));
}

glm::mat4 cameraManager::get_PV()
{
  mat_view     = glm::lookAt(cameraPos, cameraPos + cameraFront, up);
  mat_perspect = glm::perspective(fov, aspecty / aspectx, close, far);
  return mat_perspect * mat_view;
}

glm::vec3 cameraManager::get_cam_pos() const
{
  return cameraPos;
}

GLfloat *const cameraManager::get_fov_ptr()
{
  return &fov;
}

void cameraManager::set_last_coords(GLfloat x, GLfloat y)
{
  lastx = x;
  lasty = y;
}
