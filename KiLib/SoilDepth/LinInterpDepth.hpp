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

#include <KiLib/SoilDepth/BaseSoilDepth.hpp>

namespace KiLib::SoilDepth
{
   // Linearly interpolation to find soil depth
   // Depth min/max are max and min depths to use
   // valMin and valMax are bounds on val (could be min and max elevation or slope for instance)
   class LinInterpDepth : BaseSoilDepth
   {
   public:
      LinInterpDepth(double depthMin, double depthMax, double valMin, double valMax);

      double ComputeDepth(double val);

      double depthMin;
      double depthMax;
      double valMin;
      double valMax;
   };
} // namespace KiLib::SoilDepth
