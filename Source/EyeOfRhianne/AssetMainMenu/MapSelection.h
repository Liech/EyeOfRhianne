#pragma once

#include <glm/glm.hpp>
#include <string>
#include <memory>
#include <map>
#include <vector>
#include "Graphic.h"

namespace Ahwassa {
  class Window;
}
namespace Aezesel {
  class SCMAP;
}
namespace Athanah {
  class SupComModel;
  class Scenario;
  class MapFactory;
  class Gamedata;
}

class MapSelection {
public:
  MapSelection(std::string mapPath, Graphic&, Athanah::Gamedata&);

  void update();
  void menu();

private:
  void setMap(std::shared_ptr<Athanah::Map>);

  Athanah::Gamedata&                   _gamedata;
  std::shared_ptr<Athanah::MapFactory> _factory ;
  Graphic&                             _graphic ;
};