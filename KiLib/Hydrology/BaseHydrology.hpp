#pragma once

#include <KiLib/Exceptions/NotImplemented.hpp>

namespace KiLib
{
   namespace Hydrology
   {
      class BaseHydrology
      {
      public:
         BaseHydrology(){};

         double ComputeWetness() const;
      };
   } // namespace Hydrology
} // namespace KiLib
