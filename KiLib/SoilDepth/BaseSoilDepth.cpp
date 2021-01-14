#include <KiLib/Exceptions/NotImplemented.hpp>
#include <KiLib/SoilDepth/BaseSoilDepth.hpp>

using namespace KiLib::SoilDepth;

double BaseSoilDepth::ComputeDepth() const
{
   throw NotImplementedException("Method not implemented");
}
