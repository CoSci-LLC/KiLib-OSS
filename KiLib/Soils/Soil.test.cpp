#include <KiLib/Soils/SoilType.hpp>
#include <gtest/gtest.h>
#include <spdlog/spdlog.h>


namespace KiLib
{
   TEST(Soil, getTest)
   {
      KiLib::SoilType::get("ch");
   }
} // namespace KiLib