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


#include <KiLib/Utils/Sequence.hpp>
#include <gtest/gtest.h>

namespace KiLib
{
   TEST(Sequence, int)
   {
      std::vector<int> result = Sequence::Build(0, 5, 1);
      ASSERT_EQ(result[0], 0);
      ASSERT_EQ(result[1], 1);
      ASSERT_EQ(result[2], 2);
      ASSERT_EQ(result[3], 3);
      ASSERT_EQ(result[4], 4);
      ASSERT_EQ(result[5], 5);
   }

   TEST(Sequence, double)
   {
      std::vector<double> result = Sequence::Build<double>(0, 2, 0.5);
      ASSERT_EQ(result.size(), 5);
      ASSERT_DOUBLE_EQ(result[0], 0);
      ASSERT_DOUBLE_EQ(result[1], 0.5);
      ASSERT_DOUBLE_EQ(result[2], 1.0);
      ASSERT_DOUBLE_EQ(result[3], 1.5);
      ASSERT_DOUBLE_EQ(result[4], 2.0);
   }


   TEST(Sequence, double_non_zero)
   {
      std::vector<double> result = Sequence::Build<double>(2, 4, 0.5);
      ASSERT_EQ(result.size(), 5);
      ASSERT_DOUBLE_EQ(result[0], 2);
      ASSERT_DOUBLE_EQ(result[1], 2.5);
      ASSERT_DOUBLE_EQ(result[2], 3.0);
      ASSERT_DOUBLE_EQ(result[3], 3.5);
      ASSERT_DOUBLE_EQ(result[4], 4.0);
   }
   TEST(Sequence, double_odd)
   {
      std::vector<double> result = Sequence::Build<double>(2, 4.5, 0.5);
      ASSERT_EQ(result.size(), 6);
      ASSERT_DOUBLE_EQ(result[0], 2);
      ASSERT_DOUBLE_EQ(result[1], 2.5);
      ASSERT_DOUBLE_EQ(result[2], 3.0);
      ASSERT_DOUBLE_EQ(result[3], 3.5);
      ASSERT_DOUBLE_EQ(result[4], 4.0);
      ASSERT_DOUBLE_EQ(result[5], 4.5);
   }


   TEST(Sequence, double_reverse)
   {
      std::vector<double> result = Sequence::Build<double>(2, 0, -0.5);
      ASSERT_DOUBLE_EQ(result[0], 2.0);
      ASSERT_DOUBLE_EQ(result[1], 1.5);
      ASSERT_DOUBLE_EQ(result[2], 1.0);
      ASSERT_DOUBLE_EQ(result[3], 0.5);
      ASSERT_DOUBLE_EQ(result[4], 0);
   }

   TEST(Sequence, double_reverse_non_zero)
   {
      std::vector<double> result = Sequence::Build<double>(4, 2, -0.5);
      ASSERT_DOUBLE_EQ(result[0], 4.0);
      ASSERT_DOUBLE_EQ(result[1], 3.5);
      ASSERT_DOUBLE_EQ(result[2], 3.0);
      ASSERT_DOUBLE_EQ(result[3], 2.5);
      ASSERT_DOUBLE_EQ(result[4], 2.0);
   }

} // namespace KiLib