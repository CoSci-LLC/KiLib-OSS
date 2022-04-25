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
#include <spdlog/fmt/ostr.h>

namespace fs = std::filesystem;

namespace KiLib
{
   RasterNew::RasterNew()
   {
      this->nRows = 0;
      this->nCols = 0;
      this->nData = 0;

      this->nodata_value = -9999;

      this->data.resize(0, 0);
   }

   double &RasterNew::at(Eigen::Index row, Eigen::Index col)
   {
      return this->data(row, col);
   }

   Eigen::Index RasterNew::flattenIndex(Eigen::Index row, Eigen::Index col)
   {
      return row * this->nCols + col;
   }

} // namespace KiLib
