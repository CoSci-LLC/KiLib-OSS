/**
 * This file holds tests for the raster iterator class
 * @author Owen T. Parkins
 * @date Mar 2025
 */

#include <KiLib/Rasters/Raster.hpp>
#include <cmath>
#include <gtest/gtest.h>

void SetBasicRasterProperties(KiLib::Rasters::Raster<double>&); 


TEST(Rasters, TestIterator_Allvalues) {

   KiLib::Rasters::Raster<double> b(4, 4);
   SetBasicRasterProperties(b);
   b.set((size_t)0, 0, 5);
   b.set((size_t)0, 1, 5);
   b.set((size_t)0, 2, 5);
   b.set((size_t)0, 3, 5);
   b.set((size_t)1, 0, 5);
   b.set((size_t)1, 1, 5);
   b.set((size_t)1, 2, 5);
   b.set((size_t)1, 3, 5);
   b.set((size_t)2, 0, 5);
   b.set((size_t)2, 1, 5);
   b.set((size_t)2, 2, 5);
   b.set((size_t)2, 3, 5);
   b.set((size_t)3, 0, 5);
   b.set((size_t)3, 1, 5);
   b.set((size_t)3, 2, 5);
   b.set((size_t)3, 3, 5);

   size_t count = 0;
   for (auto it = b.begin(); it != b.end(); ++it) {
      EXPECT_EQ(*it, 5); 
      EXPECT_EQ( *((&it).data), 5); 
      count++;
   }

   EXPECT_EQ(count, b.get_ndata());


}


TEST(Rasters, TestIterator_NoDataValues) {

   KiLib::Rasters::Raster<double> b(4, 4);
   SetBasicRasterProperties(b);
   b.set((size_t)0, 0, 5);
   b.set((size_t)0, 1, 5);
   b.set((size_t)0, 2, 5);
   b.set((size_t)0, 3, 5);
   b.set((size_t)1, 0, 5);
   //b.set((size_t)1, 1, 5);
   b.set((size_t)1, 2, 5);
   b.set((size_t)1, 3, 5);
   b.set((size_t)2, 0, 5);
   b.set((size_t)2, 1, 5);
   b.set((size_t)2, 2, 5);
   b.set((size_t)2, 3, 5);
   b.set((size_t)3, 0, 5);
   //b.set((size_t)3, 1, 5);
   b.set((size_t)3, 2, 5);
   b.set((size_t)3, 3, 5);

   size_t count = 0;
   for (auto it = b.begin(); it != b.end(); ++it) {
      EXPECT_EQ(*it, 5); 
      count++;
   }

   EXPECT_EQ(count, 14);


}


TEST(Rasters, ind2sub) {

   KiLib::Rasters::Raster<double> b(4, 4);


   // Weak test. Should be developed more
   EXPECT_EQ(b.flatten_index(2,2,0), 10);

   auto r = b.ind2sub(10);
   EXPECT_EQ(std::get<0>(r), 2);
   EXPECT_EQ(std::get<1>(r), 2);
   EXPECT_EQ(std::get<2>(r), 0);



}
