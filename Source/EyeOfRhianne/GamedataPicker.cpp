#include "GamedataPicker.h"

#include <imgui.h>
#include <imgui_stdlib.h>
#include <iostream>
#include <filesystem>

#include "EyeOfRhianneConfiguration.h"

GamedataPicker::GamedataPicker(EyeOfRhianneConfiguration& config) : _config(config) {
}

void GamedataPicker::start() {
  if (std::filesystem::exists(_config.SupComPath + "/gamedata/units.scd"))
    _finished = true;
}

void GamedataPicker::menu() {
  ImGui::SetNextWindowPos(ImVec2(0, 0));
  ImGui::SetNextWindowSize(ImVec2(1920, 1080));
  ImGui::Begin("Find Supreme Commander");
  ImGui::Text("Please specify the path to your Forged Alliance installation:");
  ImGui::InputText("Path"        , &_config.SupComPath);
  ImGui::Text("Example: Steam\\steamapps\\common\\Supreme Commander Forged Alliance");

  if (std::filesystem::exists(_config.SupComPath + "/gamedata/units.scd") && ImGui::Button("Proceed")) {
    _finished = true;
    _config.toJsonFile("Configuration.json");
  }

  ImGui::End();
}

bool GamedataPicker::finished() const {
  return _finished;
}