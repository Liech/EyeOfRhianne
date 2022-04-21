#pragma once

#include <memory>
#include <string>
#include <nlohmann/json.hpp>


namespace Athanah {
  class SkyboxFactory;
}
class Graphic;

class SkyBoxMenuItem {
public:
  SkyBoxMenuItem(Athanah::SkyboxFactory&, Graphic&);
  virtual ~SkyBoxMenuItem() = default;

  void update();
  void menu();

private:
  void initScript();
  void setSkyBox(std::string box);

  Graphic&                       _graphic;  
  Athanah::SkyboxFactory&        _factory;
  std::string                    _path;
  std::string                    _currentSkybox = "";
  std::vector<std::string>       _allSkyboxes;
  std::vector<std::string>       _allNiceNames;

  std::shared_ptr<std::function<nlohmann::json(const nlohmann::json&)>> _setSkyBox     ;
  std::shared_ptr<std::function<nlohmann::json(const nlohmann::json&)>> _getSkyBox     ;
  std::shared_ptr<std::function<nlohmann::json(const nlohmann::json&)>> _getAllBoxes   ;
  std::shared_ptr<std::function<nlohmann::json(const nlohmann::json&)>> _disableSkyBox ;
  std::shared_ptr<std::function<nlohmann::json(const nlohmann::json&)>> _setSkyBoxColor;
};