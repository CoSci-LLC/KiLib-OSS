#pragma once

#include <KiLib/Stability/SafetyFactor/SafetyFactor.hpp>

namespace KiLib
{
   namespace Stability
   {
      namespace SafetyFactor
      {
         class MDSTab : public SafetyFactor
         {
         public:
            MDSTab();
            double ComputeSF(
               double phi, double m, double z, double Crl, double Crb, double theta, double delta, double gamma_s,
               double w, double l) const;

            double gamma_w = 9810.0; // unit weight of water [kN/m3]
         };
      } // namespace SafetyFactor
   }    // namespace Stability
} // namespace KiLib
