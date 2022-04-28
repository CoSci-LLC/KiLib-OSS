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

namespace KiLib
{
   double &RasterNew::at(Index row, Index col)
   {
      return this->data(row, col);
   }

   double RasterNew::at(Index row, Index col) const
   {
      return this->data(row, col);
   }

   double &RasterNew::operator()(Index row, Index col)
   {
      return this->data(row, col);
   }

   double RasterNew::operator()(Index row, Index col) const
   {
      return this->data(row, col);
   }

   Index RasterNew::FlattenIndex(Index row, Index col) const
   {
      if (row >= this->nRows || col >= this->nCols)
      {
         throw std::out_of_range(fmt::format(
            "FlattenIndex ({}, {}) out of range for Raster with size ({}, {})", row, col, this->nRows, this->nCols));
      }

      return row * this->nCols + col;
   }

   std::pair<Index, Index> RasterNew::GetRowCol(Index ind) const
   {
      if (ind >= this->nData)
      {
         throw std::out_of_range(fmt::format("Index {} out of range for Raster with {} datapoints", ind, this->nData));
      }

      const Index r = ind / this->nCols;
      const Index c = ind % this->nCols;
      return std::make_pair(r, c);
   }

   KiLib::Vec3 RasterNew::RandPoint(std::mt19937_64 &gen) const
   {
      KiLib::Vec3                            point;
      std::uniform_real_distribution<double> xDist(this->xllcorner, this->xllcorner + this->width);
      std::uniform_real_distribution<double> yDist(this->yllcorner, this->yllcorner + this->height);

      point.x = xDist(gen);
      point.y = yDist(gen);
      point.z = this->GetInterpBilinear(point);

      return point;
   }

   Index RasterNew::GetNearestCell(KiLib::Vec3 pos) const
   {
      const double rF = (pos.y - this->yllcorner) / this->cellsize;
      const double cF = (pos.x - this->xllcorner) / this->cellsize;

      size_t r = std::clamp<Index>((Index)std::floor(rF), 0L, this->nRows - 1);
      size_t c = std::clamp<Index>((Index)std::floor(cF), 0L, this->nCols - 1);

      return this->FlattenIndex(r, c);
   }

   double RasterNew::operator()(Vec3 pos) const
   {
      return this->GetInterpBilinear(pos);
   }

   void RasterNew::Resize(Index nRows, Index nCols)
   {
      this->data.conservativeResize(nRows, nCols);

      this->nRows  = nRows;
      this->nCols  = nCols;
      this->nData  = nRows * nCols;
      this->width  = this->nCols * this->cellsize;
      this->height = this->nRows * this->cellsize;
   }
} // namespace KiLib
