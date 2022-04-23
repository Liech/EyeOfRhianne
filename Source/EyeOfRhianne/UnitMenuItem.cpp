#include "UnitMenuItem.h"

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
#include <AhwassaGraphicsLib/Core/Camera.h>
#include <AhwassaGraphicsLib/Uniforms/Texture.h>
#include <HaasScriptingLib/ScriptEngine.h>
#include <AezeselFileIOLib/STLWriter.h>

#include "Graphic.h"

UnitMenuItem::UnitMenuItem(Athanah::Gamedata& gamedata, Graphic& graphic) : 
  _graphic(graphic),
  _gamedata(gamedata)
{
  for (auto& cat : _categories)
    _names[cat] = getNames(cat);

  initScript();
}

void UnitMenuItem::setModel(std::string newModel) {
  _currentID = newModel;
  _graphic.setModel(getCurrentModel());
  float scale = _gamedata.blueprint().loadModel(_currentID)->display().scale() * 30;
  _graphic._mesh->transformation = glm::scale(glm::mat4(1), glm::vec3(scale, scale, scale));
  auto aabb = getCurrentModel()->scm().aabb;
  _graphic.getWindow()->camera()->moveTo(aabb.getCenter() * scale);
}

std::shared_ptr<Ahwassa::Texture> UnitMenuItem::getFactionIcon(const std::string& s) {
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
  return _gamedata.icon().getFactionIcon(Athanah::Faction::Undefined, Athanah::FactionIconType::Normal);
}

std::shared_ptr<Athanah::SupComModel> UnitMenuItem::getCurrentModel() {
  return _gamedata.model().loadModel(_currentID);
}

void UnitMenuItem::update() {

}

void UnitMenuItem::menu() {
  ImGuiIO& io = ImGui::GetIO();
  if (ImGui::TreeNode("Model")) {
    //for (auto& cat : _categories) {
    //    ImGui::Image((ImTextureID)getFactionIcon(cat)->getTextureID(), ImVec2(20* io.FontGlobalScale, 20* io.FontGlobalScale));
    //    ImGui::SameLine();
    //    if (ImGui::TreeNode(cat.c_str())) {
    //        for (auto& name : _names[cat]) {
    //            unitMenuItem(name);
    //        }
    //        ImGui::TreePop();
    //    }
    //}

    ImGui::Image((ImTextureID)getFactionIcon("UEF")->getTextureID(), ImVec2(20 * io.FontGlobalScale, 20 * io.FontGlobalScale));
    ImGui::SameLine();
    if (ImGui::TreeNode("UEF")) {
      factionTechNode(Athanah::TechLevel::T1, Athanah::Faction::Uef);
      factionTechNode(Athanah::TechLevel::T2, Athanah::Faction::Uef);
      factionTechNode(Athanah::TechLevel::T3, Athanah::Faction::Uef);
      factionTechNode(Athanah::TechLevel::T4, Athanah::Faction::Uef);
      ImGui::TreePop();
    }

    ImGui::Image((ImTextureID)getFactionIcon("Cybran")->getTextureID(), ImVec2(20 * io.FontGlobalScale, 20 * io.FontGlobalScale));
    ImGui::SameLine();
    if (ImGui::TreeNode("Cybran")) {
      factionTechNode(Athanah::TechLevel::T1, Athanah::Faction::Cybran);
      factionTechNode(Athanah::TechLevel::T2, Athanah::Faction::Cybran);
      factionTechNode(Athanah::TechLevel::T3, Athanah::Faction::Cybran);
      factionTechNode(Athanah::TechLevel::T4, Athanah::Faction::Cybran);
      ImGui::TreePop();
    }

    ImGui::Image((ImTextureID)getFactionIcon("Aeon")->getTextureID(), ImVec2(20 * io.FontGlobalScale, 20 * io.FontGlobalScale));
    ImGui::SameLine();
    if (ImGui::TreeNode("Aeon")) {
      factionTechNode(Athanah::TechLevel::T1, Athanah::Faction::Aeon);
      factionTechNode(Athanah::TechLevel::T2, Athanah::Faction::Aeon);
      factionTechNode(Athanah::TechLevel::T3, Athanah::Faction::Aeon);
      factionTechNode(Athanah::TechLevel::T4, Athanah::Faction::Aeon);
      ImGui::TreePop();
    }

    ImGui::Image((ImTextureID)getFactionIcon("Seraphim")->getTextureID(), ImVec2(20 * io.FontGlobalScale, 20 * io.FontGlobalScale));
    ImGui::SameLine();
    if (ImGui::TreeNode("Seraphim")) {
      factionTechNode(Athanah::TechLevel::T1, Athanah::Faction::Seraphim);
      factionTechNode(Athanah::TechLevel::T2, Athanah::Faction::Seraphim);
      factionTechNode(Athanah::TechLevel::T3, Athanah::Faction::Seraphim);
      factionTechNode(Athanah::TechLevel::T4, Athanah::Faction::Seraphim);
      ImGui::TreePop();
    }

    if (ImGui::TreeNode("Search")) {
      
      if (ImGui::BeginPopupContextItem("DefineCategories")) {
        for (auto x : Athanah::allUnitCategories()) {
          bool checked = _currentSearch.contains(x);
          ImGui::Checkbox(unitCategory2niceString(x).c_str(), &checked);
          if (_currentSearch.contains(x)) {
            if (!checked)
              _currentSearch.erase(x);
          }
          else {
            if (checked)
              _currentSearch.insert(x);
          }            
        }
        ImGui::EndPopup();
      }
      if (ImGui::Button("Define Categories"))
        ImGui::OpenPopup("DefineCategories");

      listItemsOfCategory(_currentSearch);
      ImGui::TreePop();
    }

    ImGui::TreePop();
  }
}

void UnitMenuItem::factionTechNode(Athanah::TechLevel lvl, Athanah::Faction faction) {
  ImGuiIO& io = ImGui::GetIO();

  Athanah::UnitCategory cat;
  std::string name;
  if (lvl == Athanah::TechLevel::T1) {
    name = "T1";
    cat = Athanah::UnitCategory::Tech1;
  }
  if (lvl == Athanah::TechLevel::T2) {
    name = "T2";
    cat = Athanah::UnitCategory::Tech2;
  }
  if (lvl == Athanah::TechLevel::T3) {
    name = "T3";
    cat = Athanah::UnitCategory::Tech3;
  }
  if (lvl == Athanah::TechLevel::T4) {
    name = "T4";
    cat = Athanah::UnitCategory::Experimental;
  }
  Athanah::UnitCategory facCat;
  if (faction == Athanah::Faction::Aeon)
    facCat = Athanah::UnitCategory::Aeon;
  if (faction == Athanah::Faction::Cybran)
    facCat = Athanah::UnitCategory::Cybran;
  if (faction == Athanah::Faction::Seraphim)
    facCat = Athanah::UnitCategory::Seraphim;
  if (faction == Athanah::Faction::Uef)
    facCat = Athanah::UnitCategory::UEF;

  ImTextureID icon = (ImTextureID)_gamedata.icon().getTierIcon(faction, lvl, Athanah::SelectableButtonStatus::Normal)->getTextureID();
  ImGui::Image(icon, ImVec2(20 * io.FontGlobalScale, 20 * io.FontGlobalScale));
  ImGui::SameLine();
  if (ImGui::TreeNode(name.c_str())) {
    listItemsOfCategory({ facCat, cat });
    ImGui::TreePop();
  }
}

void UnitMenuItem::listItemsOfCategory(const std::set<Athanah::UnitCategory>& filter) {
  for (auto& unit : _gamedata.blueprint().getModelsByCategory(filter)) {
    unitMenuItem(unit);
    //ImGui::Text(unit.c_str());
  }
}

void UnitMenuItem::unitMenuItem(const std::string& unitName) {
  ImGuiIO& io = ImGui::GetIO();
  auto bp = _gamedata.blueprint().loadModel(unitName);
  ImTextureID strategicIcon = (ImTextureID)_gamedata.icon().getStrategicIcon(bp->strategicIcon(), Athanah::SelectableButtonStatus::Normal)->getTextureID();
  ImTextureID techIcon = (ImTextureID)_gamedata.icon().getTierIcon(bp->general().faction(),bp->techLevel(), Athanah::SelectableButtonStatus::Normal)->getTextureID();

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
      ImGui::Image(techIcon, ImVec2(20 * io.FontGlobalScale, 20 * io.FontGlobalScale));
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
          if (_graphic._currentAnimation != "None") {
            ImGui::SameLine();
            if (_graphic._playAnimation && ImGui::Button("Pause")) {
              _graphic._playAnimation = false;
            }
            else if (!_graphic._playAnimation && ImGui::Button("Play")) {
              _graphic._playAnimation = true;
            }
          }
        }
        if (ImGui::Button("Export")) {
          save();
        }
      }
      ImGui::EndGroup();
    }
  }
}

std::vector<std::string> UnitMenuItem::getNames(const std::string& category) {
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

void UnitMenuItem::initScript() {
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

void UnitMenuItem::save() {
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