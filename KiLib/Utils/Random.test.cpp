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


/**
 * This file just makes sure that the functions are included in the library
 * properly
 */

#include <KiLib/Utils/Random.hpp>
#include <gtest/gtest.h>
#include <spdlog/spdlog.h>

namespace KiLib
{
   TEST(Random, runif)
   {
      std::mt19937_64 gen1(1);
      std::mt19937_64 gen2(1);
      size_t          n    = 100;
      auto            vec1 = Random::runif(n, 2.8, 3.2, gen1);
      auto            vec2 = Random::runif(n, 2.8, 3.2, gen2);
      for (size_t i = 0; i < n; ++i)
      {
         ASSERT_EQ(vec1[i], vec2[i]);
      }
   }

   TEST(Random, rnorm)
   {
      std::mt19937_64 gen1(1);
      std::mt19937_64 gen2(1);
      size_t          n    = 100;
      auto            vec1 = Random::rnorm(n, 3.2, 0.3, gen1);
      auto            vec2 = Random::rnorm(n, 3.2, 0.3, gen2);
      for (size_t i = 0; i < n; ++i)
      {
         ASSERT_EQ(vec1[i], vec2[i]);
      }
   }

   TEST(Random, gen)
   {
      std::mt19937_64 gen1(1);
      auto            vec1 = Random::rnorm(1, 3.2, 0.3, gen1)[0];
      auto            vec2 = Random::rnorm(1, 3.2, 0.3, gen1)[0];
      ASSERT_NE(vec1, vec2);
   }
} // namespace KiLib
