/**
 *  Copyright (c) 2020-2021 CoSci LLC, USA <software@cosci-llc.com>
 *
 *  This file is part of KiLib-OSS.
 *
 *  KiLib-OSS is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  KiLib-OSS is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with KiLib-OSS.  If not, see <https://www.gnu.org/licenses/>.
 */


#include <KiLib/Raster/Raster.hpp>
#include <filesystem>
#include <spdlog/fmt/ostr.h>

namespace fs = std::filesystem;

namespace KiLib
{
   Raster::Raster()
   {
      this->nRows = 0;
      this->nCols = 0;
      this->nData = 0;
      this->data.resize(0);
   }

   // Load data in Raster format from specified path
   Raster::Raster(const std::string &path)
   {
      auto ext = fs::path(path).extension();

      if (ext == ".asc" || ext == ".dem")
         this->fromDEM(path);
      else if (ext == ".tif" || ext == ".tiff")
         this->fromTiff(path);
      else
      {
         spdlog::error("Unsupported file type given to raster constructor: {}", ext);
         exit(EXIT_FAILURE);
      }
   }

   // Returns (bilinear) interpolated data value at specified position
   // Takes in a vec3 for convenience, ignores Z
   double Raster::getInterpBilinear(const Vec3 &pos) const
   {
      double x = (pos.x - this->xllcorner) / this->cellsize;
      double y = (pos.y - this->yllcorner) / this->cellsize;

      const size_t r = std::min(static_cast<size_t> (std::floor(y)), this->nRows - 1ul);
      const size_t c = std::min(static_cast<size_t> (std::floor(x)), this->nCols - 1ul);
      

      const size_t ru = std::min(r + 1ul, this->nRows - 1ul);
      const size_t cr = std::min(c + 1ul, this->nCols - 1ul);

      const double f00 = this->operator()(r, c);
      const double f10 = this->operator()(r, cr);
      const double f01 = this->operator()(ru, c);
      const double f11 = this->operator()(ru, cr);

      const double sx = x - std::floor(x);
      const double sy = y - std::floor(y);

      const double val = f00 * (1.0 - sx) * (1.0 - sy) + f10 * sx * (1.0 - sy) + f01 * (1.0 - sx) * sy + f11 * sx * sy;

      return val;
   }

   // Print Raster metadata
   void Raster::print() const
   {
      fmt::print(
         "ncols {}\n"
         "nrows {}\n"
         "xllcorner {:.4f}\n"
         "yllcorner {:.4f}\n"
         "cellsize {:.4f}\n"
         "NODATA_value {}\n",
         this->nCols, this->nRows, this->xllcorner, this->yllcorner, this->cellsize, this->nodata_value);

      for (size_t row = 1; row <= this->nRows; row++)
      {
         for (size_t col = 0; col < this->nCols; col++)
         {
            double val = this->operator()(this->nRows - row, col);

            if (val == this->nodata_value)
            {
               fmt::print("{} ", this->nodata_value);
            }
            else
            {
               fmt::print("{: .3f} ", val);
            }
         }
         fmt::print("\n");
      }
   }

   void Raster::writeToFile(const std::string &path) const
   {

      auto ext = fs::path(path).extension();

      if (ext == ".asc" || ext == ".dem")
         this->toDEM(path);
      else if (ext == ".tif" || ext == ".tiff")
         this->toTiff(path);
      else
      {
         spdlog::error("Unsupported output file type: {}", ext);
         exit(EXIT_FAILURE);
      }
   }

   KiLib::Vec3 Raster::randPoint(std::mt19937_64 &gen) const
   {
      KiLib::Vec3                            point;
      std::uniform_real_distribution<double> xDist(this->xllcorner, this->xllcorner + this->width);
      std::uniform_real_distribution<double> yDist(this->yllcorner, this->yllcorner + this->height);

      point.x = xDist(gen);
      point.y = yDist(gen);
      point.z = this->getInterpBilinear(point);

      return point;
   }

   size_t Raster::flattenIndex(size_t r, size_t c) const
   {
      return r * this->nCols + c;
   }

   size_t Raster::getNearestCell(const KiLib::Vec3 &pos) const
   {
      double rF = (pos.y - this->yllcorner) / this->cellsize;
      double cF = (pos.x - this->xllcorner) / this->cellsize;

      // size_t r = std::clamp<size_t>((size_t)std::round(rF), 0UL, this->nRows - 1);  // WRONG
      // size_t c = std::clamp<size_t>((size_t)std::round(cF), 0UL, this->nCols - 1);  // WRONG
      size_t r = std::clamp<size_t>((size_t)std::floor(rF), 0UL, this->nRows - 1);
      size_t c = std::clamp<size_t>((size_t)std::floor(cF), 0UL, this->nCols - 1);

      return r * this->nCols + c;
   }

   KiLib::Vec3 Raster::getCellPos(size_t ind) const
   {
      size_t r = ind / this->nCols;
      size_t c = ind % this->nCols;

      KiLib::Vec3 pos = KiLib::Vec3(this->xllcorner + c * this->cellsize, this->yllcorner + r * this->cellsize, 0);
      pos.z           = this->getInterpBilinear(pos);

      return pos;
   }

   KiLib::Vec3 Raster::getCellCenter(size_t ind) const
   {
      size_t r = ind / this->nCols;
      size_t c = ind % this->nCols;

      KiLib::Vec3 pos = KiLib::Vec3(
         this->xllcorner + c * this->cellsize + this->cellsize / 2.0,
         this->yllcorner + r * this->cellsize + this->cellsize / 2.0, 0);
      pos.z = this->getInterpBilinear(pos);

      return pos;
   }

   double Raster::GetAverage(size_t ind, double radius) const
   {
      auto [r, c] = Raster::GetRowCol(ind);

      int extent = static_cast<int>(std::floor(radius / this->cellsize));

      int leftB  = std::clamp(c - extent, 0, (int)this->nCols - 1);
      int rightB = std::clamp(c + extent, 0, (int)this->nCols - 1);
      int upB    = std::clamp(r + extent, 0, (int)this->nRows - 1);
      int lowB   = std::clamp(r - extent, 0, (int)this->nRows - 1);

      double sum = 0.0;
      double num = 0.0;
      for (int ri = lowB; ri <= upB; ri++)
      {
         for (int ci = leftB; ci <= rightB; ci++)
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
               SPDLOG_DEBUG("SKIPPING\n");
               continue;
            }
            sum += this->at(ri, ci);
            num += 1;
         }
      }
     
      if (num == 0)
      {
         num += 1;
      }

      return sum / num;
   }

   double Raster::distFromBoundary(const Vec3 &pos) const
   {
      const double left   = pos.x - this->xllcorner;
      const double right  = this->xllcorner + this->width - pos.x;
      const double top    = this->yllcorner + this->height - pos.y;
      const double bottom = pos.y - this->yllcorner;

      return std::min(std::min(left, right), std::min(top, bottom));
   }

   std::pair<int, int> Raster::GetRowCol(const size_t ind) const
   {
      if (ind >= this->nData)
      {
         throw std::out_of_range(fmt::format("Index {} out of range for Raster with {} datapoints", ind, this->nData));
      }

      const int r = static_cast<int>(ind / this->nCols);
      const int c = static_cast<int>(ind % this->nCols);
      return std::make_pair(r, c);
   }

   double Raster::operator()(const Vec3 &pos) const
   {
      return this->getInterpBilinear(pos);
   }

   double &Raster::at(size_t row, size_t col)
   {
      return this->data.at(row * this->nCols + col);
   }

   double Raster::at(size_t row, size_t col) const
   {
      return this->data.at(row * this->nCols + col);
   }

   double &Raster::operator()(size_t row, size_t col)
   {
      return this->data[row * this->nCols + col];
   }

   double Raster::operator()(size_t row, size_t col) const
   {
      return this->data[row * this->nCols + col];
   }

   double Raster::operator()(size_t ind) const
   {
      return this->data[ind];
   }

   double &Raster::operator()(size_t ind)
   {
      return this->data[ind];
   }

   double &Raster::at(size_t ind)
   {
      return this->data.at(ind);
   }

   double Raster::at(size_t ind) const
   {
      return this->data.at(ind);
   }


   KiLib::Raster Raster::fillLike(const KiLib::Raster &other, double fillValue, bool keepNoData)
   {
      KiLib::Raster new_(other);
      for (double &val : new_.data)
      {
         if (keepNoData and (val == new_.nodata_value))
         {
            continue;
         }
         val = fillValue;
      }
      return new_;
   }

   std::vector<size_t> Raster::getValidIndices(const std::vector<const KiLib::Raster *> &rasts)
   {
      Raster::assertAgreeDim(rasts);
      std::vector<size_t> inds;

      for (size_t i = 0; i < rasts[0]->data.size(); i++)
      {
         bool foundBad = false;

         // Check all rasters at index i for nodata
         for (const KiLib::Raster *rast : rasts)
         {
            if (rast->operator()(i) == rast->nodata_value)
            {
               foundBad = true;
               break;
            }
         }

         // If found none, append i
         if (foundBad == false)
         {
            inds.push_back(i);
         }
      }

      return inds;
   }

   void Raster::assertAgreeDim(const std::vector<const KiLib::Raster *> &rasts)
   {
      if (rasts.size() == 0)
      {
         return;
      }

      auto cmp = [&](double a, double b, double eps, std::string val)
      {
         if (std::abs(a - b) > eps)
         {
            throw std::invalid_argument(fmt::format("Raster {} sizes do not agree! Got {} and {}", val, a, b));
         }
      };

      // Cell Size
      for (const KiLib::Raster *rast : rasts)
      {
         cmp(rast->cellsize, rasts.at(0)->cellsize, 1e-2, "CellSize");
      }

      // nRows
      for (const KiLib::Raster *rast : rasts)
      {
         cmp(static_cast<double>(rast->nRows), static_cast<double>(rasts.at(0)->nRows), 0.0, "Num Rows");
      }

      // nCols
      for (const KiLib::Raster *rast : rasts)
      {
         cmp(static_cast<double>(rast->nCols), static_cast<double>(rasts.at(0)->nCols), 0.0, "Num Cols");
      }
   }

   std::optional<KiLib::Vec3> Raster::GetCoordMinDistance(
      //size_t ind, double zInd, const KiLib::Raster &elev, double radius, double threshold) const
      size_t ind, const KiLib::Vec3 &inPos, const KiLib::Raster &elev, double radius, double threshold) const
   {
      const auto zInd = inPos.z;
      auto [r, c] = Raster::GetRowCol(ind);

      const int extent = static_cast<int>(std::floor(radius / this->cellsize));

      const int leftB  = std::clamp(c - extent, 0, (int)this->nCols - 1);
      const int rightB = std::clamp(c + extent, 0, (int)this->nCols - 1);
      const int upB    = std::clamp(r + extent, 0, (int)this->nRows - 1);
      const int lowB   = std::clamp(r - extent, 0, (int)this->nRows - 1);

      auto dist2value = std::numeric_limits<double>::max();
      auto value      = std::numeric_limits<double>::max();
      auto slope0     = std::numeric_limits<double>::min();
      auto slope1     = std::numeric_limits<double>::min();
      auto elev0      = std::numeric_limits<double>::min();

      KiLib::Vec3 pos; // Return value if position found
      bool        found = false;

      for (int ri = lowB; ri <= upB; ri++)
      {
         for (int ci = leftB; ci <= rightB; ci++)
         {
            // Skip nodata and values less than threshold
            if (this->at(ri, ci) == this->nodata_value || this->at(ri, ci) < threshold)
            {
               continue;
            }
            //spdlog::info("  GetCoordMinDist inPos.x = {}, ci = {}", inPos.x, static_cast<double>(ci));
            //spdlog::info("  GetCoordMinDist inPos.y = {}, ri = {}", inPos.y, static_cast<double>(ri));
            // This can probably be done faster, handles the corners being out of the radius
            //const double dr   = std::abs((double)(r - ri)) * cellsize;
            //const double dc   = std::abs((double)(c - ci)) * cellsize;
            const double dr   = inPos.y - (yllcorner + static_cast<double>(ri) * cellsize + cellsize / 2.0);
            const double dc   = inPos.x - (xllcorner + static_cast<double>(ci) * cellsize + cellsize / 2.0);
            const double dist = sqrt(dr * dr + dc * dc);
            //spdlog::info("  GetCoordMinDist dr = {}; dc = {}; dist = {}", dr, dc, dist);
            if (dist > radius)
            {
               SPDLOG_DEBUG("SKIPPING\n");
               continue;
            }
            // compute slope
            slope1 = (zInd - elev.at(ri, ci)) / dist;
            // Get position if
            if (//this->at(ri, ci) < value && 
                //elev0 < elev.at(ri, ci) && 
                elev.at(ri, ci) < zInd && 
                dist <= dist2value 
                // && 
                //slope1 > slope0)
                )
            {
               dist2value = dist;
               value      = this->at(ri, ci);
               auto ind   = this->flattenIndex(ri, ci);
               pos        = this->getCellPos(ind);
               pos.z      = 0.0; // Reset z to zero
               slope0     = slope1;
               elev0      = elev.at(ri, ci);
               found      = true;
            }
         }
      }
      // Check that pos was found
      if (found)
      {
         return pos;
      }
      else
      {
         return std::nullopt;
      }
   }

} // namespace KiLib
