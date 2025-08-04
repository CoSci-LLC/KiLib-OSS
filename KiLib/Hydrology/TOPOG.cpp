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

#define _USE_MATH_DEFINES
#include <KiLib/Exceptions/NotImplemented.hpp>
#include <KiLib/Hydrology/Hydrology.hpp>
#include <KiLib/Constants.hpp>
#include <algorithm>
#include <cmath>

using namespace KiLib::Hydrology;

TOPOG::TOPOG(){};

// clang-format off
double TOPOG::ComputeWetness(
   const double rainfall,     // Rainfall intensity [L/T]
   const double A_c,          // Accumulation area [L^2]
   const double ks,           // Hydraulic conductivity [L/T]
   const double thickness,    // Soil thickness [L]
   const double slope_angle,  // Slope angle [rad]
   const double b) const      // Contour length [L]
{
   return std::clamp( (rainfall * A_c) / ( b * ks * thickness * std::tan(slope_angle)), 0.0, 1.0);
}

double TOPOG::ComputeWaterPressure(
   const double thickness,         // Soil thickness [L]
   const double wetness,           // Wetness index [-]
   const double slope_angle) const // Slope angle [rad]
{
   return KiLib::Constants::GRAVITY * KiLib::Constants::WATER_DENSITY * thickness * wetness * std::cos(slope_angle);
}
// clang-format on
