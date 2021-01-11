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
      auto            n    = 100;
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
      auto            n    = 100;
      auto            vec1 = Random::rnorm(n, 3.2, 0.3, gen1);
      auto            vec2 = Random::rnorm(n, 3.2, 0.3, gen2);
      for (size_t i = 0; i < n; ++i)
      {
         ASSERT_EQ(vec1[i], vec2[i]);
      }
   }
} // namespace KiLib
