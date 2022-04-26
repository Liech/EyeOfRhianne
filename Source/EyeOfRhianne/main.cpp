#include <iostream>
#include <filesystem>
#include <functional>
#include <imgui.h>

#include <IyathuumCoreLib/Singleton/Database.h>
#include <IyathuumCoreLib/Util/StateMachine.h>
#include <IyathuumCoreLib/Util/State.h>
#include <IyathuumCoreLib/Util/StateTransition.h> 
#include <AhwassaGraphicsLib/Core/Window.h>
#include <AhwassaGraphicsLib/Input/IMGUIRenderer.h>
#include <AthanahCommonLib/SupCom/Gamedata/Gamedata.h>

#include "GamedataPicker.h"
#include "EyeOfRhianneConfiguration.h"
#include "EyeOfRhianne.h"
#include "GamedataLoader.h"

void enforceWorkingDir(std::string exeDir) {
  const size_t last_slash_idx = exeDir.find_last_of("\\/");
  if (std::string::npos != last_slash_idx)
  {
    exeDir.erase(last_slash_idx + 1);
  }
  std::filesystem::current_path(exeDir);
}

int main(int argc, char** argv) {
  enforceWorkingDir(std::string(argv[0]));

  EyeOfRhianneConfiguration config;
  try
  {
      config.fromJsonFile("Configuration.json");
  }
  catch (...)
  {
      config = EyeOfRhianneConfiguration();
      config.toJsonFile("Configuration.json");
  }

  int width  = config.ScreenWidth;
  int height = config.ScreenHeight;

  Ahwassa::Window w(glm::ivec2(width,height));
  GamedataLoader loader(config);
  EyeOfRhianne   eye(config, w);
  GamedataPicker picker(config);

  // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

  std::shared_ptr<Iyathuum::State> startState = std::make_shared<Iyathuum::State>("Start");
  Iyathuum::StateMachine state(startState);
  std::map<std::shared_ptr<Iyathuum::State>, std::function<void()>> _updateMap;
  std::map<std::shared_ptr<Iyathuum::State>, std::function<void()>> _drawMap;
  std::map<std::shared_ptr<Iyathuum::State>, std::function<void()>> _menuMap;
  _updateMap[startState] = [&]() { };
  _drawMap  [startState] = [&]() { };
  _menuMap  [startState] = [&]() { };

  std::shared_ptr<Iyathuum::State> pickerState = std::make_shared<Iyathuum::State>("GamedataPicker");
  _updateMap[pickerState] = [&]() { };
  _drawMap[pickerState] = [&]()   { };
  _menuMap[pickerState] = [&]()   { picker.menu();   };
  state.addState(pickerState);

  std::shared_ptr<Iyathuum::State> loaderState = std::make_shared<Iyathuum::State>("GameDataLoader");
  loaderState->setOnEnterCallback([&]() { loader.start(); });
  loaderState->setOnLeaveCallback([&]() { eye.setGamedata(loader.getGamedata()); });
  _updateMap[loaderState] = [&]() { loader.update(); };
  _drawMap  [loaderState] = [&]() { loader.draw();   };
  _menuMap  [loaderState] = [&]() { loader.menu();   };
  state.addState(loaderState);

  std::shared_ptr<Iyathuum::State> eyeState = std::make_shared<Iyathuum::State>("EyeOfRhianne");
  eyeState->setOnEnterCallback([&eye]() {eye.init(); });
  _updateMap[eyeState] = [&eye]() { eye.update(); };
  _drawMap  [eyeState] = [&eye]() { eye.draw  (); };
  _menuMap  [eyeState] = [&eye]() { eye.menu  (); };
  state.addState(eyeState);
  
  std::shared_ptr<Iyathuum::StateTransition> toPickerTransition = std::make_shared<Iyathuum::StateTransition>(startState, pickerState, [&]() {return true; });
  state.addTransition(toPickerTransition);
  std::shared_ptr<Iyathuum::StateTransition> toLoadTransition = std::make_shared<Iyathuum::StateTransition>(pickerState, loaderState, [&]() {return picker.finished(); });
  state.addTransition(toLoadTransition);
  std::shared_ptr<Iyathuum::StateTransition> toEyeTransition = std::make_shared<Iyathuum::StateTransition>(loaderState, eyeState, [&]() {return loader.finished(); });
  state.addTransition(toEyeTransition);


  // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

  std::unique_ptr<Ahwassa::IMGUIRenderer> ui;
  
  std::unique_ptr<GamedataPicker> gamedataPicker;

  w.Startup = [&]() {
    ui             = std::make_unique<Ahwassa::IMGUIRenderer>(&w);
    gamedataPicker = std::make_unique<GamedataPicker>(config);
  };

  w.Update = [&]() {
    state.update();

    _updateMap[state.currentState()]();
    _drawMap  [state.currentState()]();

    ImGuiIO& io = ImGui::GetIO();
    io.FontGlobalScale = 2;
    ui->start();
    _menuMap[state.currentState()]();
    ui->end();
  };
  w.Resize = [&](const glm::ivec2& newResolution) {
    if (eyeState == state.currentState())
      eye.resize(newResolution);
  };

  w.run();
  config.toJsonFile("Configuration.json");

  Iyathuum::DatabaseTerminator::terminateAll();
}