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
#include <KiLib/Soils/PorosityCalc.hpp>
#include <KiLib/Soils/FieldCapacityCalc.hpp>
#include <KiLib/Soils/UserDefined.hpp>
#include <KiLib/Soils/DistributionModel.hpp>
#include <KiLib/Soils/DensityWetCalc.hpp>

namespace KiLib
{
   TEST(PorosityDecorator, InitializationNormalDist)
   {
      // Create a user defined soil
      Soils::PorosityCalc<Soils::DistributionModel<Soils::UserDefined>> ud(Soils::DistributionModelType::Normal);

      // Add the required info for SlideforNET use case
      ud.SetCohesion({
            .constant = 10000,
            .uniformPrimula{
               .min = 20000,
               .max = 40000,
            },
            .uniform{
               .min = 5000,
               .max = 5000,
            },
            .normal{
               .mean   = 10000,
               .stdDev = 5000,
            }});

      ud.SetFrictionAngle({
            .constant = 22 * M_PI / 180.0,
            .uniformPrimula{
               .min = 14 * M_PI / 180.0,
               .max = 24 * M_PI / 180.0,
            },
            .uniform{
               .min = 14 * M_PI / 180.0,
               .max = 14 * M_PI / 180.0,
            },
            .normal{
               .mean   = 22 * M_PI / 180.0,
               .stdDev = 5 * M_PI / 180.0,
            }});

      ud.SetDensityDry({
            .constant = 890,
            .uniformPrimula{
               .min = 1040.84,
               .max = 1601.3,
            },
            .uniform{
               .min = 800,
               .max = 800,
            },
            .normal{
               .mean   = 900,
               .stdDev = 200,
            }});

      // We should be using the normal distribution porosity method now
      ASSERT_EQ(ud.GetPorosity(), 1001);
      ASSERT_EQ(ud.GetDensityDry().GetConstant(), 890);

      // Test uninitialized Values
      //EXPECT_THROW(ud.GetPorosity(), std::bad_optional_access);
 
      // Make sure the value was set
      //ASSERT_EQ(ud.GetPorosity(), porosity);
   }


   TEST(Decorators, AllCalcs)
   {
      // Create a user defined soil
      Soils::DensityWetCalc<Soils::FieldCapacityCalc<Soils::PorosityCalc<Soils::DistributionModel<Soils::UserDefined>>>> ud(Soils::DistributionModelType::Normal);

      // Add the required info for SlideforNET use case
      ud.SetCohesion({
            .constant = 10000,
            .uniformPrimula{
               .min = 20000,
               .max = 40000,
            },
            .uniform{
               .min = 5000,
               .max = 5000,
            },
            .normal{
               .mean   = 10000,
               .stdDev = 5000,
            }});

      ud.SetFrictionAngle({
            .constant = 22 * M_PI / 180.0,
            .uniformPrimula{
               .min = 14 * M_PI / 180.0,
               .max = 24 * M_PI / 180.0,
            },
            .uniform{
               .min = 14 * M_PI / 180.0,
               .max = 14 * M_PI / 180.0,
            },
            .normal{
               .mean   = 22 * M_PI / 180.0,
               .stdDev = 5 * M_PI / 180.0,
            }});

      ud.SetDensityDry({
            .constant = 890,
            .uniformPrimula{
               .min = 1040.84,
               .max = 1601.3,
            },
            .uniform{
               .min = 800,
               .max = 800,
            },
            .normal{
               .mean   = 900,
               .stdDev = 200,
            }});

      // We should be using the normal distribution porosity method now
      ASSERT_EQ(ud.GetPorosity(), 1001);
      ASSERT_EQ(ud.GetDensityDry().GetConstant(), 890);
      ASSERT_EQ(ud.GetFieldCapacity(), 52);
      ASSERT_EQ(ud.GetDensityWet(), 42);

      // Test uninitialized Values
      //EXPECT_THROW(ud.GetPorosity(), std::bad_optional_access);
 
      // Make sure the value was set
      //ASSERT_EQ(ud.GetPorosity(), porosity);
   }
}