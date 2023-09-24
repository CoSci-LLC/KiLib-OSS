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

#define TESTING_DELTA 0.0000000001

#include <KiLib/Utils/StatisticalOutput.hpp>
#include <gtest/gtest.h>
#include <memory>

namespace KiLib
{
   TEST(StatisticalOutput, OnTheFlyMean)
   {
      auto so = std::make_unique<KiLib::Utils::StatisticalOutput>();
      so->AddData(1.2);
      so->AddData(2.6);
      so->AddData(5.4);
      so->AddData(6.3);
      so->AddData(84.23);
      so->AddData(2.62);
      so->AddData(21.12);
      ASSERT_NEAR(so->GetMean(), 17.638571428571428, TESTING_DELTA);
   }


   TEST(StatisticalOutput, OnTheFlyStdDev)
   {
      auto so = std::make_unique<KiLib::Utils::StatisticalOutput>();
      so->AddData(1.2);
      so->AddData(2.6);
      so->AddData(5.4);
      so->AddData(6.3);
      so->AddData(84.23);
      so->AddData(2.62);
      so->AddData(21.12);
      ASSERT_NEAR(so->GetStdDev(), 27.894769826500983, TESTING_DELTA);
   }
} // namespace KiLib