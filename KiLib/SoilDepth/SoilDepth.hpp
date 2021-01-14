#pragma once

namespace KiLib
{
   namespace SoilDepth
   {
      // Linearly interpolation to find soil depth
      // Depth min/max are max and min depths to use
      // valMin and valMax are bounds on val (could be min and max elevation or slope for instance)
      double getDepthLinInterp(double val, double depthMin, double depthMax, double valMin, double valMax);

   } // namespace SoilDepth
} // namespace KiLib
