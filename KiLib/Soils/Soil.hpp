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

   class ISoil 
   {
   public:
      virtual std::string GetName() const = 0;
      virtual std::string GetLongName() const = 0;

      virtual double GetPorosity() const = 0;
      virtual double GetSaturatedWaterContent() const = 0;
      virtual double GetResidualWaterContent() const = 0;
      virtual double GetFieldCapacity() const = 0;
      virtual double GetInitWaterContent() const = 0;
      virtual double GetWaterExchangeTerm() const = 0;
      virtual double GetVgWetAlpha1() const = 0;
      virtual double GetVgWetN1() const = 0;
      virtual double GetPoreFracMatrix() const = 0;
      virtual double GetPoreFracFractures() const = 0;
      virtual double GetMaxTensileStrain() const = 0;

      virtual ValueDistribution GetFrictionAngle() const = 0;
      virtual ValueDistribution GetDensityDry() const = 0;
      virtual ValueDistribution GetCohesion() const = 0;
      virtual ValueDistribution GetConductivity() const = 0;
   };

   class Soil : public ISoil
   {
   public:
      std::string GetName() const override;
      std::string GetLongName() const override;

      double GetPorosity() const override;
      double GetSaturatedWaterContent() const override;
      double GetResidualWaterContent() const override;
      double GetFieldCapacity() const override;
      double GetInitWaterContent() const override;
      double GetWaterExchangeTerm() const override;
      double GetVgWetAlpha1() const override;
      double GetVgWetN1() const override;
      double GetPoreFracMatrix() const override;
      double GetPoreFracFractures() const override;
      double GetMaxTensileStrain() const override;

      ValueDistribution GetFrictionAngle() const override;
      ValueDistribution GetDensityDry() const override;
      ValueDistribution GetCohesion() const override;
      ValueDistribution GetConductivity() const override;

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
