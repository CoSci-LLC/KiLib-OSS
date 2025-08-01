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
   class sc : public Soil
   {
   public:
      sc()
      {
         this->name     = "sc";
         this->longname = "Clayey sand";

         this->porosity              = 0.4;    // [m3/m3]
         this->saturatedWaterContent = 0.4;    // [m3/m3]
         this->residualWaterContent  = 0.04;   // [m3/m3]
         this->fieldCapacity         = 0.2;    // [m3/m3]
         this->initWaterContent      = 0.15;   // [m3/m3]
         this->waterExchangeTerm     = 3e-06;  // [1/s]
         this->vgWetAlpha1           = 0.0002; // [1/Pa]
         this->vgWetN1               = 2.0;
         this->maxTensileStrain      = 0;      // [Pa]
         this->matrixCompressibility = 5e-09;  // [1/Pa] (ChatGPT 2025-08-01)

         this->frictionAngle = {
            .constant = 32 * M_PI / 180.0,
            .uniformPrimula{
               .min = 27 * M_PI / 180.0,
               .max = 35 * M_PI / 180.0,
            },
            .uniform{
               .min = 31 * M_PI / 180.0,
               .max = 31 * M_PI / 180.0,
            },
            .normal{
               .mean   = 32 * M_PI / 180.0,
               .stdDev = 1 * M_PI / 180.0,
            }};

         this->densityDry = {
            .constant = 1500,
            .uniformPrimula{
               .min = 1681.36,
               .max = 2001.62,
            },
            .uniform{
               .min = 1400,
               .max = 1400,
            },
            .normal{
               .mean   = 1500,
               .stdDev = 100,
            }};

         this->cohesion = {
            .constant = 500,
            .uniformPrimula{
               .min = 5000,
               .max = 11000,
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
            .constant = 5e-05,
            .uniformPrimula{
               .min = 1e-08,
               .max = 1e-06,
            }};
      }
   };
} // namespace KiLib::Soils
