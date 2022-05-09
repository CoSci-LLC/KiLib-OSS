#include <KiLib/Raster/RasterNew.hpp>

namespace KiLib
{
   double RasterNew::GetInterpBilinear(Vec3 pos) const
   {
      const double x = (pos.x - this->xllcorner) / this->cellsize;
      const double y = (pos.y - this->yllcorner) / this->cellsize;

      const Index r = std::floor(y);
      const Index c = std::floor(x);

      const Index ru = std::min(r + 1L, this->nRows - 1L);
      const Index cr = std::min(c + 1L, this->nCols - 1L);

      const double f00 = this->at(r, c);
      const double f10 = this->at(r, cr);
      const double f01 = this->at(ru, c);
      const double f11 = this->at(ru, cr);

      const double sx = x - std::floor(x);
      const double sy = y - std::floor(y);

      const double val = f00 * (1.0 - sx) * (1.0 - sy) + f10 * sx * (1.0 - sy) + f01 * (1.0 - sx) * sy + f11 * sx * sy;

      return val;
   }

   double RasterNew::DistFromBoundary(const Vec3 pos) const
   {
      const double left   = pos.x - this->xllcorner;
      const double right  = this->xllcorner + this->width - pos.x;
      const double top    = this->yllcorner + this->height - pos.y;
      const double bottom = pos.y - this->yllcorner;

      return std::min(std::min(left, right), std::min(top, bottom));
   }

   double RasterNew::GetAverage(Index ind, double radius) const
   {
      auto [r, c] = this->GetRowCol(ind);

      const Index extent = std::floor(radius / this->cellsize);

      double sum = 0.0;
      double num = 0.0;
      for (auto [ri, ci] : this->RowColSubViewIndexIter(r, c, extent, extent))
      {
         // Skip nodata
         if (this->at(ri, ci) == this->nodata_value)
         {
            continue;
         }
         // This can probably be done faster, handles the corners being out of the radius
         const double dr   = std::abs((double)(r - ri)) * cellsize;
         const double dc   = std::abs((double)(c - ci)) * cellsize;
         const double dist = sqrt(dr * dr + dc * dc);
         if (dist > radius)
         {
            continue;
         }
         sum += this->at(ri, ci);
         num += 1;
      }

      return sum / num;
   }

   static const auto EnumToSlope =
      std::map<KiLib::RasterNew::SlopeMethod, std::function<KiLib::RasterNew(const KiLib::RasterNew &)>>{
         {KiLib::RasterNew::SlopeMethod::ZevenbergenThorne, KiLib::RasterNew::ComputeSlopeZevenbergenThorne},
      };

   KiLib::RasterNew KiLib::RasterNew::ComputeSlope(KiLib::RasterNew::SlopeMethod method) const
   {
      return EnumToSlope.at(method)(*this);
   }

   // Based on Zevenbergen, L.W. and Thorne, C.R. (1987), Quantitative analysis of land surface topography. Earth Surf.
   // Process. Landforms, 12: 47-56. https://doi.org/10.1002/esp.3290120107
   KiLib::RasterNew KiLib::RasterNew::ComputeSlopeZevenbergenThorne(const KiLib::RasterNew &inp)
   {
      KiLib::RasterNew slope = KiLib::RasterNew::FillLike(inp, 0.0, true);

      double ND = inp.nodata_value;
      int    NR = inp.nRows;
      int    NC = inp.nCols;

      // Slope over 4 adj points
      auto getSlope = [&](const Index r, const Index c)
      {
         Index r1 = r - 1;
         Index r2 = r + 1;
         Index c1 = c - 1;
         Index c2 = c + 1;

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

      for (auto [r, c] : inp.RowColIndexIter())
      {
         if (inp(r, c) == ND)
         {
            slope(r, c) = ND;
            continue;
         }
         slope(r, c) = getSlope(r, c);
      }

      return slope;
   }

   std::optional<KiLib::Vec3> RasterNew::GetCoordMinDistance(
      Index ind, double zInd, const KiLib::RasterNew &elev, double radius, double threshold) const
   {
      auto [r, c] = this->GetRowCol(ind);

      auto dist2value = std::numeric_limits<double>::max();
      auto value      = std::numeric_limits<double>::max();

      std::optional<KiLib::Vec3> pos = std::nullopt;

      const Index extent = std::floor(radius / this->cellsize);
      for (auto [ri, ci] : this->RowColSubViewIndexIter(r, c, extent, extent))
      {
         // Skip nodata and values less than threshold
         if (this->at(ri, ci) == this->nodata_value || this->at(ri, ci) < threshold)
         {
            continue;
         }

         const double dr   = std::abs((double)(r - ri)) * cellsize;
         const double dc   = std::abs((double)(c - ci)) * cellsize;
         const double dist = sqrt(dr * dr + dc * dc);
         if (dist > radius)
         {
            continue;
         }
         // Get position if
         if (this->at(ri, ci) < value && elev.at(ri, ci) < zInd && dist <= dist2value)
         {
            dist2value = dist;
            value      = this->at(ri, ci);
            auto ind   = this->FlattenIndex(ri, ci);
            pos        = this->GetCellPos(ind);
            (*pos).z   = 0.0; // Reset z to zero
         }
      }

      return pos;
   }
} // namespace KiLib