#pragma once

#include <KiLib/Hydrology/BaseHydrology.hpp>

namespace KiLib
{
   namespace Hydrology
   {
      class TopModel : public BaseHydrology
      {
      public:
         TopModel();

         double ComputeWetness(double rainfall, double ks, double z, double slope, double twi) const;
      };
   } // namespace Hydrology
} // namespace KiLib
