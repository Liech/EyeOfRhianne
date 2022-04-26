#include "SoundMenuItem.h"

#include <glm/gtc/matrix_transform.hpp>
#include <filesystem>
#include <imgui.h>

#include <AhwassaGraphicsLib/sound/SoundEngine.h>
#include <AhwassaGraphicsLib/sound/SoundHandler.h>
#include <AezeselFileIOLib/Sound/SoundFactory.h>

#include "Graphic.h"

SoundMenuItem::SoundMenuItem(Graphic& graphic) : _graphic(graphic) {
}

void SoundMenuItem::update() {

}

void SoundMenuItem::menu() {
  if (ImGui::TreeNode("Sounds")) {
    for (auto bank : _graphic._soundFactory->getAllBanks()) {
      if (ImGui::TreeNode(bank.c_str())) {
        for (auto sound : _graphic._soundFactory->getAllSoundsInBank(bank)) {
          if (ImGui::Button(sound.c_str())) {
            _graphic._currentSoundHandler = _graphic._soundEngine->createHandler(*_graphic._soundFactory->load(bank, sound));
            _graphic._currentSoundHandler->play();
          }
        }
        ImGui::TreePop();
      }
    }
    ImGui::TreePop();
  }
}