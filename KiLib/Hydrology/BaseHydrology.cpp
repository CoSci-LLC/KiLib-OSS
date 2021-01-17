#include <KiLib/Exceptions/NotImplemented.hpp>
#include <KiLib/Hydrology/BaseHydrology.hpp>

using namespace KiLib::Hydrology;

double BaseHydrology::ComputeWetness() const
{
   throw NotImplementedException("Method not implemented");
}
