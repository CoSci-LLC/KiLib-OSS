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

#define _USE_MATH_DEFINES
#include <KiLib/Soils/Soil.hpp>
#include <assert.h>
#include <cmath>
#include <filesystem>
#include <iostream>
#include <spdlog/spdlog.h>
#include <string>
#include <unordered_map>


namespace KiLib::Soils
{
   class sp : public virtual Soil
   {
   public:
      sp()
      {
         this->name     = "sp";
         this->longname = "Poorly graded sand";

         this->porosity              = 0.38;    // [m3/m3]
         this->saturatedWaterContent = 0.38;    // [m3/m3]
         this->residualWaterContent  = 0.038;   // [m3/m3]
         this->fieldCapacity         = 0.11;    // [m3/m3]
         this->initWaterContent      = 0.05;    // [m3/m3]
         this->waterExchangeTerm     = 3e-06;   // [1/s]
         this->vgWetAlpha1           = 0.00051; // [1/Pa]
         this->vgWetN1               = 1.7;
         this->poreFracMatrix        = fieldCapacity / porosity;
         this->poreFracFractures     = 1.0 - poreFracMatrix;
         this->maxTensileStrain      = 0; // [Pa]

         this->frictionAngle = {
            .constant = 36 * M_PI / 180.0,
            .uniformPrimula{
               .min = 36 * M_PI / 180.0,
               .max = 38 * M_PI / 180.0,
            },
            .uniform{
               .min = 35 * M_PI / 180.0,
               .max = 35 * M_PI / 180.0,
            },
            .normal{
               .mean   = 36 * M_PI / 180.0,
               .stdDev = 1 * M_PI / 180.0,
            }};

         this->densityDry = {
            .constant = 1470,
            .uniformPrimula{
               .min = 1601.3,
               .max = 1921.56,
            },
            .uniform{
               .min = 1420,
               .max = 1420,
            },
            .normal{
               .mean   = 1470,
               .stdDev = 50,
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
            .constant = 0.0005,
            .uniformPrimula{
               .min = 0.0001,
               .max = 0.001,
            }};
      }
   };
} // namespace KiLib::Soils
