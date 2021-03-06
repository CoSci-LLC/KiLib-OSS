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
#include <assert.h>
#define _USE_MATH_DEFINES
#include <cmath>
#include <filesystem>
#include <iostream>
#include <spdlog/spdlog.h>
#include <string>
#include <unordered_map>


namespace KiLib::Soils
{
   class ch : public virtual Soil
   {
   public:
      ch()
      {
         this->name     = "ch";
         this->longname = "Clay of high plasticity, fat clay";

         this->porosity              = 0.56;  // [m3/m3]
         this->saturatedWaterContent = 0.56;  // [m3/m3]
         this->residualWaterContent  = 0.056; // [m3/m3]
         this->fieldCapacity         = 0.47;  // [m3/m3]
         this->initWaterContent      = 0.15;  // [m3/m3]
         this->waterExchangeTerm     = 3e-06; // [1/s]
         this->vgWetAlpha1           = 2e-06; // [1/Pa]
         this->vgWetN1               = 2.0;
         this->poreFracMatrix        = fieldCapacity / porosity;
         this->poreFracFractures     = 1.0 - poreFracMatrix;
         this->maxTensileStrain      = 0; // [Pa]

         this->frictionAngle = {
            .constant = 22 * M_PI / 180.0,
            .uniformPrimula{
               .min = 14 * M_PI / 180.0,
               .max = 24 * M_PI / 180.0,
            },
            .uniform{
               .min = 14 * M_PI / 180.0,
               .max = 14 * M_PI / 180.0,
            },
            .normal{
               .mean   = 22 * M_PI / 180.0,
               .stdDev = 5 * M_PI / 180.0,
            }};

         this->densityDry = {
            .constant = 1140,
            .uniformPrimula{
               .min = 1200.97,
               .max = 1681.36,
            },
            .uniform{
               .min = 900,
               .max = 900,
            },
            .normal{
               .mean   = 1100,
               .stdDev = 200,
            }};

         this->cohesion = {
            .constant = 25000,
            .uniformPrimula{
               .min = 20000,
               .max = 40000,
            },
            .uniform{
               .min = 20000,
               .max = 20000,
            },
            .normal{
               .mean   = 25000,
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
