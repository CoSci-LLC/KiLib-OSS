#pragma once

#include <KiLib/SoilDepth/BaseSoilDepth.hpp>

namespace KiLib
{
   namespace SoilDepth
   {
      // Linearly interpolation to find soil depth
      // Depth min/max are max and min depths to use
      // valMin and valMax are bounds on val (could be min and max elevation or slope for instance)
      class LinInterpDepth : BaseSoilDepth
      {
      public:
         LinInterpDepth(double depthMin, double depthMax, double valMin, double valMax);

         double computeDepth(double val);

         double depthMin;
         double depthMax;
         double valMin;
         double valMax;
      };
   } // namespace SoilDepth
} // namespace KiLib
