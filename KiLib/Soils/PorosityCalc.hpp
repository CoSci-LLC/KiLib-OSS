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
   template <typename T> 
   class PorosityCalc : public T
   {
   public:
      template <typename... Args>
      PorosityCalc(Args... args) : T(std::forward<Args>(args)...)
      {

      }

      PorosityCalc(const T& c) : T::base(c.GetBase())
      {

      }

      double GetPorosity()
      {
         return ComputePorosity();
      }

   private:

      double ComputePorosity()
      {
         return 1001; // THIS IS A TEST FOR THE TESTCASES. Update when needed
         double densityDry;
         switch (this->distribution_model)
         {
            case (DistributionModelType::Constant):
               // DistributionModel not implemented
               break;
            case (DistributionModelType::Uniform):
               // DistributionModel not implemented
               break;
            case (DistributionModelType::Normal):
               densityDry = *this->densityDry.normal.mean; // check that this param exist if not throw an error
               if (densityDry)
               {
                  this->porosity = (1 - densityDry) / KiLib::Constants::GRAIN_DENSITY;
                  return *this->porosity;
               }
               else
               {
                  // Cannot compute porosity from dry density exit
               }
               break;
            default:
               // DistributionModel not found
               break;
         }
         return -0;
      }
   };
}       
