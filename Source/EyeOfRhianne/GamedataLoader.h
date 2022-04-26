#pragma once

#include <memory>
#include <future>

namespace Athanah {
  class Gamedata;
}

class EyeOfRhianneConfiguration;

class GamedataLoader {
  public:
    GamedataLoader(EyeOfRhianneConfiguration& config);

    void start();
    void draw();
    void update();
    void menu();

    bool finished() { return _finished; }
    std::shared_ptr<Athanah::Gamedata> getGamedata();

  private:
    EyeOfRhianneConfiguration& _config;

    bool _finished = false;

    std::shared_ptr<Athanah::Gamedata> _result;
    std::future<void>                  _loaderThread;
};