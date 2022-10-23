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
 
   /**
    * This is used so that we know our update calls for Porosity 
    * are going to work properly
    */
   double test_function_for_soil_passing(const KiLib::Soils::ISoil& s)
   {
      return s.GetPorosity();
   }

   TEST(Decorators, AllCalcs)
   {
      Soils::UserDefined ud;

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


      // Test the above information
      ASSERT_EQ(ud.GetDensityDry().GetConstant(), 890);


      const Soils::IDistributionModelDecorator& pc = Soils::PorosityCalc(Soils::DistributionModel(ud ,Soils::DistributionModelType::Normal));

      // We should be using the normal distribution porosity method now
      spdlog::debug(" PC: Grabbing Porosity");
      ASSERT_EQ(pc.GetPorosity(), 1001);

      Soils::FieldCapacityCalc fcc(pc);
      
      spdlog::debug("FCC: Grabbing Field Cap");
      ASSERT_EQ(fcc.GetFieldCapacity(), 52);

      spdlog::debug("FCC: Grabbing Porosity");
      ASSERT_EQ(fcc.GetPorosity(), 1001);


      Soils::DensityWetCalc dwc(fcc);

      spdlog::debug("DWC: Grabbing GetFieldCapacity");
      ASSERT_EQ(dwc.GetFieldCapacity(), 52);

      spdlog::debug("DWC: Grabbing GetPorosity");
      ASSERT_EQ(dwc.GetPorosity(), 1001);

      spdlog::debug("DWC: Grabbing GetDensityWet");
      ASSERT_EQ(dwc.GetDensityWet(), 42);
   }


   TEST(Decorators, DistributionModelTest)
   {
      // Create a user defined soil
      Soils::UserDefined ud;

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

      const double porosity_value = 50;

      ud.SetPorosity(porosity_value);

      auto dm = Soils::DistributionModel(ud ,Soils::DistributionModelType::Normal);

      // We should be using the normal distribution porosity method now
      ASSERT_EQ(dm.GetDensityDry().GetConstant(), 890);
      
      ASSERT_EQ(dm.GetPorosity(), porosity_value);


      KiLib::Soils::IDistributionModelDecorator &soil_base = dm;    

      ASSERT_EQ(test_function_for_soil_passing(soil_base), porosity_value);

      // Test uninitialized Values
      //EXPECT_THROW(ud.GetPorosity(), std::bad_optional_access);
 
      // Make sure the value was set
      //ASSERT_EQ(ud.GetPorosity(), porosity);
   }



   TEST(Decorators, PorosityCalcTest)
   {
      // Create a user defined soil
      Soils::UserDefined ud;
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

      const double porosity_value = 50;

      ud.SetPorosity(porosity_value);

      Soils::PorosityCalc pc(Soils::DistributionModel(ud ,Soils::DistributionModelType::Normal));

      // We should be using the normal distribution porosity method now
      ASSERT_EQ(pc.GetDensityDry().GetConstant(), 890);
      
      ASSERT_EQ(pc.GetPorosity(), 1001);


      KiLib::Soils::ISoil &soil_base = pc;    

      ASSERT_EQ(test_function_for_soil_passing(soil_base), 1001);

      // Test uninitialized Values
      //EXPECT_THROW(ud.GetPorosity(), std::bad_optional_access);
 
      // Make sure the value was set
      //ASSERT_EQ(ud.GetPorosity(), porosity);
   }


}