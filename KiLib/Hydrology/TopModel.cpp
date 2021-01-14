#include <KiLib/Exceptions/NotImplemented.hpp>
#include <KiLib/Hydrology/Hydrology.hpp>
#include <cmath>

using namespace KiLib::Hydrology;

TopModel::TopModel(){};

double TopModel::ComputeWetness(double rainfall, double ks, double z, double slope, double twi) const
{
   double wetness = std::min(1.0, ((rainfall * 0.9) / (ks * z * std::cos(slope))) * twi);

   return wetness;
}
