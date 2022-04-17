#pragma once

#include <glm/glm.hpp>
#include <string>
#include <memory>
#include <vector>

class Graphic;

namespace Ahwassa {
  class Button;
  class Window;
}

namespace Athanah {
  class SupComModel;
}

class AnimationSelection {
public:
  AnimationSelection(Graphic&);
  virtual ~AnimationSelection() = default;

  void update();
  void menu();

private:
  void save();
  std::vector<glm::mat4> getAnimation();

  std::string                           _currentAnimation = "None";
  bool                                  _play = true;
  float                                 _time = 0;
  Graphic&                              _graphic;
  std::shared_ptr<Athanah::SupComModel> _model;
  std::shared_ptr<Ahwassa::Button>      _pause;
  std::shared_ptr<Ahwassa::Button>      _save = nullptr;
};