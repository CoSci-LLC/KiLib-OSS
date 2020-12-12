#include <KiLib/Utils/Sequence.hpp>
#include <gtest/gtest.h>

namespace KiLib
{
   TEST(Sequence, int)
   {
      std::vector<int> result = Sequence::Build(0, 5, 1);
      ASSERT_EQ(result[0], 0);
      ASSERT_EQ(result[1], 1);
      ASSERT_EQ(result[2], 2);
      ASSERT_EQ(result[3], 3);
      ASSERT_EQ(result[4], 4);
      ASSERT_EQ(result[5], 5);
   }

   TEST(Sequence, double)
   {
      std::vector<double> result = Sequence::Build<double>(0, 2, 0.5);
      ASSERT_EQ(result.size(), 5);
      ASSERT_DOUBLE_EQ(result[0], 0);
      ASSERT_DOUBLE_EQ(result[1], 0.5);
      ASSERT_DOUBLE_EQ(result[2], 1.0);
      ASSERT_DOUBLE_EQ(result[3], 1.5);
      ASSERT_DOUBLE_EQ(result[4], 2.0);
   }


   TEST(Sequence, double_non_zero)
   {
      std::vector<double> result = Sequence::Build<double>(2, 4, 0.5);
      ASSERT_EQ(result.size(), 5);
      ASSERT_DOUBLE_EQ(result[0], 2);
      ASSERT_DOUBLE_EQ(result[1], 2.5);
      ASSERT_DOUBLE_EQ(result[2], 3.0);
      ASSERT_DOUBLE_EQ(result[3], 3.5);
      ASSERT_DOUBLE_EQ(result[4], 4.0);
   }
   TEST(Sequence, double_odd)
   {
      std::vector<double> result = Sequence::Build<double>(2, 4.5, 0.5);
      ASSERT_EQ(result.size(), 6);
      ASSERT_DOUBLE_EQ(result[0], 2);
      ASSERT_DOUBLE_EQ(result[1], 2.5);
      ASSERT_DOUBLE_EQ(result[2], 3.0);
      ASSERT_DOUBLE_EQ(result[3], 3.5);
      ASSERT_DOUBLE_EQ(result[4], 4.0);
      ASSERT_DOUBLE_EQ(result[5], 4.5);
   }


   TEST(Sequence, double_reverse)
   {
      std::vector<double> result = Sequence::Build<double>(2, 0, -0.5);
      ASSERT_DOUBLE_EQ(result[0], 2.0);
      ASSERT_DOUBLE_EQ(result[1], 1.5);
      ASSERT_DOUBLE_EQ(result[2], 1.0);
      ASSERT_DOUBLE_EQ(result[3], 0.5);
      ASSERT_DOUBLE_EQ(result[4], 0);
   }

   TEST(Sequence, double_reverse_non_zero)
   {
      std::vector<double> result = Sequence::Build<double>(4, 2, -0.5);
      ASSERT_DOUBLE_EQ(result[0], 4.0);
      ASSERT_DOUBLE_EQ(result[1], 3.5);
      ASSERT_DOUBLE_EQ(result[2], 3.0);
      ASSERT_DOUBLE_EQ(result[3], 2.5);
      ASSERT_DOUBLE_EQ(result[4], 2.0);
   }

} // namespace KiLib