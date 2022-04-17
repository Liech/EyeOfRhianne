#include "AnimationSelection.h"

#include <glm/gtc/matrix_transform.hpp>
#include <imgui.h>
#include <AthanahCommonLib/SupCom/SupComModel.h>
#include <AezeselFileIOLib/STLWriter.h>
#include <AhwassaGraphicsLib/Core/Window.h>
#include <AhwassaGraphicsLib/Widgets/Button.h>

#include "Graphic.h"

AnimationSelection::AnimationSelection(Graphic& graphic) : _graphic(graphic){
}

void AnimationSelection::update() {
  
  if (_play)
    _time = std::fmod(_time + 0.01f, 1);

  if (_graphic._model != _model) {
    _time = 0;
    _model = _graphic._model;
    _currentAnimation = "None";
  }
  
  if (_graphic._mesh)
    _graphic._mesh->animation = getAnimation();
}

void AnimationSelection::menu() {
  auto animations = _graphic._model->availableAnimations();
  if (animations.size() != 0) {
    if (ImGui::BeginPopupContextItem("Chose Animation")) {
      for (auto& anim : _graphic._model->availableAnimations()) {
        if (ImGui::Button(anim.c_str())) {
          _currentAnimation = anim;
          _time = 0;
        }
      }
      ImGui::EndMenu();
    }
    if (ImGui::Button("Animate"))
      ImGui::OpenPopup("Chose Animation");
    if (_currentAnimation != "None") {
      ImGui::SameLine();
      if (_play && ImGui::Button("Pause")) {
        _play = !_play;
      } else
        if (!_play && ImGui::Button("Play")) {
          _play = !_play;
        }
    }
  }
  if (ImGui::Button("Export")) {
    save();
  }
}

std::vector<glm::mat4> AnimationSelection::getAnimation() {
  if (_currentAnimation == "None" || !_model)
    return std::vector<glm::mat4>();
  return _model->getAnimation(_currentAnimation, _model->getAnimationLength(_currentAnimation) * _time);
}

void AnimationSelection::save() {
  std::vector<glm::vec3> data;
  data.resize(_model->scm().indices.size() * 3);
  auto anim = getAnimation();
  for (size_t i = 0; i < _model->scm().indices.size(); i++) {
    const auto& v1 = _model->scm().vertecies[_model->scm().indices[i].a];
    const auto& v2 = _model->scm().vertecies[_model->scm().indices[i].b];
    const auto& v3 = _model->scm().vertecies[_model->scm().indices[i].c];
    auto mat1 = glm::mat4(1.0);
    auto mat2 = glm::mat4(1.0);
    auto mat3 = glm::mat4(1.0);

    if (_currentAnimation != "None") {
      mat1 = anim[v1.boneIndex[0]];
      mat2 = anim[v2.boneIndex[0]];
      mat3 = anim[v3.boneIndex[0]];
    }

    data[i * 3 + 0] = mat1 * glm::vec4(v1.position, 1);
    data[i * 3 + 1] = mat2 * glm::vec4(v2.position, 1);
    data[i * 3 + 2] = mat3 * glm::vec4(v3.position, 1);
  }
  Aezesel::STLWriter::write("Output.stl", data);
}