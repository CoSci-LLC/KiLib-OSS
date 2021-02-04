#pragma once

#include <KiLib/Stability/SafetyFactor/SafetyFactor.hpp>

namespace KiLib
{
   namespace Stability
   {
      namespace SafetyFactor
      {
         class MDSTAB : public SafetyFactor
         {
         public:
            MDSTAB();
            virtual double ComputeSF(
               double phi, double m, double z, double Crl, double Crb, double theta, double delta, double gamma_s,
               double w, double l, double gamma_w) const;
         };
      } // namespace SafetyFactor
   }    // namespace Stability
} // namespace KiLib
