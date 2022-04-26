#include "GamedataPicker.h"

#include <imgui.h>
#include <imgui_stdlib.h>
#include <iostream>
#include <filesystem>

#include "EyeOfRhianneConfiguration.h"

GamedataPicker::GamedataPicker(EyeOfRhianneConfiguration& config) : _config(config) {

  if (_config.UseSCD) {
    std::cout << "Hint: " << std::endl;
    std::cout << "  Loading time can be speed up by extracting the SCFA gamedata content to the Data folder." << std::endl;
    std::cout << "  Gamedata Path: " << _config.SupComPath << "/gamedata" << std::endl;
    std::cout << "  Extract the SCD files (rename to .zip or use 7zip) as folders" << std::endl;
    std::cout << "  At the end e.g. '" << _config.AssetPath << "/" << "ambience' should exist" << std::endl;
    std::cout << "  Finally change 'UseSCD' in 'Configuration.json' to false" << std::endl;
  }

}

void GamedataPicker::menu() {
  ImGui::SetNextWindowPos(ImVec2(0, 0));
  ImGui::Begin("Find Supreme Commander");
  ImGui::Text("Please specify the path to your Forged Alliance installation:");
  ImGui::InputText("Path"        , &_config.SupComPath);

  ImGui::Checkbox("Use SCD Files", &_config.UseSCD);

  if (ImGui::Button("Proceed")) {
    _finished = true;
    _config.toJsonFile("Configuration.json");
  }

  ImGui::End();
}

bool GamedataPicker::finished() const {
  return _finished;
}