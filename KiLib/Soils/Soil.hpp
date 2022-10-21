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

#define _USE_MATH_DEFINES
#include <KiLib/Utils/SafeDouble.hpp>
#include <cmath>
#include <optional>
#include <string>

namespace KiLib::Soils
{
   // Parameters for a uniform distribution of values
   struct ValueUniform
   {
      double GetMin() const;
      double GetMax() const;

      std::optional<double> min;
      std::optional<double> max;
   };

   // Parameters for a normal distribution of values
   struct ValueNormal
   {
      double GetMean() const;
      double GetStdDev() const;

      std::optional<double> mean;
      std::optional<double> stdDev;
   };

   // Optional distribution parameters struct
   struct ValueDistribution
   {
      ValueUniform GetUniformPrimula() const;
      ValueUniform GetUniform() const;
      ValueNormal  GetNormal() const;
      double       GetConstant() const;

      std::optional<double> constant;
      ValueUniform          uniformPrimula;
      ValueUniform          uniform;
      ValueNormal           normal;
   };

   class Soil
   {
   public:
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

      enum class DistributionModel : int { Constant, Uniform, Normal, LogNormal };
      void ComputePorosity(DistributionModel &distamodel);
      void ComputeDensityWet(const double &saturation, DistributionModel &distModel);
      void ComputeFieldCapacity(DistributionModel &distModel);

   protected:
      std::string name;
      std::string longname;

      // Use SafeDouble so that if a value is called and not set, it throws an
      // exception
      std::optional<double> porosity;
      std::optional<double> saturatedWaterContent;
      std::optional<double> residualWaterContent;
      std::optional<double> fieldCapacity;
      std::optional<double> initWaterContent;
      std::optional<double> waterExchangeTerm;
      std::optional<double> vgWetAlpha1;
      std::optional<double> vgWetN1;
      std::optional<double> maxTensileStrain;

      ValueDistribution frictionAngle;
      ValueDistribution densityDry;
      ValueDistribution cohesion;
      ValueDistribution conductivity;
   };
} // namespace KiLib::Soils
