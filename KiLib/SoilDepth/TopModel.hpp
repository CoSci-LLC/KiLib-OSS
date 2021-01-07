#pragma once

#include <KiLib/SoilDepth/BaseSoilDepth.hpp>

namespace KiLib
{
   namespace SoilDepth
   {
      class TopModel : public BaseSoilDepth
      {
      public:
         TopModel();

         double ComputeDepth(double rainfall, double ks, double z, double slope, double twi) const;
      };
   } // namespace SoilDepth
} // namespace KiLib
