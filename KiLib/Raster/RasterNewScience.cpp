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
} // namespace KiLib