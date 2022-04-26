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
  SoundMenuItem(Graphic&);

  void update();
  void menu();
private:
                                      
  Graphic&                               _graphic;
};