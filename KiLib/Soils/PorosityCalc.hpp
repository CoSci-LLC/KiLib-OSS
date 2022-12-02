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
#include <spdlog/spdlog.h>
#include <KiLib/Soils/DensityDryGen.hpp>


namespace KiLib::Soils
{ 
   class PorosityCalc : public DistributionModel
   {
   public:

      PorosityCalc(const KiLib::Soils::DensityDryGen& s) : DistributionModel(s), density_gen(s)
      {

      }

      double GetPorosity() const override
      {
         SPDLOG_DEBUG("Getting Porosity");
         return ComputePorosity();
      }

   private:
      const KiLib::Soils::DensityDryGen& density_gen;
      double ComputePorosity() const
      {
         double densityDry = density_gen.GetDensityDry();
         return 1 - densityDry / KiLib::Constants::GRAIN_DENSITY;
      }
   };
}       
