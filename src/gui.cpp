#include "gui.h"

#include "camera.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "modelManager.h"

#include <iostream>
#include <unistd.h>

using namespace amk;

void guiManager::draw()
{
  start_frame();
  // build main window
  {
    ImGui::Begin("Controller");

    ImGui::ColorEdit3("clear color", (float *)&clear_color);

    {
      static int chosen_model = 0;
      static model::srp_data srp;
      if (!model_manager->mModels.empty()) {
        ImGui::InputInt("Choose Model", &chosen_model);
        size_t nr_models = model_manager->mModels.size();

        if (chosen_model < 0)
          chosen_model = 0;
        else if (chosen_model > nr_models - 1)
          chosen_model = nr_models - 1;

        model_manager->set_cur_model_rot_ptr(chosen_model);
        model_manager->mModels[chosen_model]->get_srp(
            srp.pos, srp.rotation, srp.scale);

        ImGui::InputFloat3("model position", (GLfloat *)&srp.pos);
        ImGui::SliderFloat3(
            "model position silder", (GLfloat *)&srp.pos, -10.0f, 10.0f);

        model_manager->mModels[chosen_model]->send_srp(srp);
      }
    }

    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
                1000.0f / io.Framerate,
                io.Framerate);
    ImGui::End();
  }

  end_frame();
}

void guiManager::start_frame() const
{
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
}

void guiManager::end_frame() const
{
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

guiManager::guiManager(GLFWwindow *window,
                       cameraManager &camera,
                       modelManager &model_manager)
    : context(InitContext()), io(ImGui::GetIO()), camera(&camera),
      fov(&camera.fov), model_manager(&model_manager)
{
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init();
}

void guiManager::shutdown() const
{
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
}

ImGuiContext *guiManager::InitContext()
{
  IMGUI_CHECKVERSION();
  return ImGui::CreateContext();
}
