#include <KiLib/Soils/SoilType.hpp>
#include <yaml-cpp/yaml.h>


namespace KiLib
{

   namespace fs = std::filesystem;
   std::unordered_map<std::string, const SoilType> SoilType::soilTypes;

   double optionalParam(YAML::Node node, double alternate)
   {
      return node.IsDefined() ? node.as<double>() : alternate;
   }

   // Loads soiltype from given path, returning success
   bool SoilType::loadFromFile(std::string path)
   {
      if (fs::exists(path) == false)
      {
         return false;
      }

      YAML::Node root = YAML::LoadFile(path);
      SoilType   s;
      s.name                  = fs::path(path).stem().string();
      s.longname              = root["longname"].as<std::string>();
      s.porosity              = root["porosity"].as<double>();
      s.saturatedWaterContent = root["saturatedWaterContent"].as<double>();
      s.residualWaterContent  = root["residualWaterContent"].as<double>();
      s.fieldCapacity         = root["fieldCapacity"].as<double>();
      s.initWaterContent      = root["initWaterContent"].as<double>();
      s.waterExchangeTerm     = root["waterExchangeTerm"].as<double>();
      s.vgWetAlpha1           = root["vgWetAlpha1"].as<double>();
      s.vgWetN1               = root["vgWetN1"].as<double>();
      s.frictionAngle         = {
         .constant = root["frictionAngle"]["constant"].as<double>() * M_PI / 180.0,
         .uniformPrimula =
            {.min = root["frictionAngle"]["uniform-primula"]["min"].as<double>() * M_PI / 180.0,
             .max = root["frictionAngle"]["uniform-primula"]["max"].as<double>() * M_PI / 180.0},
         .uniform =
            {.min = root["frictionAngle"]["uniform"]["min"].as<double>() * M_PI / 180.0,
             .max = root["frictionAngle"]["uniform"]["max"].as<double>() * M_PI / 180.0},
         .normal = {
            .mean   = root["frictionAngle"]["normal"]["mean"].as<double>() * M_PI / 180.0,
            .stdDev = root["frictionAngle"]["normal"]["stdDev"].as<double>() * M_PI / 180.0}};

      s.poreFracMatrix    = optionalParam(root["poreFracMatrix"], s.fieldCapacity / s.porosity);
      s.poreFracFractures = optionalParam(root["poreFracFractures"], 1.0 - s.poreFracMatrix);

      s.densityDry = {
         .constant = root["densityDry"]["constant"].as<double>(),
         .uniformPrimula =
            {.min = root["densityDry"]["uniform-primula"]["min"].as<double>(),
             .max = root["densityDry"]["uniform-primula"]["max"].as<double>()},
         .uniform =
            {.min = root["densityDry"]["uniform"]["min"].as<double>(),
             .max = root["densityDry"]["uniform"]["max"].as<double>()},
         .normal = {
            .mean   = root["densityDry"]["normal"]["mean"].as<double>(),
            .stdDev = root["densityDry"]["normal"]["stdDev"].as<double>()}};

      s.maxTensileStrain = root["maxTensileStrain"].as<double>();

      s.cohesion = {
         .constant = root["cohesion"]["constant"].as<double>(),
         .uniformPrimula =
            {.min = root["cohesion"]["uniform-primula"]["min"].as<double>(),
             .max = root["cohesion"]["uniform-primula"]["max"].as<double>()},
         .uniform =
            {.min = root["cohesion"]["uniform"]["min"].as<double>(),
             .max = root["cohesion"]["uniform"]["max"].as<double>()},
         .normal = {
            .mean   = root["cohesion"]["normal"]["mean"].as<double>(),
            .stdDev = root["cohesion"]["normal"]["stdDev"].as<double>()}};

      s.conductivity = {
         .constant       = root["conductivity"]["constant"].as<double>(),
         .uniformPrimula = {
            .min = root["conductivity"]["uniform-primula"]["min"].as<double>(),
            .max = root["conductivity"]["uniform-primula"]["max"].as<double>()}};

      soilTypes.emplace(s.name, s);
      soilTypes.emplace(path, s);

      return true;
   }

   // Returns a reference to the soiltype of given name OR at given path
   const SoilType &SoilType::get(std::string nameOrPath)
   {
      // Check if its in under current name or a KiLib path
      SPDLOG_DEBUG("KiLib Resource Path:" + std::string(KILIB_RESOURCE_PATH));
      std::string KiLibPath = std::string(KILIB_RESOURCE_PATH) + "/soiltypes/" + nameOrPath + ".yaml";
      if (soilTypes.count(nameOrPath) > 0)
      {
         return soilTypes.at(nameOrPath);
      }
      if (soilTypes.count(KiLibPath) > 0)
      {
         return soilTypes.at(KiLibPath);
      }

      // Try to load it in as a KiLib file
      if (loadFromFile(KiLibPath) == true)
      {
         return soilTypes.at(KiLibPath);
      }

      // Try to load it in as a user provided path
      if (loadFromFile(nameOrPath) == true)
      {
         return soilTypes.at(nameOrPath);
      }

      // We cant find it, throw an exception
      throw std::runtime_error("Could not fine SoilType, check name or path!");
   }
} // namespace KiLib
