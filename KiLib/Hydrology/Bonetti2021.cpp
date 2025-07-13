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

Bonetti2021::Bonetti2021(){};

// clang-format off
double Bonetti2021::ComputeInfiltration(
   const double rainfall,     // Rainfall rate [L/T]
   const double ks_mean,      // Hydraulic conductivity mean [L/T]
   const double ks_std) const // Hydraulic conductivity standard deviation [L]
{
   const auto mean  = std::log( std::pow(ks_mean,2) / (std::sqrt(std::pow(ks_mean,2) + std::pow(ks_std,2))));
   const auto sigma = std::sqrt( std::log( 1 + std::pow((ks_std/mean),2)));


   const auto &x = rainfall;
   const auto sqrt2 = std::sqrt(2.0);
   const auto term1 = x / 2.0;
   const auto term2 = x / 2.0 * std::erf( (mean + std::log(1.0 / 3.0 / x)) / (sqrt2 * sigma) );
   const auto term3 = (1.0 / 6.0) * std::exp(mean + std::pow(sigma,2) / 2.0) * std::erfc( (mean + std::pow(sigma,2) - std::log(3.0 * x)) / (sigma * sqrt2));
   return term1 + term2 + term3;
}
// clang-format on
