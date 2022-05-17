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


namespace KiLib
{
   ////////////////////////////////////////////////////////////////////////////////
   // Constructors
   ////////////////////////////////////////////////////////////////////////////////

   Raster::Raster()
   {
      this->xllcorner    = 0.0;
      this->yllcorner    = 0.0;
      this->nodata_value = RASTER_DEFAULT_NODATA_VALUE;
      this->cellsize     = 1.0;
      this->Resize(0, 0);
   }

   Raster::Raster(Index nRows, Index nCols) : Raster()
   {
      this->Resize(nRows, nCols);
   }

   Raster Raster::FillLike(const Raster &other, double fillValue, bool keepNoData)
   {
      KiLib::Raster new_(other);

      new_.data.fill(fillValue);

      if (keepNoData)
      {
         new_.data = (other.data == other.nodata_value).select(other.nodata_value, new_.data);
      }

      return new_;
   }
} // namespace KiLib
