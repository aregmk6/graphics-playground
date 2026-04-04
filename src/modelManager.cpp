#include "modelManager.h"

using namespace amk;

modelManager &modelManager::get_instance(cameraManager &camera)
{
  static modelManager instance(camera);
  return instance;
}

modelManager::modelManager(cameraManager &camera) : mCamera(&camera)
{
  using shaderTypes = shader::shaderTypes;

  mShaders.emplace(shaderTypes::regular,
                   shader("./shaders/shader.vert", "./shaders/shader.frag"));
  mShaders.emplace(shaderTypes::emissive,
                   lightShader("./shaders/light.vert", "./shaders/light.frag"));

  auto light = std::make_unique<amk::lightModel>(
      camera, mShaders.at(shaderTypes::emissive));
  auto material1 =
      std::make_unique<amk::model>(camera,
                                   mShaders.at(shaderTypes::regular),
                                   amk::mesh{
                                       {255, 255, 0}
  });
  auto material2 =
      std::make_unique<amk::model>(camera,
                                   mShaders.at(shaderTypes::regular),
                                   amk::mesh{
                                       {255, 0, 0}
  });
  auto material3 =
      std::make_unique<amk::model>(camera,
                                   mShaders.at(shaderTypes::regular),
                                   amk::mesh{
                                       {0, 255, 0}
  });
  auto material4 =
      std::make_unique<amk::model>(camera,
                                   mShaders.at(shaderTypes::regular),
                                   amk::mesh{
                                       {0, 0, 255}
  });

  material1->add_light_model(*light);
  material2->add_light_model(*light);
  material3->add_light_model(*light);
  material4->add_light_model(*light);

  mModels.push_back(std::move(light));
  mModels.push_back(std::move(material1));
  mModels.push_back(std::move(material2));
  mModels.push_back(std::move(material3));
  mModels.push_back(std::move(material4));

  mModels[0]->set_model_scale({0.5f, 0.5f, 0.5f});
}

void amk::modelManager::set_cur_model_rot_ptr(int model)
{
  cur_model_rot = &mModels[model]->m_axis_rotations;
}

std::vector<GLuint> modelManager::get_shader_ids() const
{
  std::vector<GLuint> ret;
  for (const auto &shader : mShaders) {
    ret.push_back(shader.second.get_id());
  }
  return ret;
}
void amk::modelManager::draw()
{
  for (auto &model : mModels) {
    model->draw_model();
  }
}
