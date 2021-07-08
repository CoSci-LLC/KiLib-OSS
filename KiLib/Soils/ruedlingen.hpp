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
         this->name     = "ruedlingen";
         this->longname = "Ruedlingen soil";

         this->porosity              = 0.47;  // [m3/m3]
         this->saturatedWaterContent = 0.47;  // [m3/m3]
         this->residualWaterContent  = 0.047; // [m3/m3]
         this->fieldCapacity         = 0.30;  // [m3/m3]
         this->initWaterContent      = 0.20;  // [m3/m3]
         this->waterExchangeTerm     = 3e-06; // [1/s]
         this->vgWetAlpha1           = 1.3e-4; // [1/Pa]
         this->vgWetN1               = 1.46;
         this->poreFracMatrix        = fieldCapacity / porosity;
         this->poreFracFractures     = 1.0 - poreFracMatrix;
         this->maxTensileStrain      = 0; // [Pa]

         this->frictionAngle = {
            .constant = 29 * M_PI / 180.0,
            .uniformPrimula{
               .min = 29 * M_PI / 180.0,
               .max = 29 * M_PI / 180.0,
            },
            .uniform{
               .min = 29 * M_PI / 180.0,
               .max = 29 * M_PI / 180.0,
            },
            .normal{
               .mean   = 29 * M_PI / 180.0,
               .stdDev = 1 * M_PI / 180.0,
            }};

         this->densityDry = {
            .constant = 1700,
            .uniformPrimula{
               .min = 1700,
               .max = 1700,
            },
            .uniform{
               .min = 1700,
               .max = 1700,
            },
            .normal{
               .mean   = 1700,
               .stdDev = 100,
            }};

         this->cohesion = {
            .constant = 0,
            .uniformPrimula{
               .min = 0,
               .max = 500,
            },
            .uniform{
               .min = 0,
               .max = 500,
            },
            .normal{
               .mean   = 250,
               .stdDev = 100,
            }};

         this->conductivity = {
            .constant = 5e-07,
            .uniformPrimula{
               .min = 1e-08,
               .max = 1e-06,
            }};
      }
   };
} // namespace KiLib::Soils
