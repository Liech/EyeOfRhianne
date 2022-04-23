#include "AssetMainMenu.h"

#include "UnitMenuItem.h"
#include "SkyBoxMenuItem.h"
#include "ListSelection.h"
#include "MapSelection.h"
#include "SoundMenuItem.h"
#include "ScriptSelection.h"
#include "Graphic.h"
#include "GraphicOptionsMenuItem.h"

#include <glm/gtc/matrix_transform.hpp>
#include "IyathuumCoreLib/BaseTypes/glmAABB.h"
#include "AthanahCommonLib/SupCom/Gamedata/Gamedata.h"

#include "AhwassaGraphicsLib/Core/Window.h"
#include "AhwassaGraphicsLib/Widgets/ListLayout.h"

#include <imgui.h>

AssetMainMenu::AssetMainMenu(EyeOfRhianneConfiguration& config, Graphic& graphic) : _graphic(graphic), _config(config){

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

void AssetMainMenu::addDialogs() {

  _units            = std::make_unique<UnitMenuItem>    (*_gamedata, _graphic);
  _skyBox           = std::make_unique<SkyBoxMenuItem>  (_gamedata->skybox(), _graphic);
  _graphicOptions   = std::make_unique<GraphicOptionsMenuItem>(_graphic);
  _maps             = std::make_shared<MapSelection>       (_config.SupComPath+ "\\maps",_graphic, *_gamedata);
  //_scripts    = std::make_shared<ScriptSelection>    (area,_graphic);
  _sounds     = std::make_unique<SoundMenuItem >    (_config.SupComPath + "\\sounds",_graphic);
}

void AssetMainMenu::menu() {
  ImGui::SetNextWindowPos(ImVec2(0, 0));
  ImGui::SetNextItemWidth(ImGui::GetFontSize() * 50);
  ImGui::Begin("Assets",0, ImGuiWindowFlags_NoMove );
  _units->menu();
  _skyBox->menu();
  _maps->menu();
  _sounds->menu();  
  optionsMenu();
  if (_showImguiDemo)
    ImGui::ShowDemoWindow();
  ImGui::End();

  //_maps      ->draw();
  //_scripts   ->draw();
}

void AssetMainMenu::optionsMenu() {
  if (ImGui::TreeNode("Options")) {
    _graphicOptions->menu();
    ImGui::Checkbox("Show IMGUI Demo Window", &_showImguiDemo);
    ImGui::TreePop();
  }
}

void AssetMainMenu::update() {
  _units ->update();
  _sounds->update();
  _skyBox->update();
  _maps  ->update();
  //_scripts   ->update();
}

Athanah::Gamedata& AssetMainMenu::gamedata() {
  return *_gamedata;
}