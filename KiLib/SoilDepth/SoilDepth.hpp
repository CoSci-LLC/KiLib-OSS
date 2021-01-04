#pragma once

#include <KiLib/Exceptions/NotImplemented.hpp>

namespace KiLib
{
   namespace SoilDepth
   {
      class SoilDepth
      {
      public:
         SoilDepth(){};

         double ComputeDepth() const;
      };
   } // namespace SoilDepth
} // namespace KiLib
