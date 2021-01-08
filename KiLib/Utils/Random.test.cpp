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
      std::mt19937_64 gen(1);
      Random::runif(5, 3.2, 3.2, gen);
   }
   TEST(Random, rnorm)
   {
      std::mt19937_64 gen(1);
      Random::rnorm(5, 3.2, 3.2, gen);
   }
} // namespace KiLib