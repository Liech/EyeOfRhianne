#include "UnitModelSelection.h"

#include <glm/gtc/matrix_transform.hpp>
#include <imgui.h>
#include <iostream>

#include <AthanahCommonLib/SupCom/Gamedata/Gamedata.h>
#include <AthanahCommonLib/SupCom/Gamedata/SupComModelFactory.h>
#include <AthanahCommonLib/SupCom/Gamedata/UiTextureFactory.h>
#include <AthanahCommonLib/SupCom/Gamedata/BlueprintFactory.h>
#include <AthanahCommonLib/SupCom/SupComModel.h>
#include <AthanahCommonLib/SupCom/Blueprint/Blueprint.h>
#include <AthanahCommonLib/SupCom/Blueprint/BlueprintGeneral.h>
#include <AthanahCommonLib/SupCom/Blueprint/BlueprintDisplay.h>
#include <AhwassaGraphicsLib/Core/Window.h>
#include <AhwassaGraphicsLib/Uniforms/Texture.h>
#include <HaasScriptingLib/ScriptEngine.h>
#include <AezeselFileIOLib/STLWriter.h>

#include "Graphic.h"

UnitModelSelection::UnitModelSelection(Athanah::Gamedata& gamedata, Graphic& graphic) : 
  _graphic(graphic),
  _gamedata(gamedata)
{
  for (auto& cat : _categories)
    _names[cat] = getNames(cat);

  initScript();
}

void UnitModelSelection::setModel(std::string newModel) {
  _currentID = newModel;
  _graphic.setModel(getCurrentModel());
  float scale = _gamedata.blueprint().loadModel(_currentID)->display().scale() * 30;
  _graphic._mesh->transformation = glm::scale(glm::mat4(1), glm::vec3(scale, scale, scale));
}

std::shared_ptr<Ahwassa::Texture> UnitModelSelection::getFactionIcon(const std::string& s) {
  if (s == "UEF")
    return _gamedata.icon().getFactionIcon(Athanah::Faction::Uef, Athanah::FactionIconType::Normal);
  else if (s == "Cybran")
    return _gamedata.icon().getFactionIcon(Athanah::Faction::Cybran, Athanah::FactionIconType::Normal);
  else if (s == "Aeon")
    return _gamedata.icon().getFactionIcon(Athanah::Faction::Aeon, Athanah::FactionIconType::Normal);
  else if (s == "Seraphim")
    return _gamedata.icon().getFactionIcon(Athanah::Faction::Seraphim, Athanah::FactionIconType::Normal);
  else if (s == "Other")
    return _gamedata.icon().getFactionIcon(Athanah::Faction::Undefined, Athanah::FactionIconType::Normal);
  return _gamedata.icon().getTierIcons(Athanah::Faction::Uef,Athanah::TechLevel::T4);
}

std::shared_ptr<Athanah::SupComModel> UnitModelSelection::getCurrentModel() {
  return _gamedata.model().loadModel(_currentID);
}

void UnitModelSelection::update() {

}

void UnitModelSelection::menu() {
  ImGuiIO& io = ImGui::GetIO();
  if (ImGui::TreeNode("Model"))
  {
      for (auto& cat : _categories) {
          ImGui::Image((ImTextureID)getFactionIcon(cat)->getTextureID(), ImVec2(20* io.FontGlobalScale, 20* io.FontGlobalScale));
          ImGui::SameLine();
          if (ImGui::TreeNode(cat.c_str())) {
              for (auto& name : _names[cat]) {
                  unitMenuItem(name);
              }
              ImGui::TreePop();
          }
      }
      ImGui::TreePop();
  }
}

void UnitModelSelection::unitMenuItem(const std::string& unitName) {
  ImGuiIO& io = ImGui::GetIO();
  auto bp = _gamedata.blueprint().loadModel(unitName);
  ImTextureID strategicIcon = (ImTextureID)_gamedata.icon().getStrategicIcon(bp->strategicIcon(), Athanah::SelectableButtonStatus::Normal)->getTextureID();

  ImGui::Image(strategicIcon, ImVec2(20 * io.FontGlobalScale, 20 * io.FontGlobalScale));
  ImGui::SameLine();
  if (true) {//ImGui::TreeNode((bp->general().unitName() + "##" + unitName).c_str())
    ImGui::Text(bp->general().unitName().c_str());
    auto icon = _gamedata.icon().getIcon(unitName);
    auto background = _gamedata.icon().getBackgroundIcon(bp->general().icon(), Athanah::ButtonStatus::Normal);
    ImVec2 p = ImGui::GetCursorScreenPos();
    ImVec2 imageSize = ImVec2(50 * io.FontGlobalScale, 50 * io.FontGlobalScale);
    ImGui::Image((ImTextureID)background->getTextureID(), imageSize);
    ImGui::GetWindowDrawList()->AddImage((ImTextureID)icon->getTextureID(), p, ImVec2(p.x + imageSize.x, p.y + imageSize.y), ImVec2(0, 0), ImVec2(1, 1));
    ImGui::SameLine();

    {
      ImGui::BeginGroup();
      ImGui::Text(("ID: " + unitName).c_str());
      ImGui::Image((ImTextureID)_gamedata.icon().getFactionIcon(bp->general().faction(), Athanah::FactionIconType::Normal)->getTextureID(), ImVec2(20 * io.FontGlobalScale, 20 * io.FontGlobalScale));
      ImGui::SameLine();
      ImGui::Image(strategicIcon, ImVec2(20 * io.FontGlobalScale, 20 * io.FontGlobalScale));

      bool loadUnit = ImGui::Button(("Load##"+unitName).c_str());
      if (loadUnit) {
        setModel(unitName);
      }
      if (_currentID == unitName) {
        auto animations = _graphic._model->availableAnimations();
        if (animations.size() != 0) {
          if (ImGui::BeginPopupContextItem("Chose Animation")) {
            for (auto& anim : _graphic._model->availableAnimations()) {
              if (ImGui::Button(anim.c_str())) {
                _graphic._currentAnimation = anim;
                _graphic._time = 0;
              }
            }
            ImGui::EndMenu();
          }
          if (ImGui::Button("Animate"))
            ImGui::OpenPopup("Chose Animation");
        }
        if (ImGui::Button("Export")) {
          save();
        }
      }
      ImGui::EndGroup();
    }
  }
}

std::vector<std::string> UnitModelSelection::getNames(const std::string& category) {
  std::vector<std::string> result;

  for (auto x : _gamedata.model().getAvailableModels()) {
    if (!_gamedata.blueprint().hasBlueprint(x))
      continue;
    auto bp = _gamedata.blueprint().loadModel(x);
    auto faction = bp->general().faction();
    bool ok = 
      (category == "UEF"     && faction == Athanah::Faction::Uef      ) ||
      (category == "Cybran"  && faction == Athanah::Faction::Cybran   ) ||
      (category == "Aeon"    && faction == Athanah::Faction::Aeon     ) ||
      (category == "Seraphim"&& faction == Athanah::Faction::Seraphim ) ||
      (category == "Other"   && faction == Athanah::Faction::Undefined)
      ;
    if (ok) {
      result.push_back(x);
    }
  }
  return result;
}

void UnitModelSelection::initScript() {
  _setUnit = std::make_shared< std::function<nlohmann::json(const nlohmann::json&)>>(
    [&](const nlohmann::json& input) -> nlohmann::json
  {
    std::string name = input;
    setModel(name);
    return 1;
  }
  );
  _getUnit = std::make_shared< std::function<nlohmann::json(const nlohmann::json&)>>(
    [&](const nlohmann::json& input) -> nlohmann::json
  {
    return _currentID;
  }
  );
  _getAllUnits = std::make_shared< std::function<nlohmann::json(const nlohmann::json&)>>(
    [&](const nlohmann::json& input) -> nlohmann::json
  {
    return _gamedata.model().getAvailableModels();
  }
  );
  _getBlueprint = std::make_shared< std::function<nlohmann::json(const nlohmann::json&)>>(
    [&](const nlohmann::json& input) -> nlohmann::json
  {
    return _gamedata.blueprint().loadModel(input)->getRaw();
  }
  );
  _setUnitColor = std::make_shared< std::function<nlohmann::json(const nlohmann::json&)>>(
    [&](const nlohmann::json& input) -> nlohmann::json
  {
    Iyathuum::Color clr(input[0], input[1], input[2],input[3]);
    _graphic._mesh->teamColor = clr;
    return 1;
  }
  );
  _graphic._scripts->registerFunction("eyeSetUnit"     , _setUnit     );
  _graphic._scripts->registerFunction("eyeGetAllUnits" , _getAllUnits );
  _graphic._scripts->registerFunction("eyeGetUnit"     , _getUnit     );
  _graphic._scripts->registerFunction("eyeGetBlueprint", _getBlueprint);
  _graphic._scripts->registerFunction("eyeSetUnitColor", _setUnitColor);
}

void UnitModelSelection::save() {
  std::vector<glm::vec3> data;
  data.resize(_graphic._model->scm().indices.size() * 3);
  auto anim = _graphic.getAnimation();
  for (size_t i = 0; i < _graphic._model->scm().indices.size(); i++) {
    const auto& v1 = _graphic._model->scm().vertecies[_graphic._model->scm().indices[i].a];
    const auto& v2 = _graphic._model->scm().vertecies[_graphic._model->scm().indices[i].b];
    const auto& v3 = _graphic._model->scm().vertecies[_graphic._model->scm().indices[i].c];
    auto mat1 = glm::mat4(1.0);
    auto mat2 = glm::mat4(1.0);
    auto mat3 = glm::mat4(1.0);

    if (_graphic._currentAnimation != "None") {
      mat1 = anim[v1.boneIndex[0]];
      mat2 = anim[v2.boneIndex[0]];
      mat3 = anim[v3.boneIndex[0]];
    }

    data[i * 3 + 0] = mat1 * glm::vec4(v1.position, 1);
    data[i * 3 + 1] = mat2 * glm::vec4(v2.position, 1);
    data[i * 3 + 2] = mat3 * glm::vec4(v3.position, 1);
  }
  Aezesel::STLWriter::write("Output.stl", data);
}