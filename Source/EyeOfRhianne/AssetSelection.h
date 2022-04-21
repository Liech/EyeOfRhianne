#pragma once

#include <string>
#include <memory>
#include "IyathuumCoreLib/BaseTypes/glmAABB.h"
#include "Graphic.h"
#include "EyeOfRhianneConfiguration.h"

namespace Ahwassa {
  class Window;
}

namespace Athanah {
  class Gamedata;
}

class UnitMenuItem;
class SkyBoxMenuItem;
class ListSelection;
class RendererSelection;
class MapSelection;
class ScriptSelection;
class SoundMenuItem;
class Graphic;

class AssetSelection {
public:
  AssetSelection(EyeOfRhianneConfiguration&, Graphic&);
  virtual ~AssetSelection() = default;

  void menu();
  void update();
  Athanah::Gamedata& gamedata();
private:
  void addDialogs();

  std::string                         _current;
  std::unique_ptr<ListSelection>      _list;
  Graphic&                            _graphic;
  std::unique_ptr<Athanah::Gamedata>  _gamedata;

  std::shared_ptr<RendererSelection  > _renderer  ;
  std::shared_ptr<MapSelection       > _maps      ;
  std::shared_ptr<ScriptSelection    > _scripts   ;

  std::unique_ptr<UnitMenuItem >    _units;
  std::unique_ptr<SoundMenuItem>         _sounds;
  std::unique_ptr<SkyBoxMenuItem    >    _skyBox;

  EyeOfRhianneConfiguration& _config;
};