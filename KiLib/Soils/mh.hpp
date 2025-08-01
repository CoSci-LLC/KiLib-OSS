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
   class mh : public Soil
   {
   public:
      mh()
      {
         this->name     = "mh";
         this->longname = "Silt of high plasticity, elastic silt";

         this->porosity              = 0.76;  // [m3/m3]
         this->saturatedWaterContent = 0.76;  // [m3/m3]
         this->residualWaterContent  = 0.076; // [m3/m3]
         this->fieldCapacity         = 0.75;  // [m3/m3]
         this->initWaterContent      = 0.15;  // [m3/m3]
         this->waterExchangeTerm     = 3e-06; // [1/s]
         this->vgWetAlpha1           = 1e-05; // [1/Pa]
         this->vgWetN1               = 3.0;
         this->maxTensileStrain      = 0; // [Pa]
         this->matrixCompressibility = 5e-07; // [1/Pa] (ChatGPT 2025-08-01)

         this->frictionAngle = {
            .constant = 24 * M_PI / 180.0,
            .uniformPrimula{
               .min = 22 * M_PI / 180.0,
               .max = 28 * M_PI / 180.0,
            },
            .uniform{
               .min = 22 * M_PI / 180.0,
               .max = 22 * M_PI / 180.0,
            },
            .normal{
               .mean   = 24 * M_PI / 180.0,
               .stdDev = 3 * M_PI / 180.0,
            }};

         this->densityDry = {
            .constant = 800,
            .uniformPrimula{
               .min = 1120.91,
               .max = 1521.23,
            },
            .uniform{
               .min = 700,
               .max = 700,
            },
            .normal{
               .mean   = 900,
               .stdDev = 200,
            }};

         this->cohesion = {
            .constant = 5000,
            .uniformPrimula{
               .min = 20000,
               .max = 40000,
            },
            .uniform{
               .min = 2000,
               .max = 2000,
            },
            .normal{
               .mean   = 5000,
               .stdDev = 3000,
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
