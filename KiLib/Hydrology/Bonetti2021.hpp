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

#include <KiLib/Hydrology/BaseInfiltration.hpp>

namespace KiLib::Hydrology
{
   class Bonetti2021 : public BaseInfiltration
   {
   public:
      Bonetti2021();

      // clang-format off
      double ComputeInfiltration(
         const double rainfall,      // Rainfall rate [L/T]
         const double ks_mean,       // Hydraulic conductivity mean[L/T]
         const double ks_std) const; // Hydraulic conductivity standard deviation [L/T]
      // clang-format on
   };
} // namespace KiLib::Hydrology
