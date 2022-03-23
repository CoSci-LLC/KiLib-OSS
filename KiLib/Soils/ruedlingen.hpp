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
   class ruedlingen : public Soil
   {
   public:
      ruedlingen()
      {
         this->name                  = "ruedlingen";
         this->longname              = "Ruedlingen soil";
         this->porosity              = 0.47;   // [m3/m3]
         this->saturatedWaterContent = 0.47;   // [m3/m3]
         this->residualWaterContent  = 0.0454; // [m3/m3]
         this->fieldCapacity         = 0.3632; // [m3/m3]
         this->initWaterContent      = 0.20;   // [m3/m3]
         this->waterExchangeTerm     = 3e-06;  // [1/s]
         this->vgWetAlpha1           = 1.3e-4; // [1/Pa]
         this->vgWetN1               = 1.46;   // [-]
         this->maxTensileStrain      = 0.00;   // [Pa]

         this->frictionAngle = {               // [rad]
            .constant = 30 * M_PI / 180.0,
            .uniformPrimula{
               .min = 30 * M_PI / 180.0,
               .max = 30 * M_PI / 180.0,
            },
            .uniform{
               .min = 30 * M_PI / 180.0,
               .max = 30 * M_PI / 180.0,
            },
            .normal{
               .mean   = 30 * M_PI / 180.0,
               .stdDev = 1 * M_PI / 180.0,
            }};

         this->densityDry = {                  // [kg/m^3]
            .constant = 1633,
            .uniformPrimula{
               .min = 1633,
               .max = 1633,
            },
            .uniform{
               .min = 1633,
               .max = 1633,
            },
            .normal{
               .mean   = 1633,
               .stdDev = 100,
            }};

         this->cohesion = {                    // [Pa]
            .constant = 500,
            .uniformPrimula{
               .min = 0,
               .max = 500,
            },
            .uniform{
               .min = 0,
               .max = 500,
            },
            .normal{
               .mean   = 500,
               .stdDev = 50,
            }};

         this->conductivity = {                // [m/s]
            .constant = 0.02,
            .uniformPrimula{
               .min = 0.01,
               .max = 0.03,
            }};
      }
   };
} // namespace KiLib::Soils
