/**
 *  Copyright (c) 2020-2021 CoSci LLC, USA <software@cosci-llc.com>
 *
 *  This file is part of KiLib-OSS.
 *
 *  KiLib-OSS is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  KiLib-OSS is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with KiLib-OSS.  If not, see <https://www.gnu.org/licenses/>.
 */


#pragma once

#include <assert.h>
#define _USE_MATH_DEFINES
#include <cmath>
#include <filesystem>
#include <iostream>
#include <spdlog/spdlog.h>
#include <string>
#include <unordered_map>


namespace KiLib
{
   class SoilType
   {
   private:
      static std::unordered_map<std::string, const SoilType> soilTypes;

      static bool loadFromFile(std::string path);

   public:
      // Parameters for a uniform distribution of values
      struct ValueUniform
      {
         double min;
         double max;
      };

      // Parameters for a normal distribution of values
      struct ValueNormal
      {
         double mean;
         double stdDev;
      };

      // Optional distribution parameters struct
      struct ValueDistribution
      {
         double       constant;
         ValueUniform uniformPrimula;
         ValueUniform uniform;
         ValueNormal  normal;
      };

      std::string       name;
      std::string       longname;
      double            porosity;
      double            saturatedWaterContent;
      double            residualWaterContent;
      double            fieldCapacity;
      double            initWaterContent;
      double            waterExchangeTerm;
      double            vgWetAlpha1;
      double            vgWetN1;
      ValueDistribution frictionAngle;
      double            poreFracMatrix;
      double            poreFracFractures;
      ValueDistribution densityDry;
      double            maxTensileStrain;
      ValueDistribution cohesion;
      ValueDistribution conductivity;

      /**
       * @brief Attempts to acquire soiltype with given name or path, loading it if it has not been loaded
       *
       * @param nameOrPath Name of included KiLib soil type, or path to custom soil type file
       * @return const SoilType&
       */
      static const SoilType &get(std::string nameOrPath);
   };

} // namespace KiLib
