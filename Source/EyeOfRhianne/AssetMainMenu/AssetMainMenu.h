#pragma once

#include <string>
#include <memory>
#include "IyathuumCoreLib/BaseTypes/glmAABB.h"
#include "Graphic.h"
#include "EyeOfRhianne/EyeOfRhianneConfiguration.h"
#include "EyeOfRhianne/EyeState.h"

namespace Ahwassa {
  class Window;
}

namespace Athanah {
  class Gamedata;
}

class UnitMenuItem;
class SkyBoxMenuItem;
class ListSelection;
class MapSelection;
class ScriptSelection;
class SoundMenuItem;
class Graphic;
class GraphicOptionsMenuItem;

class AssetMainMenu {
public:
  AssetMainMenu(EyeOfRhianneConfiguration&, Graphic&, Athanah::Gamedata&);
  virtual ~AssetMainMenu() = default;

  void menu();
  void update();
  void draw();

  Athanah::Gamedata& gamedata();
private:
  void addDialogs();
  void optionsMenu();

  std::string                         _current;
  Graphic&                            _graphic;
  Athanah::Gamedata&                  _gamedata;

  std::shared_ptr<MapSelection       > _maps      ;
  std::shared_ptr<ScriptSelection    > _scripts   ;

  std::unique_ptr<UnitMenuItem >          _units;
  std::unique_ptr<SoundMenuItem>          _sounds;
  std::unique_ptr<SkyBoxMenuItem>         _skyBox;
  std::unique_ptr<GraphicOptionsMenuItem> _graphicOptions;


  bool _showImguiDemo = false;

  EyeOfRhianneConfiguration& _config;
};