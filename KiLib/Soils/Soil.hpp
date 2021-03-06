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

#include <string>


namespace KiLib::Soils
{
   // Parameters for a uniform distribution of values
   struct ValueUniform
   {
      double min;
      double max;

      double GetMin() const
      {
         return this->min;
      }
      double GetMax() const
      {
         return this->max;
      }
   };

   // Parameters for a normal distribution of values
   struct ValueNormal
   {
      double mean;
      double stdDev;
      double GetMean() const
      {
         return this->mean;
      }
      double GetStdDev() const
      {
         return this->stdDev;
      }
   };

   // Optional distribution parameters struct
   struct ValueDistribution
   {
      double       constant;
      ValueUniform uniformPrimula;
      ValueUniform uniform;
      ValueNormal  normal;

      ValueUniform GetUniformPrimula() const
      {
         return this->uniformPrimula;
      }
      ValueUniform GetUniform() const
      {
         return this->uniform;
      }
      ValueNormal GetNormal() const
      {
         return this->normal;
      }
   };

   class Soil
   {
   public:
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

      std::string GetName() const
      {
         return this->name;
      }
      std::string GetLongName() const
      {
         return this->longname;
      }

      double GetPorosity() const
      {
         return this->porosity;
      }
      double GetSaturatedWaterContent() const
      {
         return this->saturatedWaterContent;
      }
      double GetResidualWaterContent() const
      {
         return this->residualWaterContent;
      }
      double GetFieldCapacity() const
      {
         return this->fieldCapacity;
      }
      double GetInitWaterContent() const
      {
         return this->initWaterContent;
      }
      double GetWaterExchangeTerm() const
      {
         return this->waterExchangeTerm;
      }
      double GetVgWetAlpha1() const
      {
         return this->vgWetAlpha1;
      }
      double GetVgWetN1() const
      {
         return this->vgWetN1;
      }
      double GetPoreFracMatrix() const
      {
         return this->poreFracMatrix;
      }
      double GetPoreFracFractures() const
      {
         return this->poreFracFractures;
      }
      double GetMaxTensileStrain() const
      {
         return this->maxTensileStrain;
      }

      ValueDistribution GetFrictionAngle() const
      {
         return this->frictionAngle;
      }

      ValueDistribution GetDensityDry() const
      {
         return this->densityDry;
      }

      ValueDistribution GetCohesion() const
      {
         return this->cohesion;
      }

      ValueDistribution GetConductivity() const
      {
         return this->conductivity;
      }
   };
} // namespace KiLib::Soils
