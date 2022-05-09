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
#include <fstream>
#include <spdlog/fmt/ostr.h>
#include <spdlog/spdlog.h>
#include <sstream>

namespace KiLib
{
   void Raster::FromDEM(const std::string &path)
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
            stream >> this->nCols;
         }
         else if (key == "nrows")
         {
            stream >> this->nRows;
         }
         else if (key == "xllcorner")
         {
            stream >> this->xllcorner;
         }
         else if (key == "yllcorner")
         {
            stream >> this->yllcorner;
         }
         else if (key == "cellsize")
         {
            stream >> this->cellsize;
         }
         else if (key == "nodata_value")
         {
            stream >> this->nodata_value;
         }
         else
         {
            spdlog::error("Unexpected value in header!");
            exit(EXIT_FAILURE);
         }
      }

      this->Resize(nRows, nCols);
      // Load elevations
      for (Index row = 0; row < this->nRows; row++)
      {
         getline(rasterFile, line);
         stream.str(line);
         stream.clear();

         // Moving along a row (across columns) is movement through X
         // Moving down a column (across rows) is movement through Y
         for (Index col = 0; col < this->nCols; col++)
         {
            double value;
            stream >> value;
            this->at(row, col) = value;
         }
      }

      // Flip the data to match the coordinate system of the DEM
      this->data.colwise().reverseInPlace();

      this->width  = this->nCols * this->cellsize;
      this->height = this->nRows * this->cellsize;
      this->nData  = this->nRows * this->nCols;

      rasterFile.close();
   }

   void Raster::ToDEM(const std::string &path) const
   {
      std::ofstream outFile = std::ofstream(path);
      if (!outFile.is_open())
      {
         spdlog::error("Cannot open output {}", path);
         exit(EXIT_FAILURE);
      }

      fmt::print(
         outFile,
         "ncols {}\n"
         "nrows {}\n"
         "xllcorner {}\n"
         "yllcorner {}\n"
         "cellsize {}\n"
         "NODATA_value {}\n",
         this->nCols, this->nRows, this->xllcorner, this->yllcorner, this->cellsize, this->nodata_value);

      Matrix outdata = this->data.colwise().reverse();

      for (Index row = 0; row < this->nRows; row++)
      {
         for (Index col = 0; col < this->nCols; col++)
         {
            double val = outdata(row, col);

            if (val == this->nodata_value)
            {
               fmt::print(outFile, "{} ", this->nodata_value);
            }
            else
            {
               fmt::print(outFile, "{: .8f} ", val);
            }
         }
         fmt::print(outFile, "\n");
      }

      outFile.close();
   }
} // namespace KiLib