#pragma once

#include <glm/glm.hpp>
#include <string>
#include <memory>
#include <map>
#include <vector>
#include "Graphic.h"


namespace Ahwassa {
  class Window;
  class SoundEngine;
  class SoundHandler;
}

namespace Aezesel {
  class SoundFactory;
}

class SoundMenuItem {
public:
  SoundMenuItem(std::string soundPath, Graphic&);

  void update();
  void menu();
private:
                                      
  Graphic&                               _graphic;
  std::shared_ptr<Aezesel::SoundFactory> _factory;
  std::shared_ptr<Ahwassa::SoundEngine > _soundEngine;
  std::shared_ptr<Ahwassa::SoundHandler> _currentHandler = nullptr;
};