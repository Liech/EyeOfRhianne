#include <iostream>
#include <filesystem>
#include <functional>

#include "IyathuumCoreLib/Singleton/Database.h"

#include "AhwassaGraphicsLib/Core/Window.h"
#include "AhwassaGraphicsLib/Core/Camera.h"
#include "AhwassaGraphicsLib/Drawables/FPS.h"
#include "AhwassaGraphicsLib/Widgets/Button.h"
#include "AhwassaGraphicsLib/Input/Input.h"
#include "AhwassaGraphicsLib/Input/FreeCamera.h"
#include "AhwassaGraphicsLib/Uniforms/Texture.h"

#include "AthanahCommonLib/SupCom/SupComModel.h"
#include "AhwassaGraphicsLib/PostProcessing/Bloom.h"

#include "AezeselFileIOLib/STLWriter.h"
#include "AezeselFileIOLib/SupremeCommander/SCM.h"
#include "AezeselFileIOLib/ImageIO.h"

#include "ListSelection.h"

#include "AssetSelection.h"
#include "GraphicOptions.h"
#include "Graphic.h"
#include "EyeOfRhianneConfiguration.h"

#include "AezeselFileIOLib/SupremeCommander/SCMAP.h"

#include <imgui.h>
#include "AhwassaGraphicsLib/Input/IMGUIRenderer.h"

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

  Ahwassa::Window w(width, height);
  std::unique_ptr<Ahwassa::FPS> fps;
  
  std::unique_ptr<AssetSelection>         assets   ;
  std::unique_ptr<GraphicOptions>         graphicUI;
  std::unique_ptr<Graphic>                graphic;
  std::unique_ptr<Ahwassa::IMGUIRenderer> ui;

  std::shared_ptr<Ahwassa::FreeCamera> freeCam;
  w.Startup = [&]() {
    freeCam = std::make_shared<Ahwassa::FreeCamera>(w.camera(), w.input(), Iyathuum::Key::MOUSE_BUTTON_1);
    w.camera()->setPosition(glm::vec3(20, 20, 20));
    //w.input().addUIElement(freeCam.get(), 1);

    graphic = std::make_unique<Graphic               >(&w);
    assets  = std::make_unique<AssetSelection        >(config, *graphic);
    fps     = std::make_unique<Ahwassa::FPS          >(&w);
    ui      = std::make_unique<Ahwassa::IMGUIRenderer>(&w);
  };

  w.Update = [&]() {
    assets->update();

    graphic->draw();

    ImGuiIO& io = ImGui::GetIO();
    io.FontGlobalScale = 2;
    ui->start();
    assets->menu();
    //ImGui::ShowDemoWindow();
    ui->end();

    if (config.ShowFPS) 
     fps->draw();    
  };
  w.run();

  Iyathuum::DatabaseTerminator::terminateAll();
}