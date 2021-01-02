#pragma once

#include <KiLib/Exceptions/NotImplemented.hpp>

namespace KiLib
{
   namespace Stability
   {
      namespace SafetyFactor
      {
         class SafetyFactor
         {
         public:
            SafetyFactor(){};

            double ComputeSF() const;
         };
      } // namespace SafetyFactor
   }    // namespace Stability
} // namespace KiLib
