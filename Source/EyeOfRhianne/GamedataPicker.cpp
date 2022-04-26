#include "GamedataPicker.h"

#include <imgui.h>

#include "EyeOfRhianneConfiguration.h"

GamedataPicker::GamedataPicker(EyeOfRhianneConfiguration& config) : _config(config) {

}

void GamedataPicker::menu() {
  ImGui::Begin("Find Supreme Commander");
  ImGui::Text("Hello World!");
  if (ImGui::Button("Proceed")) {
    _finished = true;
  }
  ImGui::End();
}

bool GamedataPicker::finished() const {
  return _finished;
}