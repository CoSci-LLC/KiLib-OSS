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

#include <KiLib/Soils/UserDefined.hpp>
#include <KiLib/Soils/DistributionModel.hpp>


namespace KiLib::Soils
{
   class DensityWetCalc : public DistributionModel
   {
   public:
      DensityWetCalc(const IDistributionModelDecorator& s) : DistributionModel(s)
      {

      }

      double CalculateDensityWet(const double & S)
      {
         const auto poro           = s.GetPorosity();
         const auto fc             = s.GetFieldCapacity();
         this->densityWet          = (1 - poro) * KiLib::Constants::GRAIN_DENSITY + (S * poro + (1-S) * fc) * KiLib::Constants::WATER_DENSITY;
         this->has_been_calculated = true;
         return this->densityWet;
      }

      double GetDensityWet() const override
      {
         return this->densityWet;
      }

   private:
      bool   has_been_calculated{false};
      double densityWet{0};
   };
}       
