#include "GraphicOptionsMenuItem.h"

#include <imgui.h>

#include "AhwassaGraphicsLib/Core/Window.h"
#include "AhwassaGraphicsLib/Widgets/Button.h"
#include "AhwassaGraphicsLib/Uniforms/Texture.h"
#include "IyathuumCoreLib/BaseTypes/glmAABB.h"
#include "ListSelection.h"
#include "AhwassaGraphicsLib/Widgets/Slider.h"
#include "AhwassaGraphicsLib/Widgets/Label.h"
#include "AhwassaGraphicsLib/Widgets/ListLayout.h"
#include "AhwassaGraphicsLib/PostProcessing/Bloom.h"
#include "HaasScriptingLib/ScriptEngine.h"

GraphicOptionsMenuItem::GraphicOptionsMenuItem(Graphic& graphic): _graphic(graphic){
  std::vector<std::string> textureNames;
  for (int i = 0; i < _graphic._textures.size(); i++){
    _textures[_graphic._textures[i]->getName()] = i;
    textureNames.push_back(_graphic._textures[i]->getName());
  }
  _currentTexture = _graphic._textures[0];

  initBloomScript();
  initRendererScript();
}

void GraphicOptionsMenuItem::makeBloomOptions() {
  if (ImGui::TreeNode("Bloom")) {
    float size = _graphic._bloom->size();
    ImGui::SliderFloat("Size", &size, 0, 100);
    _graphic._bloom->setSize(size);

    int direction = _graphic._bloom->directions();
    ImGui::SliderInt("Directions", &direction, 15, 100);
    _graphic._bloom->setDirections(direction);

    float intensity = _graphic._bloom->intensity();
    ImGui::SliderFloat("Intensity", &intensity, 0, 100);
    _graphic._bloom->setIntensity(intensity);

    float quality = _graphic._bloom->quality();
    ImGui::SliderFloat("Quality", &quality, 0, 20);
    _graphic._bloom->setQuality(quality);

    ImGui::TreePop();
  }
}

void GraphicOptionsMenuItem::makeRendererOptions() {
  if (ImGui::TreeNode("Renderer")) {
    ImGui::SliderFloat("Ambient" , &_graphic._ambientValue , 0.0f, 3.0f);
    ImGui::SliderFloat("Diffuse" , &_graphic._diffuseValue , 0.0f, 3.0f);
    ImGui::SliderFloat("Specular", &_graphic._specularValue, 0.0f, 3.0f);

    int radioButtonChoice = _graphic._renderedTexture;
    for (int i = 0; i < _graphic._textures.size(); i++) {
      ImGui::RadioButton(_graphic._textures[i]->getName().c_str(), &radioButtonChoice,i);
    }
    _graphic._renderedTexture = radioButtonChoice;


    ImGui::TreePop();
  }
}

void GraphicOptionsMenuItem::menu() {
  if (ImGui::TreeNode("Graphic Options")) {
    makeBloomOptions();
    makeRendererOptions();
    ImGui::TreePop();
  }  
}

void GraphicOptionsMenuItem::update() {

}

std::shared_ptr<Ahwassa::Texture> GraphicOptionsMenuItem::getCurrentTexture() {
  return _currentTexture;
}

void GraphicOptionsMenuItem::initBloomScript() {
  _setBloomQuality = std::make_shared< std::function<nlohmann::json(const nlohmann::json&)>>(
    [&](const nlohmann::json& input) -> nlohmann::json
  {
    _graphic._bloom->setQuality(input);
    return 1;
  }
  );
  _graphic._scripts->registerFunction("eyeSetBloomQuality", _setBloomQuality);

  _setBloomSize = std::make_shared< std::function<nlohmann::json(const nlohmann::json&)>>(
    [&](const nlohmann::json& input) -> nlohmann::json
  {
    _graphic._bloom->setSize(input);
    return 1;
  }
  );
  _graphic._scripts->registerFunction("eyeSetBloomSize", _setBloomSize);


  _setBloomDirections = std::make_shared< std::function<nlohmann::json(const nlohmann::json&)>>(
    [&](const nlohmann::json& input) -> nlohmann::json
  {
    _graphic._bloom->setDirections(input);
    return 1;
  }
  );
  _graphic._scripts->registerFunction("eyeSetBloomDirections", _setBloomDirections);


  _setBloomIntensity = std::make_shared< std::function<nlohmann::json(const nlohmann::json&)>>(
    [&](const nlohmann::json& input) -> nlohmann::json
  {
    _graphic._bloom->setIntensity(input);
    return 1;
  }
  );
  _graphic._scripts->registerFunction("eyeSetBloomIntensity", _setBloomIntensity);
}

void GraphicOptionsMenuItem::initRendererScript() {
  _setRenderer = std::make_shared< std::function<nlohmann::json(const nlohmann::json&)>>(
    [&](const nlohmann::json& input) -> nlohmann::json {
      std::string name = input;
      _graphic._renderedTexture = _textures[name];
      return 1;
    }
  );
  _getAllRenderer = std::make_shared< std::function<nlohmann::json(const nlohmann::json&)>>(
    [&](const nlohmann::json& input) -> nlohmann::json {


      std::vector<std::string> result;
      for (int i = 0; i < _graphic._textures.size(); i++)
        result.push_back(_graphic._textures[i]->getName());

      return result;
    }
  );
  _getRenderer = std::make_shared< std::function<nlohmann::json(const nlohmann::json&)>>(
    [&](const nlohmann::json& input) -> nlohmann::json {
      return _graphic._textures[_graphic._renderedTexture]->getName();
    }
  );
  _graphic._scripts->registerFunction("eyeSetRenderer", _setRenderer);
  _graphic._scripts->registerFunction("eyeGetAllRenderer", _getAllRenderer);
  _graphic._scripts->registerFunction("eyeGetRenderer", _getRenderer);

}