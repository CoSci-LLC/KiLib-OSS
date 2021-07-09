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


#include <KiLib/Exceptions/InvalidArgument.hpp>
#include <KiLib/KiLib.hpp>
#include <gtest/gtest.h>
#include <spdlog/spdlog.h>

namespace KiLib
{
   TEST(UserDefinedSoil, TestStandardParameters)
   {
      KiLib::Soils::UserDefined ud;

      ASSERT_EQ(ud.GetName(), "ud");
      ASSERT_EQ(ud.GetLongName(), "User Defined Soil");

      std::string new_name = "My Soil";

      ud.SetName(new_name);

      ASSERT_EQ(ud.GetName(), new_name);

      // Test uninitialized Values
      EXPECT_THROW(ud.GetPorosity(), InvalidArgumentException);

      // Set value
      const double porosity = 54.32;
      ud.SetPorosity(porosity); // Just a random value

      // Make sure the value was set
      ASSERT_EQ(ud.GetPorosity(), porosity);
   }

   TEST(UserDefinedSoil, Test_UserDefined_Value_Distributions)
   {
      // Create user defined soil
      KiLib::Soils::UserDefined ud;

      EXPECT_THROW(ud.GetFrictionAngle().GetConstant(), InvalidArgumentException);

      // Create uniform values (1)
      KiLib::Soils::UserDefinedValueUniform udvu1;
      EXPECT_THROW(udvu1.GetMin(), InvalidArgumentException);
      const double min = 1.4;
      udvu1.SetMin(min);
      ASSERT_EQ(udvu1.GetMin(), min);
      udvu1.SetMax(5.7);

      // Create uniform values (2)
      KiLib::Soils::UserDefinedValueUniform udvu2;
      udvu2.SetMin(7.9);
      udvu2.SetMax(10.3);

      // Create normal value
      KiLib::Soils::UserDefinedValueNormal udvn;
      EXPECT_THROW(udvn.GetMean(), InvalidArgumentException);
      udvn.SetMean(5.3);
      udvn.SetStdDev(1.1);

      // Create example distribution
      KiLib::Soils::UserDefinedValueDistribution udvd;
      udvd.SetUniformPrimula(udvu1);
      udvd.SetUniform(udvu2);
      udvd.SetNormal(udvn);
      EXPECT_THROW(udvd.GetConstant(), InvalidArgumentException);
      udvd.SetConstant(4.3);
      ASSERT_EQ(udvd.GetConstant(), 4.3);

      // Make sure distribution carries over the value uniform
      ASSERT_EQ(udvd.GetUniformPrimula().GetMin(), min);

      // Test the distribution within the user defined soil
      ud.SetFrictionAngle(udvd);

      // Check that all previous values are set properly
      // udvu1
      ASSERT_EQ(ud.GetFrictionAngle().GetUniformPrimula().GetMin(), min);
      ASSERT_EQ(ud.GetFrictionAngle().GetUniformPrimula().GetMax(), 5.7);

      // udvu2
      ASSERT_EQ(ud.GetFrictionAngle().GetUniform().GetMin(), 7.9);
      ASSERT_EQ(ud.GetFrictionAngle().GetUniform().GetMax(), 10.3);

      // udvn
      ASSERT_EQ(ud.GetFrictionAngle().GetNormal().GetMean(), 5.3);
      ASSERT_EQ(ud.GetFrictionAngle().GetNormal().GetStdDev(), 1.1);

      // udvd
      ASSERT_EQ(ud.GetFrictionAngle().GetConstant(), 4.3);
   }
} // namespace KiLib