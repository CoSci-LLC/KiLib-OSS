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


#pragma once

#include <Eigen/Eigen>
#include <random>
#include <string>
using Eigen::Index;

namespace KiLib
{
   /**
    * @brief Iterates over row and column indices.
    * Row major, so (0, 0)->(0, 1)->...->(1,0)->(1,1)->...
    * Supports OpenMP
    */
   struct RowColIter
   {
      using iterator_category = std::random_access_iterator_tag;
      using difference_type   = std::ptrdiff_t;
      using value_type        = std::pair<Index, Index>;
      using pointer           = std::pair<Index, Index> *;
      using reference         = std::pair<Index, Index> &;

      // Defualt constructer, inits to current pos of 0
      // topLeftX and topLeftY can be used for subview iteration
      RowColIter(Index nRows, Index nCols, Index topLeftRow, Index topLeftCol)
         : nRows(nRows), nCols(nCols), topLeftRow(topLeftRow), topLeftCol(topLeftCol), cur_flat(0)
      {
         this->pos = std::make_pair(this->topLeftRow, this->topLeftCol);
      }

      reference operator*()
      {
         return this->pos;
      }

      RowColIter &operator++()
      {
         this->update(1);
         return *this;
      }

      friend bool operator!=(const RowColIter &a, const RowColIter &b)
      {
         return a.cur_flat != b.cur_flat;
      };

      friend bool operator==(const RowColIter &a, const RowColIter &b)
      {
         return (a.cur_flat == b.cur_flat);
      }

      RowColIter &operator+=(const difference_type &movement)
      {
         this->update(movement);
         return (*this);
      }

      RowColIter &operator-=(const difference_type &movement)
      {
         this->update(-movement);
         return (*this);
      }

      RowColIter &operator--()
      {
         this->update(-1);
         return (*this);
      }

      // Is this correct?
      RowColIter operator++(int)
      {
         auto temp(*this);
         this->update(1);
         return temp;
      }

      // Is this correct?
      RowColIter operator--(int)
      {
         auto temp(*this);
         this->update(-1);
         return temp;
      }

      RowColIter operator+(const difference_type &movement)
      {
         auto oldPtr = this->cur_flat;
         this->update(movement);
         auto temp(*this);
         this->cur_flat = oldPtr;
         return temp;
      }

      RowColIter operator-(const difference_type &movement)
      {
         auto oldPtr = this->cur_flat;
         this->update(-movement);
         auto temp(*this);
         this->cur_flat = oldPtr;
         return temp;
      }

      difference_type operator-(const RowColIter &rawReverseIterator) const
      {
         return this->cur_flat - rawReverseIterator.cur_flat;
      }

      RowColIter begin() const
      {
         return RowColIter(this->nRows, this->nCols, this->topLeftRow, this->topLeftCol, 0);
      }

      RowColIter end() const
      {
         return RowColIter(this->nRows, this->nCols, this->topLeftRow, this->topLeftCol, this->nRows * this->nCols);
      }

   protected:
      value_type pos; // Row, col of current pos

      Index nRows;      // number of rows
      Index nCols;      // number of cols
      Index topLeftRow; // top let row
      Index topLeftCol; // top left col
      Index cur_flat;   // Current flat index

      // Constructor for begin / end
      RowColIter(Index nRows, Index nCols, Index topLeftRow, Index topLeftCol, Index cur_flat)
         : RowColIter(nRows, nCols, topLeftRow, topLeftCol)
      {
         this->cur_flat = cur_flat;
      }

      void update(Index movement)
      {
         this->cur_flat += movement;
         this->pos.first  = (this->cur_flat / this->nCols) + this->topLeftRow;
         this->pos.second = (this->cur_flat % this->nCols) + this->topLeftCol;
      }
   };

} // namespace KiLib
