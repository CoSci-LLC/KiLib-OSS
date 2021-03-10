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
   class sm : public virtual Soil
   {
   public:
      sm()
      {
         this->name     = "sm";
         this->longname = "Silty sand";

         this->porosity              = 0.37;    // [m3/m3]
         this->saturatedWaterContent = 0.37;    // [m3/m3]
         this->residualWaterContent  = 0.037;   // [m3/m3]
         this->fieldCapacity         = 0.17;    // [m3/m3]
         this->initWaterContent      = 0.1;     // [m3/m3]
         this->waterExchangeTerm     = 3e-06;   // [1/s]
         this->vgWetAlpha1           = 0.00017; // [1/Pa]
         this->vgWetN1               = 2.2;
         this->poreFracMatrix        = fieldCapacity / porosity;
         this->poreFracFractures     = 1.0 - poreFracMatrix;
         this->maxTensileStrain      = 0; // [Pa]

         this->frictionAngle = {
            .constant = 34 * M_PI / 180.0,
            .uniformPrimula{
               .min = 33 * M_PI / 180.0,
               .max = 35 * M_PI / 180.0,
            },
            .uniform{
               .min = 33 * M_PI / 180.0,
               .max = 33 * M_PI / 180.0,
            },
            .normal{
               .mean   = 34 * M_PI / 180.0,
               .stdDev = 1 * M_PI / 180.0,
            }};

         this->densityDry = {
            .constant = 1530,
            .uniformPrimula{
               .min = 1761.43,
               .max = 2001.62,
            },
            .uniform{
               .min = 1430,
               .max = 1430,
            },
            .normal{
               .mean   = 1530,
               .stdDev = 100,
            }};

         this->cohesion = {
            .constant = 500,
            .uniformPrimula{
               .min = 5000,
               .max = 10000,
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
