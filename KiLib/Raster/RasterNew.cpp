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
      this->xllcorner    = 0.0;
      this->yllcorner    = 0.0;
      this->nodata_value = RASTER_DEFAULT_NODATA_VALUE;
      this->cellsize     = 1.0;
      this->Resize(0, 0);
   }

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

   RasterNew::RasterNew(Index nRows, Index nCols) : RasterNew()
   {
      this->Resize(nRows, nCols);
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
} // namespace KiLib
