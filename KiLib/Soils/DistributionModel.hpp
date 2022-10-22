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

#include <KiLib/Soils/Soil.hpp>

namespace KiLib::Soils
{
   enum class DistributionModelType : int { Constant, Uniform, Normal };
   template <class T>
   class DistributionModel : public T
   {
      static_assert(std::is_base_of<Soil, T>::value, "Invalid template argument"); // Compile time safety
   public:
      template <typename... Args>
      DistributionModel(const DistributionModelType t, Args... args) : T(std::forward<Args>(args)...), base(*this), distribution_model{t}
      {}

      DistributionModel(const T& c, const DistributionModelType t) : base(c), distribution_model{t}
      {
         
      }

      std::string GetName() const { return base.GetName(); }
      std::string GetLongName() const;

      double GetPorosity() const { return base.GetPorosity(); }
      double GetSaturatedWaterContent() const { return base.GetSaturatedWaterContent(); }
      double GetResidualWaterContent() const { return base.GetResidualWaterContent(); }
      double GetFieldCapacity() const { return base.GetFieldCapacity(); }
      double GetInitWaterContent() const { return base.GetInitWaterContent(); }
      double GetWaterExchangeTerm() const { return base.GetWaterExchangeTerm(); }
      double GetVgWetAlpha1() const { return base.GetVgWetAlpha1(); }
      double GetVgWetN1() const { return base.GetVgWetN1(); }
      double GetPoreFracMatrix() const { return base.GetPoreFracMatrix(); }
      double GetPoreFracFractures() const { return base.GetPoreFracFractures(); }
      double GetMaxTensileStrain() const { return base.GetMaxTensileStrain(); }

      ValueDistribution GetFrictionAngle() const { return base.GetFrictionAngle(); }
      ValueDistribution GetDensityDry() const { return base.GetDensityDry(); }
      ValueDistribution GetCohesion() const { return base.GetCohesion(); }
      ValueDistribution GetConductivity() const { return base.GetConductivity(); }
      
      DistributionModelType GetDistributionModelType() const { return distribution_model; }


      const T GetBase() const { return base; }
   protected:
      const T& base;
      DistributionModelType distribution_model;
   };
}       
