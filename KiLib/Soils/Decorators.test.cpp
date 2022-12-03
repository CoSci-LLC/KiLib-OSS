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
#include <KiLib/Soils/ResidualWaterContentCalc.hpp>
#include <KiLib/Soils/FieldCapacityCalc.hpp>
#include <KiLib/Soils/UserDefined.hpp>
#include <KiLib/Soils/DistributionModel.hpp>
#include <KiLib/Soils/DensityDryGen.hpp>
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

      ud.SetVgWetAlpha1(0.0001);
      ud.SetVgWetN1(1.3);
  
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
               .mean   = 1450,
               .stdDev = 100,
            }});


      // Test the above information
      ASSERT_EQ(ud.GetDensityDryDistribution().GetNormal().GetMean(), 1450);

      std::random_device rand_device;
      std::mt19937_64 gen( 5489U ); // To always get the same seed.
      //std::mt19937_64 gen(rand_device());
      //std::mt19937_64 gen(time(NULL));

      //spdlog::debug("Starting soil: {:X}", (long)&ud);
      auto dm = Soils::DistributionModel(ud ,Soils::DistributionModelType::Normal);
      Soils::DensityDryGen ddg(dm, gen);
      ddg.GenerateDensityDry();
      SPDLOG_DEBUG("densityDry = {}", ddg.GetDensityDry());
      ASSERT_EQ(ddg.GetDensityDry(), 1379.6906200167286);

      Soils::PorosityCalc pc(ddg);
      auto porosity = pc.CalculatePorosity();
      SPDLOG_DEBUG("porosity       = {}", porosity);
      SPDLOG_DEBUG("pc.GetPorosity = {}", pc.GetPorosity());
      ASSERT_EQ(pc.GetPorosity(), 0.47936203018236656);
      
      // Check that get method returns same value since last calculation
      ASSERT_EQ(pc.GetPorosity(), porosity);

      Soils::ResidualWaterContentCalc rwcc(pc);
      const auto residualWaterContent = rwcc.CalculateResidualWaterContent();
      SPDLOG_DEBUG("residualWaterContent         = {}", residualWaterContent);
      SPDLOG_DEBUG("rwcc.GetResidualWaterContent = {}", rwcc.GetResidualWaterContent());
      ASSERT_EQ(rwcc.GetResidualWaterContent(), 0.047936203018236656);

      // Check that get method returns same value since last calculation
      ASSERT_EQ(rwcc.GetResidualWaterContent(), residualWaterContent);

      Soils::FieldCapacityCalc fcc(rwcc);
      SPDLOG_DEBUG("FCC: Grabbing Field Capacity");
      const auto fieldCapacity = fcc.CalculateFieldCapacity(33000);
      SPDLOG_DEBUG("fieldCapacity        = {}", fieldCapacity);
      SPDLOG_DEBUG("fcc.GetFieldCapacity = {}", fcc.GetFieldCapacity());
      ASSERT_EQ(fcc.GetFieldCapacity(), 0.3364047213531462);

      // Check that get method returns same value since last calculation
      ASSERT_EQ(fcc.GetFieldCapacity(), fieldCapacity);

      Soils::DensityWetCalc dwc(fcc);
      SPDLOG_DEBUG("DWC: Grabbing Density Wet");
      auto densityWet = dwc.CalculateDensityWet(0.0);
      SPDLOG_DEBUG("densityWet S = 0   = {}", densityWet);
      SPDLOG_DEBUG("dwc.GetDensityWet  = {}", dwc.GetDensityWet());
      ASSERT_EQ(dwc.GetDensityWet(), 1716.0953413698749);
      
      // Check that get method returns same value since last calculation
      ASSERT_EQ(dwc.GetDensityWet(), densityWet);

      densityWet = dwc.CalculateDensityWet(0.8);
      SPDLOG_DEBUG("densityWet S = 0.8 = {}", densityWet);
      SPDLOG_DEBUG("dwc.GetDensityWet  = {}", dwc.GetDensityWet());
      ASSERT_EQ(dwc.GetDensityWet(), 1830.4611884332512);

      // Check that get method returns same value since last calculation
      ASSERT_EQ(dwc.GetDensityWet(), densityWet);

      densityWet = dwc.CalculateDensityWet(1.0);
      SPDLOG_DEBUG("densityWet S = 1   = {}", densityWet);
      SPDLOG_DEBUG("dwc.GetDensityWet  = {}", dwc.GetDensityWet());
      ASSERT_EQ(dwc.GetDensityWet(), 1859.0526501990953);

      // Check that get method returns same value since last calculation
      ASSERT_EQ(dwc.GetDensityWet(), densityWet);

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
               .max = 1601.3
            },
            .uniform{
               .min = 1100,
               .max = 1400
            },
            .normal{
               .mean   = 900,
               .stdDev = 200
            }});

      std::random_device rand_device;
      std::mt19937_64 gen( 5489U ); // To always get the same seed.

      auto dm = Soils::DistributionModel(ud ,Soils::DistributionModelType::Uniform);
      ASSERT_EQ(dm.GetDensityDryDistribution().GetConstant(), 890);

      Soils::DensityDryGen ddg(dm, gen);
      ddg.GenerateDensityDry();
      SPDLOG_DEBUG("densityDry = {}", ddg.GetDensityDry());
      ASSERT_EQ(ddg.GetDensityDry(), 1336.0462864603405);
      
      Soils::PorosityCalc pc(ddg);
      auto porosity = pc.CalculatePorosity();
      SPDLOG_DEBUG("Porosity = {}", porosity);
      ASSERT_EQ(pc.GetPorosity(), 0.4958315900149658);

      // Make sure we get the same value after multiple calls to get
      ASSERT_EQ(pc.GetPorosity(), porosity);


      //KiLib::Soils::IDistributionModelDecorator &soil_base = dm;    

      //ASSERT_EQ(test_function_for_soil_passing(soil_base), porosity_value);

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

      const double porosity_value = 0.50;

      ud.SetPorosity(porosity_value);
      SPDLOG_DEBUG("Original porosity = {}", ud.GetPorosity());

      std::random_device rand_device;
      std::mt19937_64 gen( 5489U ); // To always get the same seed.

      auto dm = Soils::DistributionModel(ud ,Soils::DistributionModelType::Normal);
      Soils::DensityDryGen ddg(dm, gen);
      ddg.GenerateDensityDry();

      Soils::PorosityCalc pc(ddg);

      SPDLOG_DEBUG("Getting porosity");
      auto porosity = pc.CalculatePorosity();
      SPDLOG_DEBUG("Porosity = {}", porosity);
      ASSERT_EQ(pc.GetPorosity(), 0.58490566037735847);

      
      // We should be using the normal distribution porosity method now
      ASSERT_EQ(pc.GetDensityDryDistribution().GetConstant(), 890);
      
      KiLib::Soils::ISoil &soil_base = pc;    

      ASSERT_EQ(test_function_for_soil_passing(soil_base), porosity);

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
      std::mt19937_64 gen( 5489U ); // To always get the same seed.

      auto dm = Soils::DistributionModel(ud ,Soils::DistributionModelType::Normal);
      Soils::DensityDryGen ddg(dm, gen);

      ddg.GenerateDensityDry();

      // We should be using the normal distribution porosity method now
      //spdlog::debug("DensityDry: {}", ddg.GetDensityDry());

      Soils::PorosityCalc pc(ddg);

      auto porosity = pc.CalculatePorosity();

      ASSERT_EQ(porosity, pc.GetPorosity());

      // ddg.GenerateDensityDry(); //return a value as well
      SPDLOG_DEBUG("PorosityCalc: {}", pc.GetPorosity());

   }
}
