#include "SoundMenuItem.h"

#include <glm/gtc/matrix_transform.hpp>
#include <filesystem>
#include <imgui.h>

#include <AhwassaGraphicsLib/sound/SoundEngine.h>
#include <AezeselFileIOLib/Sound/SoundFactory.h>

#include "Graphic.h"

SoundMenuItem::SoundMenuItem(std::string soundPath, Graphic& graphic) : _graphic(graphic) {
  _factory     = std::make_shared<Aezesel::SoundFactory>(soundPath);
  _soundEngine = std::make_shared<Ahwassa::SoundEngine>();
}

void SoundMenuItem::update() {

}

void SoundMenuItem::menu() {
  if (ImGui::TreeNode("Sounds")) {
    for (auto bank : _factory->getAllBanks()) {
      if (ImGui::TreeNode(bank.c_str())) {
        for (auto sound : _factory->getAllSoundsInBank(bank)) {
          if (ImGui::Button(sound.c_str())) {
            _currentHandler = _soundEngine->createHandler(*_factory->load(bank, sound));
            _currentHandler->play();
          }
        }
        ImGui::TreePop();
      }
    }
    ImGui::TreePop();
  }
}