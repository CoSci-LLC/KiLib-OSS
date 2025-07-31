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

#include <KiLib/Hydrology/BaseHydrology.hpp>

namespace KiLib::Hydrology
{
   class Iverson2000STST : public BaseHydrology
   {
   public:
      Iverson2000STST();

      // clang-format off
      double ComputeWaterPressure(
         const double Ks,                 // Saturated vertical hydraulic conductivity [m/s]
         const double Ss,                 // Specific storage [1/m]
         const double rainfall,           // Event rainfall rate [m/s]
         const double rainfallLT,         // Long-term rainfall rate [m/s]
         const double duration,           // Event duration [s]
         const double soil_depth,         // Soil depth [m]
         const double water_table_depth,  // Water table depth [m]
         const double slope_angle) const; // Slope angle [rad]
      // clang-format on

      double ComputePressureHeadResponseFunction(const double Tstar) const;
   };
} // namespace KiLib::Hydrology
