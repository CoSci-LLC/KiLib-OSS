/**
 * This file holds tests for the raster class
 * @author Owen T. Parkins
 * @date Jan 2025
 */

#include <KiLib/Rasters/Raster.hpp>
#include <cmath>
#include <gtest/gtest.h>
#include <tuple>



void SetBasicRasterProperties(KiLib::Rasters::Raster<double>& a) 
{
   a.set_xllcorner(0);
   a.set_yllcorner(0);
   a.set_cellsize(1);
   a.set_nodata_value(-9999);
   a.set_height(a.get_cellsize() * a.get_rows());
   a.set_width(a.get_cellsize() * a.get_cols());
}



void Print(KiLib::Rasters::Raster<double> a) {

   for ( size_t i = 0; i < a.get_rows(); i++ ) {
      for ( size_t j = 0; j < a.get_cols(); j++ ) {
         for ( size_t k = 0; k < a.get_zindex(); k++) {
            auto c = a.get(i,j,k );
            std::cerr << "[          ] ( " << i << ", " << j << ", " << k << ") = " <<( c.is_nodata ? -9999 : *(a.get(i,j, k).data)) << std::endl;
         }
      }
   }
}



TEST(Rasters, CopyOperator) {
  KiLib::Rasters::Raster<double> a(2, 2);
   SetBasicRasterProperties(a);
   a.set((size_t)0, 0, 1);
   a.set((size_t)0, 1, 1);
   a.set((size_t)1, 0, 1);
   a.set((size_t)1, 1, 1);

   auto b = a;

   EXPECT_EQ(a, b);
}


TEST(Rasters, atan) {
   KiLib::Rasters::Raster<double> a(1,1);
   a.set((size_t)0,0, 0, -5);

   auto b = std::atan(a);

   EXPECT_EQ(*(b.get((size_t)0,0,0).data), std::atan(-5.0));
//   std::cerr << "std::atan(-5.0) == " << std::atan(-5.0) << std::endl;
}


// Demonstrate some basic assertions.
TEST(Rasters, Basic_Operations) {

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

   // Ensure equality works
   EXPECT_EQ(a, a);
   EXPECT_NE(a, b);

   EXPECT_EQ(a * b, b);
   EXPECT_EQ(b * a, b);

   // Test a single multiplication
   EXPECT_EQ(b * 1, b);
   EXPECT_EQ(1 * b, b);


   EXPECT_EQ(a * 5, b);
   EXPECT_EQ(5 * a, b);

   EXPECT_EQ(b / a, b);
   EXPECT_NE(a / b, b); // Make sure the ordering matters
   
   EXPECT_EQ(1 / a, a);

   KiLib::Rasters::Raster<double> b_div_result( {2,2, 1}, 0.2);
   SetBasicRasterProperties(b_div_result);
   EXPECT_EQ(1 / b, b_div_result);


   // Testing Addition
   KiLib::Rasters::Raster<double> c(2, 2);
   SetBasicRasterProperties(c);
   c.set((size_t)0, 0, 6);
   c.set((size_t)0, 1, 6);
   c.set((size_t)1, 0, 6);
   c.set((size_t)1, 1, 6);

   EXPECT_EQ(a + b, c);
   EXPECT_EQ(b + a, c); // Make sure the order doesn't matter

   //
   // Testing Subtraction
   KiLib::Rasters::Raster<double> d(2, 2);
   SetBasicRasterProperties(d);
   d.set((size_t)0, 0, 4);
   d.set((size_t)0, 1, 4);
   d.set((size_t)1, 0, 4);
   d.set((size_t)1, 1, 4);

   EXPECT_EQ(b - 1, d);
   EXPECT_EQ(b - a, d);
   EXPECT_NE(a - b, d); // MAke sure the order does matter
   
   // Make sure that nodata cells are staying no data
   a.set((size_t)0, 1, a.get_nodata_value());
   c.set((size_t)0, 1, c.get_nodata_value());
 
   EXPECT_EQ(a + b, c);
   EXPECT_EQ(b + a, c); // Make sure the order doesn't matter


}

TEST(Rasters, Different_Sized_Rasters_Operatins) {


   KiLib::Rasters::Raster<double> a(2, 2);
   SetBasicRasterProperties(a);
   a.set((size_t)0, 0, 2);
   a.set((size_t)0, 1, 2);
   a.set((size_t)1, 0, 2);
   a.set((size_t)1, 1, 2);

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


   KiLib::Rasters::Raster<double> c(4, 4);
   SetBasicRasterProperties(c);
   c.set((size_t)0, 0, 10);
   c.set((size_t)0, 1, 10);
   c.set((size_t)0, 2, c.get_nodata_value());
   c.set((size_t)0, 3, c.get_nodata_value());
   c.set((size_t)1, 0, 10);
   c.set((size_t)1, 1, 10);
   c.set((size_t)1, 2, c.get_nodata_value());
   c.set((size_t)1, 3, c.get_nodata_value());
   c.set((size_t)2, 0, c.get_nodata_value());
   c.set((size_t)2, 1, c.get_nodata_value());
   c.set((size_t)2, 2, c.get_nodata_value());
   c.set((size_t)2, 3, c.get_nodata_value());
   c.set((size_t)3, 0, c.get_nodata_value());
   c.set((size_t)3, 1, c.get_nodata_value());
   c.set((size_t)3, 2, c.get_nodata_value());
   c.set((size_t)3, 3, c.get_nodata_value());

   const auto d = a * b;
   const auto e = b * a;


   EXPECT_EQ(e, c);
   EXPECT_EQ(d, c);
   EXPECT_EQ(b * a, a * b);

}


// Demonstrate some basic assertions.
TEST(Rasters, Clamp) {

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



TEST(Rasters, Layers) {

   KiLib::Rasters::Raster<double> a( std::make_tuple(2,2,2));
   SetBasicRasterProperties(a);
   a.set((size_t)0, 0, 0, 1);
   a.set((size_t)0, 0, 1, 1);
   a.set((size_t)0, 1, 0, 1);
   a.set((size_t)0, 1, 1, 1);
   a.set((size_t)1, 0, 0, 1);
   a.set((size_t)1, 0, 1, 1);
   a.set((size_t)1, 1, 0, 1);
   a.set((size_t)1, 1, 1, 1);

   KiLib::Rasters::Raster<double> b(std::make_tuple(2,2,2));
   SetBasicRasterProperties(b);
   b.set((size_t)0, 0, 0, 5);
   b.set((size_t)0, 0, 1, 5);
   b.set((size_t)0, 1, 0, 5);
   b.set((size_t)0, 1, 1, 5);
   b.set((size_t)1, 0, 0, 5);
   b.set((size_t)1, 0, 1, 5);
   b.set((size_t)1, 1, 0, 5);
   b.set((size_t)1, 1, 1, 5);
   // Ensure equality works
   EXPECT_EQ(a, a);
   EXPECT_NE(a, b);

   EXPECT_EQ(a * 5, b);
   EXPECT_EQ(5 * a, b);

   EXPECT_EQ(b / 5, a);
}


TEST(Rasters, Layers_Different_Sized_Rasters_Operations) {


   KiLib::Rasters::Raster<double> a( std::make_tuple (2, 2, 2));
   SetBasicRasterProperties(a);
   a.set((size_t)0, 0, 0, 2);
   a.set((size_t)0, 0, 1, 2);
   a.set((size_t)0, 1, 0, 2);
   a.set((size_t)0, 1, 1, 2);
   a.set((size_t)1, 0, 0, 2);
   a.set((size_t)1, 0, 1, 2);
   a.set((size_t)1, 1, 0, 2);
   a.set((size_t)1, 1, 1, 2);

   KiLib::Rasters::Raster<double> b( std::make_tuple(4, 4, 2));
   SetBasicRasterProperties(b);
   b.set((size_t)0, 0, 0,5);
   b.set((size_t)0, 1, 0,5);
   b.set((size_t)0, 2, 0,5);
   b.set((size_t)0, 3, 0,5);
   b.set((size_t)1, 0, 0,5);
   b.set((size_t)1, 1, 0,5);
   b.set((size_t)1, 2, 0,5);
   b.set((size_t)1, 3, 0,5);
   b.set((size_t)2, 0, 0,5);
   b.set((size_t)2, 1, 0,5);
   b.set((size_t)2, 2, 0,5);
   b.set((size_t)2, 3, 0,5);
   b.set((size_t)3, 0, 0,5);
   b.set((size_t)3, 1, 0,5);
   b.set((size_t)3, 2, 0,5);
   b.set((size_t)3, 3, 0,5);
   b.set((size_t)0, 0, 1,5);
   b.set((size_t)0, 1, 1,5);
   b.set((size_t)0, 2, 1,5);
   b.set((size_t)0, 3, 1,5);
   b.set((size_t)1, 0, 1,5);
   b.set((size_t)1, 1, 1,5);
   b.set((size_t)1, 2, 1,5);
   b.set((size_t)1, 3, 1,5);
   b.set((size_t)2, 0, 1,5);
   b.set((size_t)2, 1, 1,5);
   b.set((size_t)2, 2, 1,5);
   b.set((size_t)2, 3, 1,5);
   b.set((size_t)3, 0, 1,5);
   b.set((size_t)3, 1, 1,5);
   b.set((size_t)3, 2, 1,5);
   b.set((size_t)3, 3, 1,5);



   KiLib::Rasters::Raster<double> c( std::make_tuple(4,4,2));
   SetBasicRasterProperties(c);
   c.set((size_t)0, 0, 0, 10);
   c.set((size_t)0, 1, 0, 10);
   c.set((size_t)0, 2, 0, c.get_nodata_value());
   c.set((size_t)0, 3, 0, c.get_nodata_value());
   c.set((size_t)1, 0, 0, 10);
   c.set((size_t)1, 1, 0, 10);
   c.set((size_t)1, 2, 0, c.get_nodata_value());
   c.set((size_t)1, 3, 0, c.get_nodata_value());
   c.set((size_t)2, 0, 0, c.get_nodata_value());
   c.set((size_t)2, 1, 0, c.get_nodata_value());
   c.set((size_t)2, 2, 0, c.get_nodata_value());
   c.set((size_t)2, 3, 0, c.get_nodata_value());
   c.set((size_t)3, 0, 0, c.get_nodata_value());
   c.set((size_t)3, 1, 0, c.get_nodata_value());
   c.set((size_t)3, 2, 0, c.get_nodata_value());
   c.set((size_t)3, 3, 0, c.get_nodata_value());
   c.set((size_t)0, 0, 1, 10);
   c.set((size_t)0, 1, 1, 10);
   c.set((size_t)0, 2, 1, c.get_nodata_value());
   c.set((size_t)0, 3, 1, c.get_nodata_value());
   c.set((size_t)1, 0, 1, 10);
   c.set((size_t)1, 1, 1, 10);
   c.set((size_t)1, 2, 1, c.get_nodata_value());
   c.set((size_t)1, 3, 1, c.get_nodata_value());
   c.set((size_t)2, 0, 1, c.get_nodata_value());
   c.set((size_t)2, 1, 1, c.get_nodata_value());
   c.set((size_t)2, 2, 1, c.get_nodata_value());
   c.set((size_t)2, 3, 1, c.get_nodata_value());
   c.set((size_t)3, 0, 1, c.get_nodata_value());
   c.set((size_t)3, 1, 1, c.get_nodata_value());
   c.set((size_t)3, 2, 1, c.get_nodata_value());
   c.set((size_t)3, 3, 1, c.get_nodata_value());

   const auto d = a * b;
   const auto e = b * a;


   EXPECT_EQ(e, c);
   EXPECT_EQ(d, c);
   EXPECT_EQ(b * a, a * b);

}


TEST(Rasters, Invalid_Index_set) {

   KiLib::Rasters::Raster<double> c( std::make_tuple(4, 4, 1));
   SetBasicRasterProperties(c);
   EXPECT_ANY_THROW(c.set((size_t)3, 2, 1, c.get_nodata_value()));

}


TEST(Rasters, Min_Max) {

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



   KiLib::Rasters::Raster<double> b(2, 2);
   SetBasicRasterProperties(b);
   b.set((size_t)0, 0, 44);
   b.set((size_t)0, 1, 92);
   // Missing one on purpose
   b.set((size_t)1, 1, 48);




   KiLib::Rasters::Raster<double> max(2, 2);
   SetBasicRasterProperties(max);
   max.set((size_t)0, 0, 44);
   max.set((size_t)0, 1, 93);
   // Missing one on purpose
   max.set((size_t)1, 1, 48);

   EXPECT_EQ(std::max(a,b), max);




   KiLib::Rasters::Raster<double> min(2, 2);
   SetBasicRasterProperties(min);
   min.set((size_t)0, 0, 44);
   min.set((size_t)0, 1, 92);
   // Missing one on purpose
   min.set((size_t)1, 1, 42);

   EXPECT_EQ(std::min(a,b), min);
   Print(std::min(a,b));

   // Test with INF in the data
//   a.set((size_t)1, 0, INFINITY);
//   EXPECT_EQ(std::min(a),42); 
//   EXPECT_EQ(std::max(a),93); 
}


TEST(Rasters, Init_Values) {

   KiLib::Rasters::Raster<double> a({2, 2,1}, 5);
   SetBasicRasterProperties(a);
 
   KiLib::Rasters::Raster<double> b(2, 2);
   SetBasicRasterProperties(b);
   b.set((size_t)0, 0, 5);
   b.set((size_t)0, 1, 5);
   b.set((size_t)1, 0, 5);
   b.set((size_t)1, 1, 5);


   EXPECT_EQ(a, b);
}

TEST(Rasters, Init_Values_Z) {

   KiLib::Rasters::Raster<double> a( std::make_tuple (2, 2, 2));
   SetBasicRasterProperties(a);
   a.set((size_t)0, 0, 0, 5);
   a.set((size_t)0, 0, 1, 5);
   a.set((size_t)0, 1, 0, 5);
   a.set((size_t)0, 1, 1, 5);
   a.set((size_t)1, 0, 0, 5);
   a.set((size_t)1, 0, 1, 5);
   a.set((size_t)1, 1, 0, 5);
   a.set((size_t)1, 1, 1, 5);

    KiLib::Rasters::Raster<double> b( std::make_tuple(2, 2, 2), 5);
   SetBasicRasterProperties(b);
 
   EXPECT_EQ(a, b);

}


//Add sin/cos/etcc

TEST(Rasters, erfc) {
   
   std::valarray<double> a = {0, 0, 0};

   auto b = std::erfc(a);

   for (auto n : b)
      EXPECT_EQ(1, n);
}


TEST(Rasters, ierfc) {

   KiLib::Rasters::Raster<double> a( {2, 2, 1}, 0);
   SetBasicRasterProperties(a);

   KiLib::Rasters::Raster<double> b( {2, 2, 1}, 0.56418958354775628);
   SetBasicRasterProperties(b);

   auto c = std::ierfc(a);

   EXPECT_EQ(b, c);

}
