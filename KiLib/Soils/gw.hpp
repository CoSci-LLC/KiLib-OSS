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
   class gw : public Soil
   {
   public:
      gw()
      {
         this->name     = "gw";
         this->longname = "Well-graded gravel, fine to coarse gravel";

         this->porosity              = 0.3;    // [m3/m3]
         this->saturatedWaterContent = 0.3;    // [m3/m3]
         this->residualWaterContent  = 0.01;   // [m3/m3]
         this->fieldCapacity         = 0.05;   // [m3/m3]
         this->initWaterContent      = 0.02;   // [m3/m3]
         this->waterExchangeTerm     = 3e-06;  // [1/s]
         this->vgWetAlpha1           = 0.0001; // [1/Pa]
         this->vgWetN1               = 3.3;
         this->maxTensileStrain      = 0; // [Pa]

         this->frictionAngle = {
            .constant = 40 * M_PI / 180.0,
            .uniformPrimula{
               .min = 38 * M_PI / 180.0,
               .max = 40 * M_PI / 180.0,
            },
            .uniform{
               .min = 38 * M_PI / 180.0,
               .max = 38 * M_PI / 180.0,
            },
            .normal{
               .mean   = 40 * M_PI / 180.0,
               .stdDev = 2 * M_PI / 180.0,
            }};

         this->densityDry = {
            .constant = 1800,
            .uniformPrimula{
               .min = 2001.62,
               .max = 2161.75,
            },
            .uniform{
               .min = 1700,
               .max = 1700,
            },
            .normal{
               .mean   = 1800,
               .stdDev = 100,
            }};

         this->cohesion = {
            .constant = 500,
            .uniformPrimula{
               .min = 0,
               .max = 0,
            },
            .uniform{
               .min = 400,
               .max = 400,
            },
            .normal{
               .mean   = 500,
               .stdDev = 50,
            }};

         this->conductivity = {
            .constant = 0.02,
            .uniformPrimula{
               .min = 0.01,
               .max = 0.05,
            }};
      }
   };
} // namespace KiLib::Soils
