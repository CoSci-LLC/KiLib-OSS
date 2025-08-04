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

Iverson2000STST::Iverson2000STST(){};

// clang-format off
double Iverson2000STST::ComputeWaterPressure(
   const double Ks, 
   const double Ss, 
   const double rainfall, 
   const double rainfallLT,
   const double duration,
   const double soil_depth,
   const double water_table_depth,
   const double slope_angle) const
// clang-format on
{
   const double inf   = std::min(rainfall, Ks);
   const double infLT = std::min(rainfallLT, Ks);
   const double D0    = Ks / Ss;
   const double cos2  = std::pow(std::cos(slope_angle), 2);
   const double Dhat  = 4.0 * D0 * cos2;
   const double beta  = cos2 - infLT / Ks;
   const double Tstar = duration / std::pow(soil_depth, 2) * Dhat;
   const double R     = ComputePressureHeadResponseFunction(Tstar);
   return KiLib::Constants::WATER_DENSITY * KiLib::Constants::GRAVITY * (beta * (soil_depth - water_table_depth) + soil_depth * inf / Ks * R);
}


double Iverson2000STST::ComputePressureHeadResponseFunction(const double Tstar) const
{
   return std::sqrt(Tstar/M_PI) * std::exp(-1.0/Tstar) - std::erfc(1/std::sqrt(Tstar));
}
