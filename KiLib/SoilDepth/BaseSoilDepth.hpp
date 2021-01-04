#pragma once

#include <KiLib/Exceptions/NotImplemented.hpp>

namespace KiLib
{
   namespace SoilDepth
   {
      class BaseSoilDepth
      {
      public:
         BaseSoilDepth(){};

         double ComputeDepth() const;
      };
   } // namespace SoilDepth
} // namespace KiLib
