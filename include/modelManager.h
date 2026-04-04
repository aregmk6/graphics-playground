#ifndef MODELMANAGER_H
#define MODELMANAGER_H

#include "camera.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "model.h"
#include "shaders.h"

#include <GLFW/glfw3.h>
#include <memory>
#include <unordered_map>
#include <utility>
#include <vector>

namespace amk
{

class modelManager
{
public:
  std::vector<std::unique_ptr<amk::model>> mModels;
  glm::vec3 *cur_model_rot = nullptr;

  static modelManager &get_instance(cameraManager &camera);
  std::vector<GLuint> get_shader_ids() const;
  model *const get_model_ptr() const;
  void draw();
  void set_cur_model_rot_ptr(int model);

private:
  modelManager(cameraManager &camera);

  cameraManager *mCamera;
  std::unordered_map<shader::shaderTypes, shader> mShaders;
};

}; // namespace amk

#endif
