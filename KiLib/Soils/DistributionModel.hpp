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

   class IDistributionModelDecorator: public ISoil
   {
      public:
      virtual DistributionModelType GetDistributionModelType() const = 0;
      virtual const ISoil& GetBaseSoil() const = 0;
   };

   class DistributionModel : public IDistributionModelDecorator
   {
   public:

      DistributionModel(ISoil& s, const DistributionModelType& t) : s(s), distribution_model{t}
      {

      }
      
      DistributionModel(const IDistributionModelDecorator& s): s(s), distribution_model(s.GetDistributionModelType()) 
      {
 
      }

      DistributionModelType GetDistributionModelType() const override
      {
         return this->distribution_model;
      }

      const ISoil& GetBaseSoil() const override
      {
         return s;
      }

      std::string GetName() const override
      {
         return s.GetName();
      }
      std::string GetLongName() const override
      {
         return s.GetLongName();
      }
      double GetPorosity() const override
      {
         return s.GetPorosity();
      }
      double GetSaturatedWaterContent() const override
      {
         return s.GetSaturatedWaterContent();
      }
      double GetResidualWaterContent() const override
      {
         return s.GetResidualWaterContent();
      }
      double GetFieldCapacity() const override
      {
         return s.GetFieldCapacity();
      }
      double GetInitWaterContent() const override
      {
         return s.GetInitWaterContent();
      }
      double GetWaterExchangeTerm() const override
      {
         return s.GetWaterExchangeTerm();
      }
      double GetVgWetAlpha1() const override
      {
         return s.GetVgWetAlpha1();
      }
      double GetVgWetN1() const override
      {
         return s.GetVgWetN1();
      }
      double GetPoreFracMatrix() const override
      {
         return s.GetPoreFracMatrix();
      }
      double GetPoreFracFractures() const override
      {
         return s.GetPoreFracFractures();
      }
      double GetMaxTensileStrain() const override
      {
         return s.GetMaxTensileStrain();
      }
      ValueDistribution GetFrictionAngle() const override
      {
         return s.GetFrictionAngle();
      }
      ValueDistribution GetDensityDry() const override
      {
         return s.GetDensityDry();
      }
      ValueDistribution GetCohesion() const override
      {
         return s.GetCohesion();
      }
      ValueDistribution GetConductivity() const override
      {
         return s.GetConductivity();
      }
      

   protected:
      const ISoil& s;
      DistributionModelType distribution_model;
   };
}       
