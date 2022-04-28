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

namespace KiLib
{
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

   std::vector<Index> RasterNew::GetValidIndices() const
   {
      std::vector<Index> indices;
      for (Index row = 0; row < this->nRows; row++)
      {
         for (Index col = 0; col < this->nCols; col++)
         {
            if (this->data(row, col) != this->nodata_value)
            {
               indices.push_back(this->FlattenIndex(row, col));
            }
         }
      }

      return indices;
   }
} // namespace KiLib
