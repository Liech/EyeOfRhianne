#pragma once

#include <memory>
#include <glm/glm.hpp>

class EyeOfRhianneConfiguration;

namespace Iyathuum {
  class StateMachine;
}

namespace Athanah {
  class Gamedata;
}

namespace Ahwassa {
  class Window;
  class FPS;
  class ArcBallCamera;
}

class Graphic;
class AssetMainMenu;
class Gamedata;

class EyeOfRhianne {
  public:
    EyeOfRhianne(EyeOfRhianneConfiguration&, Ahwassa::Window&);
    virtual ~EyeOfRhianne() = default;

    void init();
    void setGamedata(std::shared_ptr<Athanah::Gamedata>);

    void update();
    void draw();
    void menu();
    void resize(const glm::ivec2& newResolution);

  private:
    EyeOfRhianneConfiguration& _config;
    Ahwassa::Window&           _window;

    std::shared_ptr<Athanah::Gamedata>      _gamedata;
    std::shared_ptr<Ahwassa::FPS>           _fps;
    std::shared_ptr<AssetMainMenu>          _assets;
    std::shared_ptr<Graphic>                _graphic;
    std::shared_ptr<Ahwassa::ArcBallCamera> _arcCam;
};