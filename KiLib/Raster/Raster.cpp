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
#include <map>
#include <spdlog/fmt/ostr.h>
#include <spdlog/spdlog.h>

namespace fs = std::filesystem;

namespace KiLib
{
   Raster::Raster()
   {
      this->constructed = false;
      this->nRows       = 0;
      this->nCols       = 0;
      this->nData       = 0;
   }

   // Load data in Raster format from specified path
   Raster::Raster(std::string path)
   {
      this->constructed = false;

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

      this->constructed = true;
   }

   // Returns (bilinear) interpolated data value at specified position
   // Takes in a vec3 for convenience, ignores Z
   double Raster::getInterpBilinear(const Vec3 &pos) const
   {
      double r = std::clamp((pos.y - this->yllcorner) / this->cellsize, 0.0, (double)this->nRows - 1);
      double c = std::clamp((pos.x - this->xllcorner) / this->cellsize, 0.0, (double)this->nCols - 1);

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

   void Raster::writeToFile(const std::string path) const
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
      std::uniform_real_distribution<double> xDist(0, this->width);
      std::uniform_real_distribution<double> yDist(0, this->height);

      point.x = this->xllcorner + xDist(gen);
      point.y = this->yllcorner + yDist(gen);
      point.z = this->getInterpBilinear(point);

      return point;
   }

   size_t Raster::getNearestCell(const KiLib::Vec3 &pos)
   {
      double rF = (pos.y - this->yllcorner) / this->cellsize;
      double cF = (pos.x - this->xllcorner) / this->cellsize;

      size_t r = std::clamp((size_t)std::round(rF), 0UL, this->nRows - 1);
      size_t c = std::clamp((size_t)std::round(cF), 0UL, this->nCols - 1);

      return r * this->nCols + c;
   }

   static const auto EnumToSlope =
      std::map<KiLib::Raster::SlopeMethod, std::function<KiLib::Raster(const KiLib::Raster &)>>{
         {KiLib::Raster::SlopeMethod::ZevenbergenThorne, KiLib::Raster::computeSlopeZevenbergenThorne},
      };

   KiLib::Raster KiLib::Raster::computeSlope(KiLib::Raster::SlopeMethod method) const
   {
      return EnumToSlope.at(method)(*this);
   }

   // Based on Zevenbergen, L.W. and Thorne, C.R. (1987), Quantitative analysis of land surface topography. Earth Surf.
   // Process. Landforms, 12: 47-56. https://doi.org/10.1002/esp.3290120107
   KiLib::Raster KiLib::Raster::computeSlopeZevenbergenThorne(const KiLib::Raster &inp)
   {
      KiLib::Raster slope = KiLib::Raster::zerosLike(inp);

      double twoL = 2.0 * inp.cellsize;
      double g, h;
      double ND = inp.nodata_value;
      size_t NR = inp.nRows;
      size_t NC = inp.nCols;
      // We're going to handle each case (internal, edge, corner) separately so we dont have
      // branch in the loops

      // Internal nodes
      for (size_t r = 1; r < (NR - 1); r++)
      {
         for (size_t c = 1; c < (NC - 1); c++)
         {
            if (inp(r, c - 1) == ND || inp(r, c + 1) == ND || inp(r - 1, c) == ND || inp(r + 1, c) == ND)
            {
               slope(r, c) = ND;
               continue;
            }
            g           = (-inp(r, c - 1) + inp(r, c + 1)) / twoL; // Eqn 9
            h           = (inp(r - 1, c) - inp(r + 1, c)) / twoL;  // Eqn 10
            slope(r, c) = std::sqrt(g * g + h * h);                // Eqn 13
         }
      }

      // Left edge
      for (size_t r = 1; r < (NR - 1); r++)
      {
         if (inp(r, 1) == ND || inp(r - 1, 0) == ND || inp(r + 1, 0) == ND)
         {
            slope(r, 0) = ND;
            continue;
         }
         g           = inp(r, 1) / inp.cellsize;
         h           = (inp(r - 1, 0) - inp(r + 1, 0)) / twoL;
         slope(r, 0) = std::sqrt(g * g + h * h);
      }

      // Right edge
      for (size_t r = 1; r < (NR - 1); r++)
      {
         if (inp(r, NC - 2) == ND || inp(r - 1, NC - 1) == ND || inp(r + 1, NC - 1) == ND)
         {
            slope(r, NC - 1) = ND;
            continue;
         }
         g                = inp(r, NC - 2) / inp.cellsize;
         h                = (inp(r - 1, NC - 1) - inp(r + 1, NC - 1)) / twoL;
         slope(r, NC - 1) = std::sqrt(g * g + h * h);
      }

      // Top edge
      for (size_t c = 1; c < (NC - 1); c++)
      {
         if (inp(0, c - 1) == ND || inp(0, c + 1) == ND || inp(1, c) == ND)
         {
            slope(0, c) = ND;
            continue;
         }
         g           = (-inp(0, c - 1) + inp(0, c + 1)) / twoL;
         h           = inp(1, c) / inp.cellsize;
         slope(0, c) = std::sqrt(g * g + h * h);
      }

      // Bottom edge
      for (size_t c = 1; c < (NC - 1); c++)
      {
         if (inp(NR - 1, c - 1) == ND || inp(NR - 1, c + 1) == ND || inp(NR - 2, c) == ND)
         {
            slope(NR - 1, c) = ND;
            continue;
         }
         g                = (-inp(NR - 1, c - 1) + inp(NR - 1, c + 1)) / twoL;
         h                = inp(NR - 2, c) / inp.cellsize;
         slope(NR - 1, c) = std::sqrt(g * g + h * h);
      }

      // Top left
      if (inp(0, 1) == ND || inp(1, 0) == ND)
      {
         g           = inp(0, 1) / inp.cellsize;
         h           = inp(1, 0) / inp.cellsize;
         slope(0, 0) = std::sqrt(g * g + h * h);
      }
      else
      {
         slope(0, 0) = ND;
      }

      // Top right
      if (inp(0, NC - 2) == ND || inp(1, NC - 1) == ND)
      {
         g                = inp(0, NC - 2) / inp.cellsize;
         h                = inp(1, NC - 1) / inp.cellsize;
         slope(0, NC - 1) = std::sqrt(g * g + h * h);
      }
      else
      {
         slope(0, NC - 1) = ND;
      }

      // Bottom left
      if (inp(NR - 1, 1) == ND || inp(NR - 2, 0) == ND)
      {
         g                = inp(NR - 1, 1) / inp.cellsize;
         h                = inp(NR - 2, 0) / inp.cellsize;
         slope(NR - 1, 0) = std::sqrt(g * g + h * h);
      }
      else
      {
         slope(NR - 1, 0) = ND;
      }


      // Bottom right
      if (inp(NR - 1, NC - 2) == ND || inp(NR - 2, NC - 1) == ND)
      {
         g                     = inp(NR - 1, NC - 2) / inp.cellsize;
         h                     = inp(NR - 2, NC - 1) / inp.cellsize;
         slope(NR - 1, NC - 1) = std::sqrt(g * g + h * h);
      }
      else
      {
         slope(NR - 1, NC - 1) = ND;
      }

      return slope;
   }

} // namespace KiLib
