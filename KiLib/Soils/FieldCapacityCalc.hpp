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
   class FieldCapacityCalc : public DistributionModel
   {
   public:
      FieldCapacityCalc(const IDistributionModelDecorator& s) : DistributionModel(s)
      {
         
      }

      double CalculateFieldCapacity(const double & P)
      {
         const auto t_r = s.GetResidualWaterContent();
         const auto t_s = s.GetPorosity();
         const auto n   = s.GetVgWetN1();
         const auto alpha = s.GetVgWetAlpha1();
         fieldCapacity = t_r + (t_s - t_r) / pow(1 + pow(alpha * std::fabs(P),n), 1-1/n);
         has_been_calculated = true;
         return fieldCapacity;
      }

      double GetFieldCapacity() const override
      {
         return fieldCapacity;
      }

   private:
      bool has_been_calculated{false};
      double fieldCapacity{0};
   };
}       
