#pragma once

#include <KiLib/SoilDepth/SoilDepth.hpp>

namespace KiLib
{
   namespace SoilDepth
   {
      class TopModel : public SoilDepth
      {
      public:
         TopModel();

         double ComputeDepth(double rainfall, double ks, double z, double slope, double twi) const;
      };
   } // namespace SoilDepth
} // namespace KiLib
