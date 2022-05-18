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


#include <KiLib/Utils/Vec3.hpp>
#include <gtest/gtest.h>

namespace KiLib
{
   TEST(Vec3, Construct)
   {
      auto vec = Vec3{1.0, 2.0, 3.0};
      ASSERT_DOUBLE_EQ(1.0, vec.x);
      ASSERT_DOUBLE_EQ(2.0, vec.y);
      ASSERT_DOUBLE_EQ(3.0, vec.z);
   }

   TEST(Vec3, AddOp)
   {
      auto v1 = Vec3{1.0, 2.0, 3.0};
      auto v2 = Vec3{5.0, 6.5, 9.0};
      auto d1 = 3.0;

      auto v3 = v1 + v2;
      ASSERT_EQ(v3, Vec3(6.0, 8.5, 12.0));

      auto v4 = v1 + d1;
      ASSERT_EQ(v4, Vec3(4.0, 5.0, 6.0));

      auto v5 = d1 + v1;
      ASSERT_EQ(v5, Vec3(4.0, 5.0, 6.0));

      auto v6 = v2;
      v6 += v1;
      ASSERT_EQ(v6, Vec3(6.0, 8.5, 12.0));

      auto v7 = v2;
      v7 += d1;
      ASSERT_EQ(v7, Vec3(8.0, 9.5, 12.0));
   }

   TEST(Vec3, SubOp)
   {
      auto v1 = Vec3{1.0, 2.0, 3.0};
      auto v2 = Vec3{5.0, 6.5, 9.0};
      auto d1 = 3.0;

      auto v3 = v1 - v2;
      ASSERT_EQ(v3, Vec3(-4.0, -4.5, -6.0));

      auto v4 = v1 - d1;
      ASSERT_EQ(v4, Vec3(-2.0, -1.0, 0.0));

      auto v5 = d1 - v1;
      ASSERT_EQ(v5, Vec3(2.0, 1.0, 0.0));

      auto v6 = v2;
      v6 -= v1;
      ASSERT_EQ(v6, Vec3(4.0, 4.5, 6.0));

      auto v7 = v2;
      v7 -= d1;
      ASSERT_EQ(v7, Vec3(2.0, 3.5, 6.0));
   }

   TEST(Vec3, MulOp)
   {
      auto v1 = Vec3{1.0, 2.0, 3.0};
      auto v2 = Vec3{5.0, 6.5, 9.0};
      auto d1 = 3.0;

      auto v3 = v1 * v2;
      ASSERT_EQ(v3, Vec3(5.0, 13.0, 27.0));

      auto v4 = v1 * d1;
      ASSERT_EQ(v4, Vec3(3.0, 6.0, 9.0));

      auto v5 = d1 * v1;
      ASSERT_EQ(v5, Vec3(3.0, 6.0, 9.0));

      auto v6 = v2;
      v6 *= v1;
      ASSERT_EQ(v6, Vec3(5.0, 13.0, 27.0));

      auto v7 = v2;
      v7 *= d1;
      ASSERT_EQ(v7, Vec3(15.0, 19.5, 27.0));
   }

   TEST(Vec3, DivOp)
   {
      auto v1 = Vec3{1.0, 2.0, 3.0};
      auto v2 = Vec3{5.0, 6.5, 9.0};
      auto d1 = 3.0;

      auto v3 = v1 / v2;
      ASSERT_EQ(v3, Vec3(0.2, 2.0/6.5, 1.0/3.0));

      auto v4 = v1 / d1;
      ASSERT_EQ(v4, Vec3(1.0/3.0, 2.0/3.0, 1.0));

      auto v5 = d1 / v1;
      ASSERT_EQ(v5, Vec3(3.0, 3.0/2.0, 1.0));

      auto v6 = v2;
      v6 /= v1;
      ASSERT_EQ(v6, Vec3(5.0, 6.5/2.0, 3.0));

      auto v7 = v2;
      v7 /= d1;
      ASSERT_EQ(v7, Vec3(5.0/3.0, 6.5/3.0, 3.0));
   }

   TEST(Vec3, EqOp)
   {
      auto v1 = Vec3{1.0, 2.0, 3.0};
      auto v2 = Vec3{5.0, 6.5, 9.0};
      auto v3 = Vec3{1.0, 2.0, 3.0};

      ASSERT_EQ(v1, v3);
      ASSERT_NE(v1, v2);
      ASSERT_NE(v3, v2);
   }
} // namespace KiLib
