#ifndef GUI_H
#define GUI_H

#include "camera.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "modelManager.h"

#include <GLFW/glfw3.h>

namespace amk
{

class guiManager
{
public:
  guiManager(GLFWwindow *window,
             cameraManager &camera,
             modelManager &model_manager);
  void shutdown() const;

  void draw();

  void test_function() const;

  static constexpr GLfloat R = 61.0f / 255.0f, G = 190.0f / 255.0f, B = 1.0f;

  ImGuiContext *context;
  cameraManager *camera;
  modelManager *model_manager;
  ImGuiIO &io;
  GLfloat *fov;
  GLuint increment;
  ImVec4 clear_color = ImVec4(R, G, B, 1.00f);

  bool show_demo_window    = false;
  bool show_another_window = false;

private:
  void start_frame() const;
  void end_frame() const;
  void model_movement();

  // ------- helpers -------

  ImGuiContext *InitContext();

  bool first = true;
};

}; // namespace amk

#endif
