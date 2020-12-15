#include <KiLib/Utils/NewtonRaphson.hpp>
#include <spdlog/spdlog.h>
#include <gtest/gtest.h>

namespace KiLib
{
   TEST(NewtonRaphson, PowerFunctionTest1)
   {
      spdlog::set_level(spdlog::level::info);
      auto f  = [](double x) -> double { return std::pow(x, 6) - x - 1; };
      auto df = [](double x) -> double { return 6 * std::pow(x, 5) - 1; };
      ASSERT_LT(std::abs(NewtonRaphson::NewtonRaphsonAnalytic(f, df, 1.0, 1.0e-08, 100000) - 1.134724138), 1e-08);
   }

} // namespace KiLib
