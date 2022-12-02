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
#include <KiLib/Soils/DensityDryGen.hpp>

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
      ud.SetCohesionDistribution({
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

      ud.SetFrictionAngleDistribution({
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

      ud.SetDensityDryDistribution({
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
               .mean   = 1100,
               .stdDev = 0,
            }});


      // Test the above information
      ASSERT_EQ(ud.GetDensityDryDistribution().GetNormal().GetMean(), 1100);
      const double porosity_standard_value = -0.41471698113207545;

      std::random_device rand_device;
      std::mt19937_64 gen(rand_device());
      //spdlog::debug("Starting soil: {:X}", (long)&ud);
      auto dm = Soils::DistributionModel(ud ,Soils::DistributionModelType::Normal);
      Soils::DensityDryGen ddg(dm, gen);
      ddg.GenerateDensityDry();

      const Soils::IDistributionModelDecorator& pc = Soils::PorosityCalc(ddg);

      // We should be using the normal distribution porosity method now
      //spdlog::debug(" PC: Grabbing Porosity");
      ASSERT_EQ(pc.GetPorosity(), porosity_standard_value);

      Soils::FieldCapacityCalc fcc(pc);
      
      //spdlog::debug("FCC: Grabbing Field Cap");
      ASSERT_EQ(fcc.GetFieldCapacity(), 52);

      //spdlog::debug("FCC: Grabbing Porosity");
      ASSERT_EQ(fcc.GetPorosity(), porosity_standard_value);


      Soils::DensityWetCalc dwc(fcc);

      //spdlog::debug("DWC: Grabbing GetFieldCapacity");
      ASSERT_EQ(dwc.GetFieldCapacity(), 52);

      //spdlog::debug("DWC: Grabbing GetPorosity");
      ASSERT_EQ(dwc.GetPorosity(), porosity_standard_value);

      //spdlog::debug("DWC: Grabbing GetDensityWet");
      ASSERT_EQ(dwc.GetDensityWet(), 42);
   }


   TEST(Decorators, DistributionModelTest)
   {
      // Create a user defined soil
      Soils::UserDefined ud;

      // Add the required info for SlideforNET use case
      ud.SetCohesionDistribution({
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

      ud.SetFrictionAngleDistribution({
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

      ud.SetDensityDryDistribution({
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
      ASSERT_EQ(dm.GetDensityDryDistribution().GetConstant(), 890);
      
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
      ud.SetCohesionDistribution({
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

      ud.SetFrictionAngleDistribution({
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

      ud.SetDensityDryDistribution({
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
               .mean   = 1100,
               .stdDev = 0,
            }});

      const double porosity_value = 50;
      const double porosity_standard_value = 1-890/2650;

      ud.SetPorosity(porosity_value);

      std::random_device rand_device;
      std::mt19937_64 gen(rand_device());
      auto dm = Soils::DistributionModel(ud ,Soils::DistributionModelType::Normal);
      Soils::DensityDryGen ddg(dm, gen);
      ddg.GenerateDensityDry();

      Soils::PorosityCalc pc(ddg);

      
      // We should be using the normal distribution porosity method now
      ASSERT_EQ(pc.GetDensityDryDistribution().GetConstant(), 890);
      
      ASSERT_EQ(pc.GetPorosity(), porosity_standard_value);


      KiLib::Soils::ISoil &soil_base = pc;    

      ASSERT_EQ(test_function_for_soil_passing(soil_base), porosity_standard_value);

      // Test uninitialized Values
      //EXPECT_THROW(ud.GetPorosity(), std::bad_optional_access);
 
      // Make sure the value was set
      //ASSERT_EQ(ud.GetPorosity(), porosity);
   }


   TEST(Decorators, TestPorosityWithDensityDry)
   {
      Soils::UserDefined ud;

      // Add the required info for SlideforNET use case
      ud.SetCohesionDistribution({
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

      ud.SetFrictionAngleDistribution({
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

      ud.SetDensityDryDistribution({
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


      std::random_device rand_device;
      std::mt19937_64 gen(rand_device());

      auto dm = Soils::DistributionModel(ud ,Soils::DistributionModelType::Normal);
      Soils::DensityDryGen ddg(dm, gen);

      ddg.GenerateDensityDry();

      // We should be using the normal distribution porosity method now
      //spdlog::debug("DensityDry: {}", ddg.GetDensityDry());

      const Soils::IDistributionModelDecorator& pc = Soils::PorosityCalc(ddg);

      // ddg.GenerateDensityDry(); //return a value as well
      spdlog::debug("PorosityCalc: {}", pc.GetPorosity());

   }
}
