#pragma once

#include <glm/glm.hpp>
#include <string>
#include <memory>
#include <vector>
#include <map>
#include <functional>
#include "AthanahCommonLib/SupCom/SupComEnums.h"
#include "AthanahCommonLib/SupCom/Gamedata/Gamedata.h"
#include <nlohmann/json.hpp>

namespace Ahwassa {
  class Texture;
}
namespace Athanah {
  class SupComModel;
}

class Graphic;

class UnitMenuItem {
public:
  UnitMenuItem(Athanah::Gamedata& gamedata, Graphic&);
  virtual ~UnitMenuItem() = default;

  void menu();
  void update();
  void save();

  std::shared_ptr<Athanah::SupComModel> getCurrentModel();


private:
  const std::vector<std::string> _categories = std::vector<std::string>{ "UEF","Cybran","Aeon","Seraphim","Other","NonUnit" };

  void unitMenuItem(const std::string& unitName);
  void initScript();
  void setModel(std::string id);
  std::shared_ptr<Ahwassa::Texture> getFactionIcon(const std::string&);
  std::vector<std::string> getNames(const std::string& category);

  Graphic&                                        _graphic        ;
  Athanah::Gamedata&                              _gamedata       ;
  std::string                                     _currentID = "" ;
  std::map<std::string, std::vector<std::string>> _names;

  std::shared_ptr<std::function<nlohmann::json(const nlohmann::json&)>> _setUnit;
  std::shared_ptr<std::function<nlohmann::json(const nlohmann::json&)>> _setUnitColor;
  std::shared_ptr<std::function<nlohmann::json(const nlohmann::json&)>> _getUnit    ;
  std::shared_ptr<std::function<nlohmann::json(const nlohmann::json&)>> _getBlueprint;
  std::shared_ptr<std::function<nlohmann::json(const nlohmann::json&)>> _getAllUnits;

};