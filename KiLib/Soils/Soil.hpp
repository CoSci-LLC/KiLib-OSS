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

#include <KiLib/Soils/Soils.hpp>
#include <KiLib/Utils/AbstractFactory.hpp>
#include <KiLib/Utils/enum.hpp>
#include <string>

namespace KiLib::Soils
{
   BETTER_ENUM(SoilID, int, ch, cl, gc, gm, gp, gw, mh, ml, oh, ol, sc, sm, sp, sw)

   // Parameters for a uniform distribution of values
   struct ValueUniform
   {
      double min;
      double max;
      double GetMin() const;
      double GetMax() const;
   };

   // Parameters for a normal distribution of values
   struct ValueNormal
   {
      double mean;
      double stdDev;
      double GetMean() const;
      double GetStdDev() const;
   };

   // Optional distribution parameters struct
   struct ValueDistribution
   {
      double       constant;
      ValueUniform uniformPrimula;
      ValueUniform uniform;
      ValueNormal  normal;
      ValueUniform GetUniformPrimula() const;
      ValueUniform GetUniform() const;
      ValueNormal  GetNormal() const;
   };

   class Soil
   {
   public:
      static const AbsFac<SoilID, Soil> factory;

      std::string name;
      std::string longname;

      double porosity;
      double saturatedWaterContent;
      double residualWaterContent;
      double fieldCapacity;
      double initWaterContent;
      double waterExchangeTerm;
      double vgWetAlpha1;
      double vgWetN1;
      double poreFracMatrix;
      double poreFracFractures;
      double maxTensileStrain;

      ValueDistribution frictionAngle;
      ValueDistribution densityDry;
      ValueDistribution cohesion;
      ValueDistribution conductivity;

      std::string GetName() const;
      std::string GetLongName() const;

      double GetPorosity() const;
      double GetSaturatedWaterContent() const;
      double GetResidualWaterContent() const;
      double GetFieldCapacity() const;
      double GetInitWaterContent() const;
      double GetWaterExchangeTerm() const;
      double GetVgWetAlpha1() const;
      double GetVgWetN1() const;
      double GetPoreFracMatrix() const;
      double GetPoreFracFractures() const;
      double GetMaxTensileStrain() const;

      ValueDistribution GetFrictionAngle() const;
      ValueDistribution GetDensityDry() const;
      ValueDistribution GetCohesion() const;
      ValueDistribution GetConductivity() const;
   };
} // namespace KiLib::Soils
