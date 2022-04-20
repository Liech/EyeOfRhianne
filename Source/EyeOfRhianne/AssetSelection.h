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

class UnitModelSelection;
class SkyBoxSelection;
class ListSelection;
class RendererSelection;
class MapSelection;
class ScriptSelection;
class SoundSelection;
class Graphic;
class AnimationControlDialog;

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

  std::unique_ptr<UnitModelSelection >    _units;
  std::unique_ptr<AnimationControlDialog> _animationControl;
  std::unique_ptr<SoundSelection>         _sounds;
  std::unique_ptr<SkyBoxSelection    >    _skyBox;

  EyeOfRhianneConfiguration& _config;
};