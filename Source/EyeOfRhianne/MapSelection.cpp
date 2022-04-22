#include "MapSelection.h"

#include <glm/gtc/matrix_transform.hpp>
#include <filesystem>
#include <imgui.h>

#include "AhwassaGraphicsLib/Core/Window.h"
#include "AhwassaGraphicsLib/Uniforms/Texture.h"
#include "AhwassaGraphicsLib/Geometry/HeightFieldMeshGenerator.h"
#include "AhwassaGraphicsLib/Vertex/PositionColorNormalVertex.h"

#include "Graphic.h"
#include "AthanahCommonLib/Map/Scenario.h"
#include "AthanahCommonLib/Map/Map.h"
#include "AthanahCommonLib/Map/MapFactory.h"
#include "AthanahCommonLib/Map/MapRenderer.h"
#include "AthanahCommonLib/SupCom/Gamedata/Gamedata.h"
#include "AezeselFileIOLib/ImageIO.h"
#include "AezeselFileIOLib/SupremeCommander/SCMAP.h"

MapSelection::MapSelection(std::string mapPath, Graphic& graphic, Athanah::Gamedata& gamedata) : _graphic(graphic), _gamedata(gamedata){
  _factory = std::make_shared<Athanah::MapFactory>(mapPath);

  std::vector<std::string> maps;
  std::vector<std::string> niceNamesMaps;

//  _list = std::make_unique<ListSelection>(maps, niceNamesMaps, area, _graphic.getWindow(), [mapPath, this](std::string newMap) {
//
//  }, [this](Iyathuum::glmAABB<2> loc, std::string name, bool hovered) {
//    _graphic.getWindow()->renderer().rectangle().start();
//    _graphic.getWindow()->renderer().rectangle().drawRectangle(loc, hovered ? Iyathuum::Color(0.8f * 255, 0.8f * 255, 0.8f * 255) : Iyathuum::Color(0.4f * 255, 0.4f * 255, 0.4f * 255));
//    _graphic.getWindow()->renderer().rectangle().end();
//    _graphic.getWindow()->renderer().texture().start();
//    Iyathuum::glmAABB<2> textureLoc = loc;
//    auto map = _factory->loadMap(name);
//    textureLoc.setSize(glm::vec2(textureLoc.getSize()[1], textureLoc.getSize()[1]));
//    _graphic.getWindow()->renderer().texture().draw(*map->getPreview(), textureLoc);
//    _graphic.getWindow()->renderer().texture().end();
//    _graphic.getWindow()->renderer().text().start();
//    _graphic.getWindow()->renderer().text().drawText(map->getScenario().name(), loc.getPosition() + glm::vec2(textureLoc.getSize()[0], 0),0.6f);
//    _graphic.getWindow()->renderer().text().end();
//  });

}

void MapSelection::update() {

}

void MapSelection::menu() {
  ImGuiIO& io = ImGui::GetIO();
  if (ImGui::TreeNode("Map")) {
    for (auto entry : _factory->getAvailableMaps()) {
      auto map = _factory->loadMap(entry);
      
      ImGui::Image((ImTextureID)map->getPreview()->getTextureID(), ImVec2(80 * io.FontGlobalScale, 80 * io.FontGlobalScale));
      ImGui::SameLine();
      ImGui::BeginGroup();
      ImGui::Text(map->getScenario().name().c_str());
      if (map->getScenario().type() == Athanah::ScenarioType::Campaign)
        ImGui::Text("Type: Campaign");
      else
        ImGui::Text("Type: Skirmish");
      ImGui::Text(("Size: " + std::to_string(map->getScenario().width()) + "x" + std::to_string(map->getScenario().height())).c_str());
      if (ImGui::Button(("Load##" + map->getScenario().name()).c_str())) {
        setMap(map);
        _graphic.getWindow()->camera()->moveTo(map->aabb().getCenter());
      }
      ImGui::EndGroup();
    }
    ImGui::TreePop();
  }
}

void MapSelection::setMap(std::shared_ptr<Athanah::Map> map) {
  map->loadFull();

  std::array<std::string, 5> textures;
  for (int i = 0; i < 5; i++) {
    textures[i] = map->scmap().terrainTexturePaths[i].path;
  }

  _graphic._mapRenderer = std::make_shared<Athanah::MapRenderer>(_graphic.getWindow()->camera(), textures, _gamedata);

  auto tinter = [&](const std::array<size_t, 2> position, Ahwassa::PositionColorNormalVertex& v) {
    std::array<size_t, 2> half = { position[0] / 2,position[1] / 2 };
    if (position[0] == map->scmap().heightMapData->getDimension(0) - 1) half[0] = (position[0] - 1) / 2;
    if (position[1] == map->scmap().heightMapData->getDimension(1) - 1) half[1] = (position[1] - 1) / 2;
    v.color = map->scmap().highTexture->getVal(half).to4();
  };

  _graphic._mapMesh = Ahwassa::HeightFieldMeshGenerator::generate<unsigned short, Ahwassa::PositionColorNormalVertex>(*map->scmap().heightMapData, 0, std::numeric_limits<unsigned short>().max(), tinter, 2000, 1);
  _graphic._currentMap = map;
}