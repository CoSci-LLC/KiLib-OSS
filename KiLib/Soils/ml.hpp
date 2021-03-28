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
   class ml : public Soil
   {
   public:
      ml()
      {
         this->name     = "ml";
         this->longname = "Silt";

         this->porosity              = 0.47;    // [m3/m3]
         this->saturatedWaterContent = 0.47;    // [m3/m3]
         this->residualWaterContent  = 0.047;   // [m3/m3]
         this->fieldCapacity         = 0.3;     // [m3/m3]
         this->initWaterContent      = 0.15;    // [m3/m3]
         this->waterExchangeTerm     = 3e-06;   // [1/s]
         this->vgWetAlpha1           = 0.00013; // [1/Pa]
         this->vgWetN1               = 1.46;
         this->poreFracMatrix        = fieldCapacity / porosity;
         this->poreFracFractures     = 1.0 - poreFracMatrix;
         this->maxTensileStrain      = 0; // [Pa]

         this->frictionAngle = {
            .constant = 33 * M_PI / 180.0,
            .uniformPrimula{
               .min = 30 * M_PI / 180.0,
               .max = 34 * M_PI / 180.0,
            },
            .uniform{
               .min = 30 * M_PI / 180.0,
               .max = 30 * M_PI / 180.0,
            },
            .normal{
               .mean   = 33 * M_PI / 180.0,
               .stdDev = 2 * M_PI / 180.0,
            }};

         this->densityDry = {
            .constant = 1430,
            .uniformPrimula{
               .min = 1521.23,
               .max = 1921.56,
            },
            .uniform{
               .min = 1300,
               .max = 1300,
            },
            .normal{
               .mean   = 1430,
               .stdDev = 100,
            }};

         this->cohesion = {
            .constant = 500,
            .uniformPrimula{
               .min = 10000,
               .max = 15000,
            },
            .uniform{
               .min = 0,
               .max = 0,
            },
            .normal{
               .mean   = 500,
               .stdDev = 250,
            }};

         this->conductivity = {
            .constant = 0.0005,
            .uniformPrimula{
               .min = 1e-06,
               .max = 0.001,
            }};
      }
   };
} // namespace KiLib::Soils
