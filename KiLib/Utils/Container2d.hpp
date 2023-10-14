#pragma once

#include <cstddef>
#include <stdexcept>
#include <vector>

namespace KiLib::Utils
{
   template <typename T> class Container2d
   {
   public:
      Container2d(size_t rows, size_t cols)
      {
         this->data = std::vector<std::vector<T>>(rows, std::vector<T>(cols, T()));
      }

      T &operator()(size_t row, size_t col)
      {
         this->_rBoundCheck(row);
         this->_cBoundCheck(col);

         return this->data[row][col];
      }

      const T &operator()(size_t row, size_t col) const
      {
         this->_rBoundCheck(row);
         this->_cBoundCheck(col);
         return this->data[row][col];
      }

      T &at(size_t row, size_t col)
      {
         this->_rBoundCheck(row);
         this->_cBoundCheck(col);
         return this->data[row][col];
      }

      const T &at(size_t row, size_t col) const
      {
         this->_rBoundCheck(row);
         this->_cBoundCheck(col);
         return this->data[row][col];
      }

      std::vector<T> &operator[](size_t row)
      {
         this->_rBoundCheck(row);
         return this->data[row];
      }

      const std::vector<T> &operator[](size_t row) const
      {
         this->_rBoundCheck(row);
         return this->data[row];
      }

      size_t rows() const
      {
         return this->data.size();
      }

      size_t cols() const
      {
         return this->data.empty() ? 0 : this->data[0].size();
      }

      size_t size() const
      {
         return this->rows() * this->cols();
      }

      void _rBoundCheck(size_t col) const
      {
         if (col >= this->rows())
         {
            throw std::out_of_range("Container2d | Row index out of range");
         }
      }

      void _cBoundCheck(size_t col) const
      {
         if (col >= this->cols())
         {
            throw std::out_of_range("Container2d | Col index out of range");
         }
      }

   private:
      std::vector<std::vector<T>> data;
   };
} // namespace KiLib::Utils
