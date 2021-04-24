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
   class cl : public Soil
   {
   public:
      cl()
      {
         this->name     = "cl";
         this->longname = "Clay of low plasticity, lean clay";

         this->porosity              = 0.41;  // [m3/m3]
         this->saturatedWaterContent = 0.41;  // [m3/m3]
         this->residualWaterContent  = 0.041; // [m3/m3]
         this->fieldCapacity         = 0.25;  // [m3/m3]
         this->initWaterContent      = 0.15;  // [m3/m3]
         this->waterExchangeTerm     = 3e-06; // [1/s]
         this->vgWetAlpha1           = 2e-06; // [1/Pa]
         this->vgWetN1               = 2.0;
         this->poreFracMatrix        = fieldCapacity / porosity;
         this->poreFracFractures     = 1.0 - poreFracMatrix;
         this->maxTensileStrain      = 0; // [Pa]

         this->frictionAngle = {
            .constant = 27 * M_PI / 180.0,
            .uniformPrimula{
               .min = 26 * M_PI / 180.0,
               .max = 30 * M_PI / 180.0,
            },
            .uniform{
               .min = 26 * M_PI / 180.0,
               .max = 26 * M_PI / 180.0,
            },
            .normal{
               .mean   = 27 * M_PI / 180.0,
               .stdDev = 2 * M_PI / 180.0,
            }};

         this->densityDry = {
            .constant = 1490,
            .uniformPrimula{
               .min = 1521.23,
               .max = 1921.56,
            },
            .uniform{
               .min = 1300,
               .max = 1300,
            },
            .normal{
               .mean   = 1490,
               .stdDev = 100,
            }};

         this->cohesion = {
            .constant = 20000,
            .uniformPrimula{
               .min = 13000,
               .max = 30000,
            },
            .uniform{
               .min = 13000,
               .max = 13000,
            },
            .normal{
               .mean   = 20000,
               .stdDev = 5000,
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
