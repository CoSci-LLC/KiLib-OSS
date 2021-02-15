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

   KiLib::Vec3 Raster::randPoint()
   {
      const double x = this->xllcorner + ((double)rand() / (double)RAND_MAX) * this->width;
      const double y = this->yllcorner + ((double)rand() / (double)RAND_MAX) * this->height;
      const double z = this->getInterpBilinear({x, y, 0});

      return KiLib::Vec3{x, y, z};
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
      // We're going to handle each case (internal, edge, corner) separately so we dont have
      // branch in the loops

      // Internal nodes
      for (size_t r = 1; r < (inp.nRows - 1); r++)
      {
         for (size_t c = 1; c < (inp.nRows - 1); c++)
         {
            g           = (-inp(r, c - 1) + inp(r, c + 1)) / twoL; // Eqn 9
            h           = (inp(r - 1, c) - inp(r + 1, c)) / twoL;  // Eqn 10
            slope(r, c) = -std::sqrt(g * g + h * h);               // Eqn 13
         }
      }

      // Left edge
      for (size_t r = 1; r < (inp.nRows - 1); r++)
      {
         g           = inp(r, 1) / inp.cellsize;
         h           = (inp(r - 1, 0) - inp(r + 1, 0)) / twoL;
         slope(r, 0) = -std::sqrt(g * g + h * h);
      }

      // Right edge
      for (size_t r = 1; r < (inp.nRows - 1); r++)
      {
         g                       = inp(r, inp.nCols - 2) / inp.cellsize;
         h                       = (inp(r - 1, inp.nCols - 1) - inp(r + 1, inp.nCols - 1)) / twoL;
         slope(r, inp.nCols - 1) = -std::sqrt(g * g + h * h);
      }

      // Top edge
      for (size_t c = 1; c < (inp.nCols - 1); c++)
      {
         g           = (-inp(0, c - 1) + inp(0, c + 1)) / twoL;
         h           = inp(1, c) / inp.cellsize;
         slope(0, c) = -std::sqrt(g * g + h * h);
      }

      // Bottom edge
      for (size_t c = 1; c < (inp.nCols - 1); c++)
      {
         g                       = (-inp(inp.nRows - 1, c - 1) + inp(inp.nRows - 1, c + 1)) / twoL;
         h                       = inp(inp.nRows - 2, c) / inp.cellsize;
         slope(inp.nRows - 1, c) = -std::sqrt(g * g + h * h);
      }

      // Top left
      g           = inp(0, 1) / inp.cellsize;
      h           = inp(1, 0) / inp.cellsize;
      slope(0, 0) = -std::sqrt(g * g + h * h);

      // Top right
      g                       = inp(0, inp.nCols - 2) / inp.cellsize;
      h                       = inp(1, inp.nCols - 1) / inp.cellsize;
      slope(0, inp.nCols - 1) = -std::sqrt(g * g + h * h);

      // Bottom left
      g                       = inp(inp.nRows - 1, 1) / inp.cellsize;
      h                       = inp(inp.nRows - 2, 0) / inp.cellsize;
      slope(inp.nRows - 1, 0) = -std::sqrt(g * g + h * h);

      // Bottom right
      g                                   = inp(inp.nRows - 1, inp.nCols - 2) / inp.cellsize;
      h                                   = inp(inp.nRows - 2, inp.nCols - 1) / inp.cellsize;
      slope(inp.nRows - 1, inp.nCols - 1) = -std::sqrt(g * g + h * h);

      return inp;
   }

} // namespace KiLib
