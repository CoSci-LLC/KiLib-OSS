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
#include <algorithm>
#include <cmath>

using namespace KiLib::Hydrology;

TopModel::TopModel(){};

// clang-format off
double TopModel::ComputeWetness(
   const double rainfall,   // Rainfall intensity [L/T]
   const double ks,         // Hydraulic conductivity [L/T]
   const double thickness,  // Soil thickness [L]
   const double twi) const  // Topographic Wetness Index [-]
{
   return std::clamp(rainfall / (ks * thickness) * std::exp(twi), 0.0, 1.0);
}
// clang-format on
