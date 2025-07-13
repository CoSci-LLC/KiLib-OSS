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

Stone2008::Stone2008(){};

// clang-format off
double Stone2008::ComputeInfiltration(
   const double rainfall,     // Rainfall rate [L/T]
   const double mu_f) const   // Mean infiltration [L/T]
{
   return mu_f * (1.0 - std::exp(-rainfall/ mu_f));
}
// clang-format on
