#include "EyeOfRhianneConfiguration.h"

nlohmann::json EyeOfRhianneConfiguration::toJson() const {
  nlohmann::json result;
  result["ScreenWidth"]  = ScreenWidth ;
  result["ScreenHeight"] = ScreenHeight;
  result["AssetPath"]    = AssetPath   ;
  result["ShowFPS"]      = ShowFPS     ;
  result["SupComPath"]   = SupComPath  ;
  result["UseSCD"]       = UseSCD      ;
  return result;
}

void EyeOfRhianneConfiguration::fromJson(nlohmann::json from) {
  ScreenWidth  = from["ScreenWidth"] ;
  ScreenHeight = from["ScreenHeight"];
  AssetPath    = from["AssetPath"];
  ShowFPS      = from["ShowFPS"];
  SupComPath   = from["SupComPath"];
  UseSCD       = from["UseSCD"]  ;
}

Vishala::BinaryPackage EyeOfRhianneConfiguration::toBinary()const {
  Vishala::BinaryPackage result;
  Vishala::BinaryPackage::val2bin<int        >(result, ScreenWidth );
  Vishala::BinaryPackage::val2bin<int        >(result, ScreenHeight);
  Vishala::BinaryPackage::val2bin<bool       >(result, ShowFPS     );
  Vishala::BinaryPackage::val2bin<std::string>(result, AssetPath   );
  Vishala::BinaryPackage::val2bin<std::string>(result, SupComPath  );
  Vishala::BinaryPackage::val2bin<bool       >(result, UseSCD      );
  return result;
}

void EyeOfRhianneConfiguration::fromBinary(Vishala::BinaryPackage& data) {
  ScreenWidth  = Vishala::BinaryPackage::bin2val<int        >(data);
  ScreenHeight = Vishala::BinaryPackage::bin2val<int        >(data);
  ShowFPS      = Vishala::BinaryPackage::bin2val<bool       >(data);
  AssetPath    = Vishala::BinaryPackage::bin2val<std::string>(data);
  SupComPath   = Vishala::BinaryPackage::bin2val<std::string>(data);
  UseSCD       = Vishala::BinaryPackage::bin2val<bool       >(data);
}