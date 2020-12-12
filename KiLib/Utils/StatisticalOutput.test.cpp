#include <KiLib/Utils/StatisticalOutput.hpp>
#include <gtest/gtest.h>
#include <memory>
#include <test/goldValues.hpp>

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