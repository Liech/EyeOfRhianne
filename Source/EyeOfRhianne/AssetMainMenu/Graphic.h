#pragma once

#include <memory>
#include <vector>
#include <AthanahCommonLib/SupCom/SupComMeshRendererDef.h>
#include <IyathuumCoreLib/BaseTypes/MultiDimensionalArray.h>
#include <IyathuumCoreLib/BaseTypes/Color.h>

namespace Ahwassa {
  class Window;
  class DeferredComposer;
  class Bloom;
  class CubeReflection;
  class CubeTexture;
  class Texture;
  class AdditiveComposer;
  class BoxRenderer;
  struct DiffuseMeshRendererMesh;
  class SoundEngine;
  class SoundHandler;
}

namespace Aezesel {
  class SoundFactory;
}

namespace Athanah {
  class SkyBox;
  class Map;
  class MapRenderer;
}

namespace Haas {
  class ScriptEngine;
}

class Graphic {
public:
  Graphic(Ahwassa::Window*, const std::string& soundPath);
  virtual ~Graphic() = default;

  void update();
  void draw();
  std::unique_ptr<Iyathuum::MultiDimensionalArray<Iyathuum::Color,2>> screenshot();
  void setModel(std::shared_ptr<Athanah::SupComModel>); 
  std::vector<glm::mat4> getAnimation();
  void setResolution(const glm::ivec2&);

  std::shared_ptr<Athanah::SupComMesh> _mesh;
  int                                  _renderedTexture = 0;
  std::shared_ptr<Athanah::SupComModel> _model;
  bool                                  _playAnimation = true;
  float                                 _time = 0;
  std::string                           _currentAnimation = "None";

  float _ambientValue = 1;
  float _specularValue = 1;
  float _diffuseValue = 1;

  std::vector<std::shared_ptr<Ahwassa::Texture>>    _textures;
  std::shared_ptr<Athanah::SupComMeshRendererDef>   _renderer;
  std::shared_ptr<Ahwassa::Bloom>                   _bloom;
  std::shared_ptr<Ahwassa::CubeReflection  >        _cubeReflection     ;
  bool                                              _bloomEnabled = true;
  std::shared_ptr<Athanah::SkyBox>                  _skyBox;
  std::shared_ptr<Ahwassa::CubeTexture>             _reflectionTexture;
  std::shared_ptr<Athanah::Map>                     _currentMap;
  std::shared_ptr<Haas::ScriptEngine>               _scripts;
  std::shared_ptr<Ahwassa::IMesh>                   _mapMesh = nullptr;
  std::shared_ptr<Athanah::MapRenderer>             _mapRenderer = nullptr;
  std::shared_ptr<Aezesel::SoundFactory>            _soundFactory;
  std::shared_ptr<Ahwassa::SoundEngine >            _soundEngine;
  std::shared_ptr<Ahwassa::SoundHandler>            _currentSoundHandler = nullptr;
  Ahwassa::Window* getWindow();
private:
  void drawScene();

  Ahwassa::Window*                                _window  ;
  std::shared_ptr<Ahwassa::DeferredComposer>      _composer;
};