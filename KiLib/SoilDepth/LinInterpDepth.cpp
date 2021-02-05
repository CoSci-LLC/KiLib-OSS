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


#include <KiLib/SoilDepth/LinInterpDepth.hpp>
#include <algorithm>

using namespace KiLib::SoilDepth;

LinInterpDepth::LinInterpDepth(double depthMin, double depthMax, double valMin, double valMax)
{
   this->depthMin = depthMin;
   this->depthMax = depthMax;
   this->valMin   = valMin;
   this->valMax   = valMax;
}


double LinInterpDepth::ComputeDepth(double val)
{
   const double soildepth =
      this->depthMax - (val - this->valMin) / (this->valMax - this->valMin) * (this->depthMax - this->depthMin);

   return std::min(std::max(soildepth, this->depthMin), this->depthMax);
}
