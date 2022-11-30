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


#include <KiLib/KiLib.hpp>
#include <gtest/gtest.h>
#include <spdlog/spdlog.h>


namespace KiLib
{
   TEST(Soil, getTest)
   {
      KiLib::Soils::Soil a = KiLib::Soils::factory.get("ch");
      KiLib::Soils::Soil b = KiLib::Soils::factory.get(KiLib::Soils::SoilID::ml);

      ASSERT_EQ(a.GetConductivityDistributon().GetUniformPrimula().GetMax(), 1e-06);
      ASSERT_EQ(a.GetLongName(), "Clay of high plasticity, fat clay");
      ASSERT_EQ(a.GetCohesionDistribution().GetConstant(), 25000.0);

      ASSERT_EQ(b.GetFrictionAngleDistribution().GetNormal().GetMean(), 33 * M_PI / 180.0);
      ASSERT_EQ(b.GetLongName(), "Silt");
      ASSERT_NE(b.GetLongName(), "SDJKFLJSdKLFJSDFKL");
   }
} // namespace KiLib