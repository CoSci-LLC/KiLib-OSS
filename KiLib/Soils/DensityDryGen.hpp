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
#include <spdlog/spdlog.h>
#include <KiLib/Soils/DistributionModel.hpp>
#include <KiLib/Exceptions/NotImplemented.hpp>
#include <KiLib/Exceptions/NotGenerated.hpp>

namespace KiLib::Soils
{ 
   class DensityDryGen : public DistributionModel
   {
   public:

      DensityDryGen(const IDistributionModelDecorator& s, std::mt19937_64 &gen) : DistributionModel(s), gen(gen)
      {
      }

      double GenerateDensityDry()
      {
         double min, max, mean, sd;
         switch (GetDistributionModelType())
         {
            case (DistributionModelType::Constant):
               this->densityDry         = GetBaseSoil().GetDensityDryDistribution().GetConstant();
               this->has_been_generated = true;               
               break;

            case (DistributionModelType::Uniform):
               min                      = GetBaseSoil().GetDensityDryDistribution().GetUniform().GetMin();
               max                      = GetBaseSoil().GetDensityDryDistribution().GetUniform().GetMax();
               this->densityDry         = KiLib::Random::runif(1, min, max, gen)[0];
               this->has_been_generated = true;               
               break;

            case (DistributionModelType::Normal):
               mean            = GetBaseSoil().GetDensityDryDistribution().GetNormal().GetMean();
               sd              = GetBaseSoil().GetDensityDryDistribution().GetNormal().GetStdDev();
               if (sd == 0.0)
               {
                  this->densityDry = mean;
               }
               else
               {
                  auto logValues   = KiLib::Random::TransformNormalToLogNormal(mean, sd);
                  this->densityDry = KiLib::Random::rlnorm(1, logValues.first, logValues.second, gen)[0];
               }
               this->has_been_generated = true;               
               break;

            default:
               throw NotImplementedException("Unknown Distribution Type for DensityDryGen");
         }
         return densityDry;        
      }

      double GetDensityDry() const override
      {
         if (!has_been_generated) 
         {
            throw NotGeneratedException("Please Generate Value before calling GetDensityDry by using GenerateDensityDry");
         }
         return densityDry;
      }

   private:
      std::mt19937_64 &gen;
      bool             has_been_generated{false};
      double           densityDry{0.0};
   };
}       
