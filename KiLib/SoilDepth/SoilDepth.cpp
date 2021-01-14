#include <KiLib/SoilDepth/SoilDepth.hpp>
#include <algorithm>

double KiLib::SoilDepth::getDepthLinInterp(double val, double depthMin, double depthMax, double valMin, double valMax)
{
   const double soildepth = depthMax - (val - valMin) / (valMax - valMin) * (depthMax - depthMin);

   return std::min(std::max(soildepth, depthMin), depthMax);
}
