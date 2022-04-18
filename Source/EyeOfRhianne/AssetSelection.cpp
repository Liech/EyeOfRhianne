#include "AssetSelection.h"

#include "UnitModelSelection.h"
#include "SkyBoxSelection.h"
#include "RendererSelection.h"
#include "ListSelection.h"
#include "MapSelection.h"
#include "SoundSelection.h"
#include "ScriptSelection.h"
#include "Graphic.h"
#include "AnimationControlDialog.h"

#include <glm/gtc/matrix_transform.hpp>
#include "IyathuumCoreLib/BaseTypes/glmAABB.h"
#include "AthanahCommonLib/SupCom/Gamedata/Gamedata.h"

#include "AhwassaGraphicsLib/Core/Window.h"
#include "AhwassaGraphicsLib/Widgets/ListLayout.h"

#include <imgui.h>

AssetSelection::AssetSelection(EyeOfRhianneConfiguration& config, Graphic& graphic) : _graphic(graphic), _config(config){

  if (config.UseSCD)
  {
      std::cout << "Hint: " << std::endl;
      std::cout << "  Loading time can be speed up by extracting the SCFA gamedata content to the Data folder." << std::endl;
      std::cout << "  Gamedata Path: " << config.SupComPath <<"/gamedata" << std::endl;
      std::cout << "  Extract the SCD files (rename to .zip or use 7zip) as folders" << std::endl;
      std::cout << "  At the end e.g. '" << config.AssetPath << "/" << "ambience' should exist" << std::endl;
      std::cout << "  Finally change 'UseSCD' in 'Configuration.json' to false" << std::endl;
  }
  _gamedata = std::make_unique<Athanah::Gamedata>(config.SupComPath,config.UseSCD);

  addDialogs();
}  

void AssetSelection::addDialogs() {

  _units            = std::make_unique<UnitModelSelection>    (*_gamedata, _graphic);
  _animationControl = std::make_unique<AnimationControlDialog>(_graphic);
  //_skyBox           = std::make_unique<SkyBoxSelection>       (_gamedata->skybox(), _graphic);
  //_renderer   = std::make_shared<RendererSelection>  (area,_graphic);
  //_maps       = std::make_shared<MapSelection>       (_config.SupComPath+ "\\maps",area,_graphic, *_gamedata);
  //_scripts    = std::make_shared<ScriptSelection>    (area,_graphic);
  _sounds     = std::make_unique<SoundSelection >    (_config.SupComPath + "\\sounds",_graphic);
}

void AssetSelection::menu() {
  ImGui::Begin("Assets");
  _units->menu();
  //_skyBox->menu();
  _sounds->menu();  
  ImGui::End();

  if (_graphic._currentAnimation != "None") {
    ImGui::Begin("Animation Control");
    _animationControl->menu();
    ImGui::End();
  }

  //_skyBox    ->draw();
  //_renderer  ->draw();
  //_maps      ->draw();
  //_scripts   ->draw();
  //_list      ->draw();
}

void AssetSelection::update() {
  _units ->update();
  _sounds->update();
  //_skyBox->update();
  //_renderer  ->update();
  //_maps      ->update();
  //_scripts   ->update();
  //_sounds    ->update();
}
