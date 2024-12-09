#include <KiLib/Raster/Raster.hpp>
#include <functional>
#include <map>

namespace KiLib
{
   static const auto EnumToSlope =
      std::map<KiLib::Raster::SlopeMethod, std::function<KiLib::Raster(const KiLib::Raster &)>>{
         {KiLib::Raster::SlopeMethod::ZevenbergenThorne, KiLib::Raster::ComputeSlopeZevenbergenThorne},
      };

   KiLib::Raster KiLib::Raster::ComputeSlope(KiLib::Raster::SlopeMethod method) const
   {
      return EnumToSlope.at(method)(*this);
   }

   // Based on Zevenbergen, L.W. and Thorne, C.R. (1987), Quantitative analysis of land surface topography. Earth Surf.
   // Process. Landforms, 12: 47-56. https://doi.org/10.1002/esp.3290120107
   KiLib::Raster KiLib::Raster::ComputeSlopeZevenbergenThorne(const KiLib::Raster &inp)
   {
      KiLib::Raster slope = KiLib::Raster::fillLike(inp, 0.0, true);

      double ND = inp.nodata_value;
      int    NR = inp.nRows;
      int    NC = inp.nCols;

      // Slope over 4 adj points
      auto getSlope = [&](const size_t r, const size_t c)
      {
         int r1 = r - 1;
         int r2 = r + 1;
         int c1 = c - 1;
         int c2 = c + 1;

         double xDiv = inp.cellsize * 2;
         double yDiv = inp.cellsize * 2;

         if ((r1 < 0) or (inp.at(r1, c) == ND))
         {
            r1 = r;
            yDiv /= 2;
         }
         if ((r2 > (NR - 1)) or (inp.at(r2, c) == ND))
         {
            r2 = r;
            yDiv /= 2;
         }
         if ((c1 < 0) or (inp.at(r, c1) == ND))
         {
            c1 = c;
            xDiv /= 2;
         }
         if ((c2 > (NC - 1)) or (inp.at(r, c2) == ND))
         {
            c2 = c;
            xDiv /= 2;
         }
         double g = (-inp(r, c1) + inp(r, c2)) / xDiv; // Eqn 9
         double h = (inp(r1, c) - inp(r2, c)) / yDiv;  // Eqn 10
         return std::sqrt(g * g + h * h);              // Eqn 13
      };

      for (int r = 0; r < NR; r++)
      {
         for (int c = 0; c < NC; c++)
         {
            if (inp(r, c) == ND)
            {
               slope(r, c) = ND;
               continue;
            }
            slope(r, c) = getSlope(r, c);
         }
      }

      return slope;
   }
} // namespace KiLib