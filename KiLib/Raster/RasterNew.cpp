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

#undef NDEBUG // For bounds checking in eigen

#include <KiLib/Raster/RasterNew.hpp>
#include <filesystem>
#include <fstream>
#include <spdlog/fmt/ostr.h>
#include <spdlog/spdlog.h>
#include <sstream>

#ifdef __APPLE__
namespace fs = std::__fs::filesystem;
#else
namespace fs = std::filesystem;
#endif

namespace KiLib
{
   ////////////////////////////////////////////////////////////////////////////////
   // Constructors
   ////////////////////////////////////////////////////////////////////////////////

   RasterNew::RasterNew()
   {
      this->nodata_value = -9999;
      this->Resize(0, 0);
   }

   /**
    * @brief Construct a new RasterNew object from a raster on disk. Format is determined by extension.
    * Accepted formats are ASCII DEMs (.asc, .ddem), and GeoTIFF (.tif, .tiff).
    *
    * @param path
    */
   RasterNew::RasterNew(const std::string &path)
   {
      auto ext = fs::path(path).extension();

      if (ext == ".asc" || ext == ".dem")
      {
         this->fromDEM(path);
      }
      else if (ext == ".tif" || ext == ".tiff")
      {
         this->fromTiff(path);
      }
      else
      {
         spdlog::error("Unsupported file type given to raster constructor: {}", ext);
         exit(EXIT_FAILURE);
      }
   }

   ////////////////////////////////////////////////////////////////////////////////
   // Access
   ////////////////////////////////////////////////////////////////////////////////

   double &RasterNew::at(Eigen::Index row, Eigen::Index col)
   {
      return this->data(row, col);
   }

   double RasterNew::at(Eigen::Index row, Eigen::Index col) const
   {
      return this->data(row, col);
   }

   double &RasterNew::operator()(Eigen::Index row, Eigen::Index col)
   {
      return this->data(row, col);
   }

   double RasterNew::operator()(Eigen::Index row, Eigen::Index col) const
   {
      return this->data(row, col);
   }

   Eigen::Index RasterNew::flattenIndex(Eigen::Index row, Eigen::Index col) const
   {
      return row * this->nCols + col;
   }

   void RasterNew::Resize(Eigen::Index nRows, Eigen::Index nCols)
   {
      this->data.conservativeResize(nRows, nCols);
      this->nRows  = nRows;
      this->nCols  = nCols;
      this->nData  = nRows * nCols;
      this->width  = this->nCols * this->cellsize;
      this->height = this->nRows * this->cellsize;
   }

   ////////////////////////////////////////////////////////////////////////////////
   // I/O
   ////////////////////////////////////////////////////////////////////////////////

   void RasterNew::writeToFile(const std::string &path) const
   {

      auto ext = fs::path(path).extension();

      if (ext == ".asc" || ext == ".dem")
      {
         this->toDEM(path);
      }
      else if (ext == ".tif" || ext == ".tiff")
      {
         this->toTiff(path);
      }
      else
      {
         spdlog::error("Unsupported output file type: {}", ext);
         exit(EXIT_FAILURE);
      }
   }

   ////////////////////////////////////////////////////////////////////////////////
   // Stats
   ////////////////////////////////////////////////////////////////////////////////

   double RasterNew::GetMinValue() const
   {
      return (this->data.array() != this->nodata_value).select(this->data, DOUBLE_INF).minCoeff();
   }

   double RasterNew::GetMaxValue() const
   {
      return (this->data.array() != this->nodata_value).select(this->data, -DOUBLE_INF).maxCoeff();
   }

   double RasterNew::GetMeanValue() const
   {
      auto   mask  = this->data.array() != this->nodata_value;
      double sum   = mask.select(this->data, 0.0).sum();
      double denom = mask.cast<double>().sum();
      return sum / denom;
   }

   size_t RasterNew::GetNDataPoints() const
   {
      return (this->data.array() != this->nodata_value).cast<size_t>().sum();
   }

   size_t RasterNew::GetNNoDataPoints() const
   {
      return (this->data.array() == this->nodata_value).cast<size_t>().sum();
   }

   std::vector<size_t> RasterNew::GetValidIndices() const
   {
      std::vector<size_t> indices;
      for (Eigen::Index row = 0; row < this->nRows; row++)
      {
         for (Eigen::Index col = 0; col < this->nCols; col++)
         {
            if (this->data(row, col) != this->nodata_value)
            {
               indices.push_back(this->flattenIndex(row, col));
            }
         }
      }

      return indices;
   }
} // namespace KiLib
