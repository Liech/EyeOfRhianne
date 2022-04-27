#include "GamedataLoader.h"

#include <iostream>
#include <imgui.h>

#include <AthanahCommonLib/SupCom/Gamedata/Gamedata.h>

#include "EyeOfRhianneConfiguration.h"

GamedataLoader::GamedataLoader(EyeOfRhianneConfiguration& config) : _config(config) {

}

std::shared_ptr<Athanah::Gamedata> GamedataLoader::getGamedata() { 
  return _result; 
}

void GamedataLoader::start() {
  //if (_config.UseSCD) {
  //  std::cout << "Hint: " << std::endl;
  //  std::cout << "  Loading time can be speed up by extracting the SCFA gamedata content to the Data folder." << std::endl;
  //  std::cout << "  Gamedata Path: " << _config.SupComPath << "/gamedata" << std::endl;
  //  std::cout << "  Extract the SCD files (rename to .zip or use 7zip) as folders" << std::endl;
  //  std::cout << "  At the end e.g. '" << _config.AssetPath << "/" << "ambience' should exist" << std::endl;
  //  std::cout << "  Finally change 'UseSCD' in 'Configuration.json' to false" << std::endl;
  //}

  _loaderThread = std::async(std::launch::async, [this]() {
    _result = std::make_unique<Athanah::Gamedata>(_config.SupComPath, _config.UseSCD);
    _finished = true;
  });
}

void GamedataLoader::draw() {


}

void GamedataLoader::update() {
  
}

void GamedataLoader::menu() {
  ImGui::SetNextWindowPos(ImVec2(0, 0));
  ImGui::SetNextWindowSize(ImVec2(1920,1080));
  ImGui::Begin("Please Wait");
  ImGui::Text("Laden...");
  ImGui::End();
}