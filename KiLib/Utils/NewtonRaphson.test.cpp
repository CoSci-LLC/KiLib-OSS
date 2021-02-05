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


#include <KiLib/Utils/NewtonRaphson.hpp>
#include <gtest/gtest.h>
#include <spdlog/spdlog.h>

namespace KiLib
{
   TEST(NewtonRaphson, PowerFunctionTest1)
   {
      auto f  = [](double x) -> double { return std::pow(x, 6) - x - 1; };
      auto df = [](double x) -> double { return 6 * std::pow(x, 5) - 1; };
      ASSERT_LT(std::abs(NewtonRaphson::NewtonRaphsonAnalytic(f, df, 1.0, 1.0e-08, 100000) - 1.134724138), 1e-08);
   }

} // namespace KiLib
