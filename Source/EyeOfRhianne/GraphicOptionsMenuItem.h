#pragma once

#include <memory>
#include <functional>
#include <map>
#include "Graphic.h"
#include <nlohmann/json.hpp>

namespace Ahwassa {
  class Window;
  class Button;
  class Texture;
  class Slider;
  class Label;
  class ListLayout;
  class Bloom;
}

class ListSelection;

class GraphicOptionsMenuItem {
public:
  GraphicOptionsMenuItem(Graphic& graphic);

  void update();
  void menu();

  std::shared_ptr<Ahwassa::Texture> getCurrentTexture();

private:
  void makeBloomOptions();
  void makeRendererOptions();
  void initBloomScript();
  void initRendererScript();

  std::map<std::string,int> _textures;
  std::shared_ptr<Ahwassa::Texture>              _currentTexture;
  Graphic& _graphic;


  std::shared_ptr<std::function<nlohmann::json(const nlohmann::json&)>> _setBloomQuality   ;
  std::shared_ptr<std::function<nlohmann::json(const nlohmann::json&)>> _setBloomDirections;
  std::shared_ptr<std::function<nlohmann::json(const nlohmann::json&)>> _setBloomIntensity ;
  std::shared_ptr<std::function<nlohmann::json(const nlohmann::json&)>> _setBloomSize      ;
  std::shared_ptr<std::function<nlohmann::json(const nlohmann::json&)>> _setRenderer;
  std::shared_ptr<std::function<nlohmann::json(const nlohmann::json&)>> _getRenderer;
  std::shared_ptr<std::function<nlohmann::json(const nlohmann::json&)>> _getAllRenderer;
};