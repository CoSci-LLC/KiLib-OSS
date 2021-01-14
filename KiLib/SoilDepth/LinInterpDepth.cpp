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


double LinInterpDepth::computeDepth(double val)
{
   const double soildepth =
      this->depthMax - (val - this->valMin) / (this->valMax - this->valMin) * (this->depthMax - this->depthMin);

   return std::min(std::max(soildepth, this->depthMin), this->depthMax);
}
