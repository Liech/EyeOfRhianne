#include "EyeOfRhianne.h"

#include <IyathuumCoreLib/Util/StateMachine.h>
#include <IyathuumCoreLib/Util/State.h>
#include <IyathuumCoreLib/Util/StateTransition.h>

#include <AhwassaGraphicsLib/Core/Window.h>
#include <AhwassaGraphicsLib/Core/Camera.h>
#include <AhwassaGraphicsLib/Input/Input.h>
#include <AhwassaGraphicsLib/Drawables/FPS.h>
#include <AhwassaGraphicsLib/Input/ArcBallCamera.h>

#include "AssetMainMenu/Graphic.h"
#include "AssetMainMenu/AssetMainMenu.h"
#include "AssetMainMenu/GraphicOptionsMenuItem.h"
#include "AssetMainMenu/SkyBoxMenuItem.h"
#include "AssetMainMenu/SoundMenuItem.h"
#include "AssetMainMenu/UnitMenuItem.h"

EyeOfRhianne::EyeOfRhianne(EyeOfRhianneConfiguration& config, Ahwassa::Window& w) : _config(config), _window(w) {
}

void EyeOfRhianne::setGamedata(std::shared_ptr<Athanah::Gamedata> gamedata) {
  _gamedata = gamedata;
}

void EyeOfRhianne::init() {
  _arcCam  = std::make_shared<Ahwassa::ArcBallCamera>(_window.camera(), _window.input());
  _graphic = std::make_shared<Graphic       >(&_window, _config.SupComPath + "\\sounds");
  _assets  = std::make_shared<AssetMainMenu >(_config, *_graphic, *_gamedata);
  _fps     = std::make_shared<Ahwassa::FPS  >(&_window);

  _window.camera()->setPosition(glm::vec3(20, 20, 20));
  _window.input().addUIElement(_arcCam.get(), 1);
}

void EyeOfRhianne::update() {
  _assets->update();
  _graphic->update();
}

void EyeOfRhianne::draw() {
  _graphic->draw();
  _assets->draw();

  if (_config.ShowFPS) 
    _fps->draw();
}

void EyeOfRhianne::menu() {
  _assets->menu();
}

void EyeOfRhianne::resize(const glm::ivec2& newResolution) {
  _arcCam->setLocalPosition(Iyathuum::glmAABB<2>(glm::vec2(0, 0), glm::vec2(newResolution[0], newResolution[1])));
  _graphic->setResolution(newResolution);
}
