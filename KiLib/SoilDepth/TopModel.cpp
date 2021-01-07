#include <KiLib/Exceptions/NotImplemented.hpp>
#include <KiLib/SoilDepth/TopModel.hpp>
#include <cmath>

using namespace KiLib::SoilDepth;

TopModel::TopModel(){};

double TopModel::ComputeDepth(double rainfall, double ks, double z, double slope, double twi) const
{
   double depth = std::min(1.0, ((rainfall * 0.9) / (ks * z * std::cos(slope))) * twi);

   return depth;
}