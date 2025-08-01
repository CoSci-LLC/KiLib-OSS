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
   class ol : public Soil
   {
   public:
      ol()
      {
         this->name     = "ol";
         this->longname = "Organic silt, organic clay";

         this->porosity              = 0.57;  // [m3/m3]
         this->saturatedWaterContent = 0.57;  // [m3/m3]
         this->residualWaterContent  = 0.057; // [m3/m3]
         this->fieldCapacity         = 0.48;  // [m3/m3]
         this->initWaterContent      = 0.15;  // [m3/m3]
         this->waterExchangeTerm     = 3e-06; // [1/s]
         this->vgWetAlpha1           = 1e-05; // [1/Pa]
         this->vgWetN1               = 3.0;
         this->maxTensileStrain      = 0; // [Pa]
         this->matrixCompressibility = 1e-05; // [1/Pa] (ChatGPT 2025-08-01)

         this->frictionAngle = {
            .constant = 25 * M_PI / 180.0,
            .uniformPrimula{
               .min = 26 * M_PI / 180.0,
               .max = 30 * M_PI / 180.0,
            },
            .uniform{
               .min = 26 * M_PI / 180.0,
               .max = 26 * M_PI / 180.0,
            },
            .normal{
               .mean   = 25 * M_PI / 180.0,
               .stdDev = 2 * M_PI / 180.0,
            }};

         this->densityDry = {
            .constant = 1130,
            .uniformPrimula{
               .min = 1281.04,
               .max = 1601.3,
            },
            .uniform{
               .min = 1000,
               .max = 1000,
            },
            .normal{
               .mean   = 1130,
               .stdDev = 100,
            }};

         this->cohesion = {
            .constant = 10000,
            .uniformPrimula{
               .min = 15000,
               .max = 30000,
            },
            .uniform{
               .min = 13000,
               .max = 13000,
            },
            .normal{
               .mean   = 10000,
               .stdDev = 5000,
            }};

         this->conductivity = {
            .constant = 5e-05,
            .uniformPrimula{
               .min = 1e-06,
               .max = 0.0001,
            }};
      }
   };
} // namespace KiLib::Soils
