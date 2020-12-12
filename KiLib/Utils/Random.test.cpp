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
      Random::runif(5, 3.2, 3.2);
   }
   TEST(Random, rnorm)
   {
      Random::rnorm(5, 3.2, 3.2);
   }
} // namespace KiLib