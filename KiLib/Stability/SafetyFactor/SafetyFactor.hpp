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

            virtual double ComputeSF() const = 0;
         };
      } // namespace SafetyFactor
   }    // namespace Stability
} // namespace KiLib
