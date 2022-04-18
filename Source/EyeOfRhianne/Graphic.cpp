#include "Graphic.h"

#include <filesystem>
#include <glm/gtc/matrix_transform.hpp>

#include <AhwassaGraphicsLib/Core/Window.h>
#include <AhwassaGraphicsLib/Core/Renderer.h>
#include <AhwassaGraphicsLib/Core/Camera.h>
#include <AhwassaGraphicsLib/Uniforms/CubeTexture.h>
#include <AhwassaGraphicsLib/Uniforms/Texture.h>
#include <AhwassaGraphicsLib/PostProcessing/DeferredComposer.h>
#include <AhwassaGraphicsLib/PostProcessing/AdditiveComposer.h>
#include <AhwassaGraphicsLib/PostProcessing/Bloom.h>
#include <AhwassaGraphicsLib/PostProcessing/CubeReflection.h>
#include <AhwassaGraphicsLib/Renderer/BoxRenderer.h>
#include <AhwassaGraphicsLib/Uniforms/Rendertarget.h>

#include <HaasScriptingLib/ScriptEngine.h>
#include <AhwassaGraphicsLib/Renderer/BasicTexture2DRenderer.h>
#include <AthanahCommonLib/SupCom/SupComMeshRendererDef.h>
#include <AthanahCommonLib/SupCom/SupComModel.h>
#include <AthanahCommonLib/SkyBox.h>
#include <AthanahCommonLib/Map/MapRenderer.h>

Graphic::Graphic(Ahwassa::Window* window) {
  _window         = window;
  _scripts = std::make_shared<Haas::ScriptEngine>();
  _composer       = std::make_shared<Ahwassa::DeferredComposer     >(_window, _window->getResolution());
  _bloom          = std::make_shared<Ahwassa::Bloom                >(_window, _window->getResolution());
  _cubeReflection = std::make_shared<Ahwassa::CubeReflection       >(_window, _window->getResolution());
  _renderer       = std::make_shared<Athanah::SupComMeshRendererDef>(_window->camera());
  _reflectionTexture = std::make_shared<Ahwassa::CubeTexture>("Reflection", 0);


  _textures.push_back(_bloom->getResult());
  _textures.push_back(_cubeReflection->getResult());
  _textures.push_back(_composer->getResult());
  for (auto x : _composer->getRawTextures())
    _textures.push_back(x);
  _textures.push_back(_composer->getDepth());
}

void Graphic::update() {
  if (_playAnimation)
    _time = std::fmod(_time + 0.01f, 1);
  if (_mesh)
    _mesh->animation = getAnimation();
}

void Graphic::draw() {
  drawScene();
  _window->renderer().texture().start();
  _window->renderer().texture().draw(*_textures[_renderedTexture], Iyathuum::glmAABB<2>(glm::vec2(0, 0), (glm::vec2)_window->getResolution()), true);
  _window->renderer().texture().end();

}

void Graphic::drawScene() {
  _composer->start();
  if (_skyBox)
    _skyBox->draw();
  if (_mapRenderer)
    _mapRenderer->draw(*_mapMesh);
  _renderer->draw();
  getWindow()->renderer().draw();
  _composer->end();
  _cubeReflection->setTexture(_reflectionTexture);
  _cubeReflection->draw(_window->camera()->getPosition(), _composer->getResult(), _composer->getRawTextures()[3], _composer->getRawTextures()[0], _composer->getRawTextures()[1], 0);
  if (_bloomEnabled)
    _bloom->draw(_cubeReflection->getResult(), _composer->getRawTextures()[3], 1);
}

std::unique_ptr<Iyathuum::MultiDimensionalArray<Iyathuum::Color, 2>> Graphic::screenshot() {
  _window->renderer().texture().start();
  _window->renderer().texture().draw(*_textures[_renderedTexture], Iyathuum::glmAABB<2>(glm::vec2(0, 0), (glm::vec2)_window->getResolution()), true);
  _window->renderer().texture().end();

  return _textures[_renderedTexture]->getImage();
}

Ahwassa::Window* Graphic::getWindow() {
  return _window;
}

void Graphic::setModel(std::shared_ptr<Athanah::SupComModel> model) {
  _model = model;
  _mesh = std::make_shared<Athanah::SupComMesh>();
  _mesh->model = model;
  _mesh->teamColor = Iyathuum::Color(rand() % 255, rand() % 255, rand() % 255);
  _mesh->transformation = glm::translate(glm::mat4(1.0), glm::vec3(0, 0, 0));
  _renderer->addMesh(_mesh);   
  _time = 0;
  _currentAnimation = "None";
}

std::vector<glm::mat4> Graphic::getAnimation() {
  if (_currentAnimation == "None" || !_model)
    return std::vector<glm::mat4>();
  return _model->getAnimation(_currentAnimation, _model->getAnimationLength(_currentAnimation) * _time);
}
