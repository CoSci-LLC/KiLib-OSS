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
      double r = std::clamp<double>((pos.y - this->yllcorner) / this->cellsize, 0.0, (double)this->nRows - 1);
      double c = std::clamp<double>((pos.x - this->xllcorner) / this->cellsize, 0.0, (double)this->nCols - 1);

      if (r == floor(r) && c == floor(c))
      {
         return this->at(r, c);
      }

      const double y1  = floor(r) * this->cellsize;
      const double y2  = ceil(r) * this->cellsize;
      const double x1  = floor(c) * this->cellsize;
      const double x2  = ceil(c) * this->cellsize;
      const double q11 = this->operator()(floor(r), floor(c));
      const double q12 = this->operator()(ceil(r), floor(c));
      const double q21 = this->operator()(floor(r), ceil(c));
      const double q22 = this->operator()(ceil(r), ceil(c));

      const double x = c * this->cellsize;
      const double y = r * this->cellsize;

      double fy1 = q12;
      double fy2 = q11;
      if (x2 != x1)
      {
         fy1 = ((x2 - x) / (x2 - x1)) * q11 + ((x - x1) / (x2 - x1)) * q21;
         fy2 = ((x2 - x) / (x2 - x1)) * q12 + ((x - x1) / (x2 - x1)) * q22;
      }

      double z = 0;
      if (y2 != y1)
      {
         z = ((y2 - y) / (y2 - y1)) * fy1 + ((y - y1) / (y2 - y1)) * fy2;
      }
      else
      {
         z = (fy1 + fy2) / 2.0;
      }

      return z;
   }

   // Print Raster metadata
   void Raster::print()
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

   KiLib::Vec3 Raster::randPoint(std::mt19937_64 &gen)
   {
      KiLib::Vec3                            point;
      std::uniform_real_distribution<double> xDist(this->xllcorner, this->width);
      std::uniform_real_distribution<double> yDist(this->yllcorner, this->height);

      point.x = xDist(gen);
      point.y = yDist(gen);
      point.z = this->getInterpBilinear(point);

      return point;
   }

   size_t Raster::flattenIndex(size_t r, size_t c)
   {
      return r * this->nCols + c;
   }

   size_t Raster::getNearestCell(const KiLib::Vec3 &pos)
   {
      double rF = (pos.y - this->yllcorner) / this->cellsize;
      double cF = (pos.x - this->xllcorner) / this->cellsize;

      size_t r = std::clamp<size_t>((size_t)std::round(rF), 0UL, this->nRows - 1);
      size_t c = std::clamp<size_t>((size_t)std::round(cF), 0UL, this->nCols - 1);

      return r * this->nCols + c;
   }

   KiLib::Vec3 Raster::getCellPos(size_t ind)
   {
      size_t r = ind / this->nCols;
      size_t c = ind % this->nCols;

      KiLib::Vec3 pos = KiLib::Vec3(this->xllcorner + c * this->cellsize, this->yllcorner + r * this->cellsize, 0);
      pos.z           = this->getInterpBilinear(pos);

      return pos;
   }

   double Raster::GetAverage(size_t ind, double radius)
   {
      if (ind >= this->nData)
      {
         throw std::out_of_range(fmt::format("Index {} out of range for Raster with {} datapoints", ind, this->nData));
      }

      int r      = ind / this->nCols;
      int c      = ind % this->nCols;
      int extent = std::floor(radius / this->cellsize);

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
            // this can probably be done faster, handles the corners being out of the radius
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

      return sum / num;
   }

   double Raster::distFromBoundary(const Vec3 &pos)
   {
      const double left   = pos.x - this->xllcorner;
      const double right  = this->xllcorner + this->width - pos.x;
      const double top    = this->yllcorner + this->height - pos.y;
      const double bottom = pos.y - this->yllcorner;

      return std::min(std::min(left, right), std::min(top, bottom));
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
      
      auto cmp = [&](double a, double b, double eps, std::string val) {
         if (std::abs(a-b) > eps) {
            throw std::invalid_argument(fmt::format("Raster {} sizes do not agree! Got {} and {}", val, a, b));
         }
      };

      // Cell Size
      for (const KiLib::Raster* rast : rasts) {
         cmp(rast->cellsize, rasts.at(0)->cellsize, 1e-2, "CellSize");
      }
      
      // nRows
      for (const KiLib::Raster* rast : rasts) {
         cmp(rast->nRows, rasts.at(0)->nRows, 0.0, "Num Rows");
      }
      
      // nCols
      for (const KiLib::Raster* rast : rasts) {
         cmp(rast->nCols, rasts.at(0)->nCols, 0.0, "Num Cols");
      }
   }
} // namespace KiLib
