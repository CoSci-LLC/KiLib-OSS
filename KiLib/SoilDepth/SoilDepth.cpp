#include <KiLib/Exceptions/NotImplemented.hpp>
#include <KiLib/SoilDepth/SoilDepth.hpp>

using namespace KiLib::SoilDepth;

double SoilDepth::ComputeDepth() const
{
   throw NotImplementedException("Method not implemented");
}