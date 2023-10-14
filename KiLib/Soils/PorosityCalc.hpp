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

#include <KiLib/Soils/DensityDryGen.hpp>
#include <KiLib/Soils/DistributionModel.hpp>
#include <KiLib/Soils/UserDefined.hpp>


namespace KiLib::Soils
{
   class PorosityCalc : public DistributionModel
   {
   public:
      PorosityCalc(const KiLib::Soils::DensityDryGen &s) : DistributionModel(s), density_gen(s)
      {
      }

      double CalculatePorosity()
      {
         this->porosity           = 1 - density_gen.GetDensityDry() / KiLib::Constants::GRAIN_DENSITY;
         this->has_been_generated = true;
         return this->porosity;
      }

      double GetPorosity() const override
      {
         if (!has_been_generated)
         {
            throw NotGeneratedException("Please calculate Value before calling GetPorosity by using CalculatePorosity");
         }
         return this->porosity;
      }

   private:
      const KiLib::Soils::DensityDryGen &density_gen;
      bool                               has_been_generated{false};
      double                             porosity{0};
   };
} // namespace KiLib::Soils
