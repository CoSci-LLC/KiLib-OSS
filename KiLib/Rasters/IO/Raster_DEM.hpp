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


#include <KiLib/Rasters/Raster.hpp>
#include <fstream>
#include <spdlog/fmt/ostr.h>
#include <spdlog/spdlog.h>
#include <sstream>
#include <stdexcept>

namespace KiLib::Rasters
{
   template<typename T>
   static KiLib::Rasters::Raster<T> fromDEM(const std::string &path, std::function<bool(T&, double, bool)> construct_val)
   {
      std::ifstream      rasterFile;
      std::string        line, key;
      std::istringstream stream(line);

      // Open Elevation file
      rasterFile.open(path, std::ios::in);
      if (!rasterFile.is_open())
      {
         spdlog::error("Failed to open {} for reading", path);
         exit(EXIT_FAILURE);
      }


      size_t nCols, nRows, cellsize;
      double xllcorner, yllcorner, nodata_value;

      // Load header
      for (int i = 0; i < 6; i++)
      {
         getline(rasterFile, line);
         stream.str(line);
         stream.clear();

         stream >> key;

         // Force lower-case keys
         std::transform(key.begin(), key.end(), key.begin(), ::tolower);

         if (key == "ncols")
         {
            stream >> nCols;
         }
         else if (key == "nrows")
         {
            stream >> nRows;
         }
         else if (key == "xllcorner")
         {
            stream >> xllcorner;
         }
         else if (key == "yllcorner")
         {
            stream >> yllcorner;
         }
         else if (key == "cellsize")
         {
            stream >> cellsize;
         }
         else if (key == "nodata_value")
         {
            stream >> nodata_value;
         }
         else
         {
            throw std::invalid_argument("Unexpected value in header!");
         }
      }

      

      KiLib::Rasters::Raster<T> raster(nRows, nCols);
      raster.set_yllcorner(yllcorner);
      raster.set_xllcorner(xllcorner);
      raster.set_nodatavalue(nodata_value);

      // Load elevations
      printf("Loading raster now: %zu x %zu", nRows, nCols);
      for (size_t row = 0; row < nRows; row++)
      {
         getline(rasterFile, line);
         stream.str(line);
         stream.clear();

         // Moving along a row (across columns) is movement through X
         // Moving down a column (across rows) is movement through Y
         for (size_t col = 0; col < nCols; col++)
         {
            double value;
            stream >> value;
             T v;
               if( construct_val( v, value, value == raster.get_nodatavalue() ) ) {
                     printf("Adding value %lf", value);
                    raster.set(nRows - row - 1, col, v);
              }
         }
      }

      raster.set_width( nCols * cellsize );
      raster.set_height( nRows * cellsize );

      rasterFile.close();

      return raster;
   }

} // namespace KiLib
