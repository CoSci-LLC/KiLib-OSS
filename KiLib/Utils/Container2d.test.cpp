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


#include <KiLib/Utils/Container2d.hpp>
#include <cstddef>
#include <gtest/gtest.h>

namespace KiLib
{
   // Templated function that builds a 2d container of a given type
   // With a given number of rows and columns, values [0, 1, 2, ...]

   template <typename T>
   KiLib::Utils::Container2d<T> build2dContainer(size_t rows, size_t cols)
   {
      KiLib::Utils::Container2d<T> matrix(rows, cols);

      for (size_t i = 0; i < matrix.rows(); ++i)
      {
         for (size_t j = 0; j < matrix.cols(); ++j)
         {
            matrix(i, j) = i * matrix.cols() + j;
         }
      }

      return matrix;
   }

   TEST(Container2d, Insert_SizeT)
   {
      std::vector<size_t> ground_truth = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19};

      auto matrix = build2dContainer<size_t>(4, 5);

      GTEST_ASSERT_EQ(matrix.rows(), 4);
      GTEST_ASSERT_EQ(matrix.cols(), 5);
      GTEST_ASSERT_EQ(matrix.size(), 20);

      // Check the values
      for (size_t i = 0; i < matrix.rows(); ++i)
      {
         for (size_t j = 0; j < matrix.cols(); ++j)
         {
            GTEST_ASSERT_EQ(matrix(i, j), ground_truth[i * matrix.cols() + j]);
         }
      }
   }
   
   TEST(Container2d, Insert_float)
   {
      std::vector<float> ground_truth = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18};
      const size_t r = 3;
      const size_t c = 6;

      auto matrix = build2dContainer<float>(r, c);

      GTEST_ASSERT_EQ(matrix.rows(), r);
      GTEST_ASSERT_EQ(matrix.cols(), c);
      GTEST_ASSERT_EQ(matrix.size(), r*c);

      // Check the values
      for (size_t i = 0; i < matrix.rows(); ++i)
      {
         for (size_t j = 0; j < matrix.cols(); ++j)
         {
            GTEST_ASSERT_EQ(matrix(i, j), ground_truth[i * matrix.cols() + j]);
         }
      }
   }

   TEST(Container2d, BoundsCheck)
   {
      const size_t r = 3;
      const size_t c = 6;
      auto matrix = build2dContainer<float>(r, c);

      GTEST_ASSERT_EQ(matrix.rows(), r);
      GTEST_ASSERT_EQ(matrix.cols(), c);
      GTEST_ASSERT_EQ(matrix.size(), r*c);

      ASSERT_THROW(matrix(r, c), std::out_of_range);
      ASSERT_THROW(matrix(-1, 2), std::out_of_range);
      ASSERT_THROW(matrix(2, -1), std::out_of_range);
      ASSERT_THROW(matrix(-1, -1), std::out_of_range);
      ASSERT_THROW(matrix(r-1, c), std::out_of_range);

      ASSERT_NO_THROW(matrix(r-1, c-1));
      ASSERT_NO_THROW(matrix(0, 0));
   }

   TEST(Container2d, Mutation)
   {
      const size_t r = 10;
      const size_t c = 20;
      auto matrix = build2dContainer<float>(r, c);

      GTEST_ASSERT_EQ(matrix.rows(), r);
      GTEST_ASSERT_EQ(matrix.cols(), c);
      GTEST_ASSERT_EQ(matrix.size(), r*c);

      // Check the values
      size_t counter = 0;
      for (size_t i = 0; i < matrix.rows(); ++i)
      {
         for (size_t j = 0; j < matrix.cols(); ++j)
         {
            GTEST_ASSERT_EQ(matrix(i, j), counter);
            ++counter;
         }
      }

      // Mutate the values
      for (size_t i = 0; i < matrix.rows(); ++i)
      {
         for (size_t j = 0; j < matrix.cols(); ++j)
         {
            matrix(i, j) += 1;
         }
      }

      // Check the values
      counter = 0;
      for (size_t i = 0; i < matrix.rows(); ++i)
      {
         for (size_t j = 0; j < matrix.cols(); ++j)
         {
            GTEST_ASSERT_EQ(matrix(i, j), counter + 1);
            ++counter;
         }
      }
   }
} // namespace KiLib
