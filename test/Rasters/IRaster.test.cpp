/**
 * This file holds tests for the raster class
 * @author Owen T. Parkins
 * @date Jan 2025
 */

#include <KiLib/Rasters/IRaster.hpp>
#include <KiLib/Rasters/SparseRaster.hpp>
#include <KiLib/Rasters/Raster.hpp>
#include <cmath>
#include <gtest/gtest.h>
#include <tuple>


void SetBasicRasterProperties(KiLib::Rasters::Raster<double>&); 

// Demonstrate some basic assertions.
TEST(IRaster, Raster_Basic_Operations) {

   KiLib::Rasters::Raster<double> real_a(2, 2);
   SetBasicRasterProperties(real_a);
   real_a.set((size_t)0, 0, 1);
   real_a.set((size_t)0, 1, 1);
   real_a.set((size_t)1, 0, 1);
   real_a.set((size_t)1, 1, 1);

   KiLib::Rasters::Raster<double> real_b(2, 2);
   SetBasicRasterProperties(real_b);
   real_b.set((size_t)0, 0, 5);
   real_b.set((size_t)0, 1, 5);
   real_b.set((size_t)1, 0, 5);
   real_b.set((size_t)1, 1, 5);


   // Change to the Raster Interface
   KiLib::Rasters::IRaster<double>& a = real_a;
   KiLib::Rasters::IRaster<double>& b = real_b;

   // Ensure equality works
   EXPECT_TRUE(a == a);
   EXPECT_TRUE(a != b);

   EXPECT_TRUE( (a * b) == b);
   EXPECT_TRUE((b * a) == b);

   // Test a single multiplication
   EXPECT_TRUE( (b * 1) == b);
//   EXPECT_EQ(1 * b, b);


   EXPECT_TRUE( (a * 5) == b);
//   EXPECT_EQ(5 * a, b);

//   EXPECT_EQ(b / a, b);
//   EXPECT_NE(a / b, b); // Make sure the ordering matters
   
//   EXPECT_EQ(1 / a, a);

// KiLib::Rasters::Raster<double> b_div_result(2,2, 0.2);
//   SetBasicRasterProperties(b_div_result);
//   EXPECT_EQ(1 / b, b_div_result);


//   // Testing Addition
//   KiLib::Rasters::Raster<double> c(2, 2);
//   SetBasicRasterProperties(c);
//   c.set((size_t)0, 0, 6);
//   c.set((size_t)0, 1, 6);
//   c.set((size_t)1, 0, 6);
//   c.set((size_t)1, 1, 6);

//   EXPECT_EQ(a + b, c);
//   EXPECT_EQ(b + a, c); // Make sure the order doesn't matter

   //
   // Testing Subtraction
//   KiLib::Rasters::Raster<double> d(2, 2);
//   SetBasicRasterProperties(d);
//   d.set((size_t)0, 0, 4);
//   d.set((size_t)0, 1, 4);
//   d.set((size_t)1, 0, 4);
//   d.set((size_t)1, 1, 4);

//   EXPECT_EQ(b - a, d);
//   EXPECT_NE(a - b, d); // MAke sure the order does matter
   
   // Make sure that nodata cells are staying no data
//   a.set((size_t)0, 1, a.get_nodata_value());
//   c.set((size_t)0, 1, c.get_nodata_value());
 
//   EXPECT_EQ(a + b, c);
//   EXPECT_EQ(b + a, c); // Make sure the order doesn't matter


}

/*
// Demonstrate some basic assertions.
TEST(IRaster, Clamp) {

   KiLib::Rasters::Raster<double> a(2, 2);
   SetBasicRasterProperties(a);
   a.set((size_t)0, 0, 1);
   a.set((size_t)0, 1, 1);
   a.set((size_t)1, 0, 1);
   a.set((size_t)1, 1, 1);

   KiLib::Rasters::Raster<double> b(2, 2);
   SetBasicRasterProperties(b);
   b.set((size_t)0, 0, 5);
   b.set((size_t)0, 1, 5);
   b.set((size_t)1, 0, 5);
   b.set((size_t)1, 1, 5);

   auto c = std::clamp(b, 0.0, 1.0);

   // Ensure equality works
   EXPECT_EQ(a, c);
}


TEST(IRaster, Min_Max) {

   KiLib::Rasters::Raster<double> a(2, 2);
   SetBasicRasterProperties(a);
   a.set((size_t)0, 0, 44);
   a.set((size_t)0, 1, 93);
   // Missing one on purpose
   a.set((size_t)1, 1, 42);

   EXPECT_EQ(std::min(a),42); 
   EXPECT_EQ(std::max(a),93); 

   // Test with NAN in the data
   EXPECT_EQ(std::min(a),42); 
   EXPECT_EQ(std::max(a),93); 


   // Test with INF in the data
//   a.set((size_t)1, 0, INFINITY);
//   EXPECT_EQ(std::min(a),42); 
//   EXPECT_EQ(std::max(a),93); 
}
*/
