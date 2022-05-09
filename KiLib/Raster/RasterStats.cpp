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
   double Raster::GetMinValue() const
   {
      return (this->data != this->nodata_value).select(this->data, DOUBLE_INF).minCoeff();
   }

   double Raster::GetMaxValue() const
   {
      return (this->data != this->nodata_value).select(this->data, -DOUBLE_INF).maxCoeff();
   }

   double Raster::GetMeanValue() const
   {
      auto   mask  = this->data != this->nodata_value;
      double sum   = mask.select(this->data, 0.0).sum();
      double denom = mask.count();
      return sum / denom;
   }

   Index Raster::GetNDataPoints() const
   {
      return (this->data != this->nodata_value).count();
   }

   Index Raster::GetNNoDataPoints() const
   {
      return (this->data == this->nodata_value).count();
   }

   std::vector<Index> Raster::GetValidIndices() const
   {
      std::vector<Index> indices;
      for (auto [row, col] : this->RowColIndexIter())
      {
         if (this->data(row, col) != this->nodata_value)
         {
            indices.push_back(this->FlattenIndex(row, col));
         }
      }

      return indices;
   }
} // namespace KiLib
