/**
 * This file holds tests for the raster class
 * @author Owen T. Parkins
 * @date Jan 2025
 */

#include <KiLib/Rasters/Raster.hpp>
#include <cmath>
#include <gtest/gtest.h>
#include <ostream>
#include <tuple>

class Rasters : public testing::TestWithParam<KiLib::Rasters::TYPE> {};


namespace KiLib::Rasters {
      template<typename T>
      void PrintTo(const  Raster<T>& raster, std::ostream* os) { 
         *os << raster.to_string();

         for ( size_t i = 0; i < raster.get_rows(); i++ ) {
            for ( size_t j = 0; j < raster.get_cols(); j++ ) {
               for ( size_t k = 0; k < raster.get_zindex(); k++) {
                  auto c = raster.get(i,j,k );
                  *os << "[          ] ( " << i << ", " << j << ", " << k << ") = " <<( c.is_nodata ? -9999 : *(c.data)) << std::endl;
               }
            }
         }

      }
} //namespace




INSTANTIATE_TEST_SUITE_P(
   KiLib, Rasters,
      testing::Values(KiLib::Rasters::TYPE::SPARSE, KiLib::Rasters::TYPE::DENSE),
   [](const testing::TestParamInfo<KiLib::Rasters::TYPE>& info) {
      switch(info.param) {
         case KiLib::Rasters::TYPE::SPARSE:
            return "Sparse";
         case KiLib::Rasters::TYPE::DENSE:
            return "Dense";
         default:
            return "Unknown";
      }
   });


void SetBasicRasterProperties(KiLib::Rasters::Raster<double>& a) 
{
   a.set_xllcorner(0);
   a.set_yllcorner(0);
   a.set_cellsize(1);
   a.set_nodata_value(-9999);
   a.set_height(a.get_cellsize() * a.get_rows());
   a.set_width(a.get_cellsize() * a.get_cols());
}



void Print(const KiLib::Rasters::Raster<double>& a) {

   for ( size_t i = 0; i < a.get_rows(); i++ ) {
      for ( size_t j = 0; j < a.get_cols(); j++ ) {
         for ( size_t k = 0; k < a.get_zindex(); k++) {
            auto c = a.get(i,j,k );
            std::cerr << "[          ] ( " << i << ", " << j << ", " << k << ") = " <<( c.is_nodata ? -9999 : *(a.get(i,j, k).data)) << std::endl;
         }
      }
   }
}



TEST_P(Rasters, CopyOperator) {
   KiLib::Rasters::Raster<double> a( { 2, 2, 1},
      {
         {{0, 0, 0}, 1},
         {{0, 1, 0}, 1},
         {{1, 0, 0}, 1},
         {{1, 1, 0}, 1},
      }, GetParam());
   SetBasicRasterProperties(a);

   auto b = a;

   EXPECT_EQ(a, b);
}


TEST_P(Rasters, atan) {
   KiLib::Rasters::Raster<double> a( { 2, 2, 2},
    {
         {{0, 0, 0}, -5},
         {{0, 1, 0}, -5},
         {{1, 0, 0}, -5},
         {{1, 1, 0}, -5},
         {{0, 0, 1}, -5},
         {{0, 1, 1}, -5},
         {{1, 0, 1}, -5},
         {{1, 1, 1}, -5},
      }, GetParam());

   auto b = std::atan(a);

   for ( auto it = b.begin(); it != b.end(); ++it) {
      EXPECT_EQ(*((&it).data), std::atan(-5.0));
   }
}

TEST_P(Rasters, tan) {
   KiLib::Rasters::Raster<double> a( { 2, 2, 2},
    {
         {{0, 0, 0}, -5},
         {{0, 1, 0}, -5},
         {{1, 0, 0}, -5},
         {{1, 1, 0}, -5},
         {{0, 0, 1}, -5},
         {{0, 1, 1}, -5},
         {{1, 0, 1}, -5},
         {{1, 1, 1}, -5},
      }, GetParam());

   auto b = std::tan(a);

   for ( auto it = b.begin(); it != b.end(); ++it) {
      EXPECT_EQ(*((&it).data), std::tan(-5.0));
   }
}

TEST_P(Rasters, cos) {
   KiLib::Rasters::Raster<double> a( { 2, 2, 2},
    {
         {{0, 0, 0}, -5},
         {{0, 1, 0}, -5},
         {{1, 0, 0}, -5},
         {{1, 1, 0}, -5},
         {{0, 0, 1}, -5},
         {{0, 1, 1}, -5},
         {{1, 0, 1}, -5},
         {{1, 1, 1}, -5},
      }, GetParam());

   auto b = std::cos(a);

   for ( auto it = b.begin(); it != b.end(); ++it) {
      EXPECT_EQ(*((&it).data), std::cos(-5.0));
   }
}

TEST_P(Rasters, sin) {
   KiLib::Rasters::Raster<double> a( { 2, 2, 2},
    {
         {{0, 0, 0}, -5},
         {{0, 1, 0}, -5},
         {{1, 0, 0}, -5},
         {{1, 1, 0}, -5},
         {{0, 0, 1}, -5},
         {{0, 1, 1}, -5},
         {{1, 0, 1}, -5},
         {{1, 1, 1}, -5},
      }, GetParam());

   auto b = std::sin(a);

   for ( auto it = b.begin(); it != b.end(); ++it) {
      EXPECT_EQ(*((&it).data), std::sin(-5.0));
   }
}

TEST_P(Rasters, exp) {
   KiLib::Rasters::Raster<double> a( { 2, 2, 2},
    {
         {{0, 0, 0}, -5},
         {{0, 1, 0}, -5},
         {{1, 0, 0}, -5},
         {{1, 1, 0}, -5},
         {{0, 0, 1}, -5},
         {{0, 1, 1}, -5},
         {{1, 0, 1}, -5},
         {{1, 1, 1}, -5},
      }, GetParam());

   auto b = std::exp(a);

   for ( auto it = b.begin(); it != b.end(); ++it) {
      EXPECT_EQ(*((&it).data), std::exp(-5.0));
   }
}

// Demonstrate some basic assertions.
TEST_P(Rasters, Basic_Operations) {

   KiLib::Rasters::Raster<double> a( { 2, 2, 2},
    {
         {{0, 0, 0}, 1},
         {{0, 1, 0}, 1},
         {{1, 0, 0}, 1},
         {{1, 1, 0}, 1},
         {{0, 0, 1}, 1},
         {{0, 1, 1}, 1},
         {{1, 0, 1}, 1},
         {{1, 1, 1}, 1},
      }, GetParam());
   SetBasicRasterProperties(a);

   KiLib::Rasters::Raster<double> b( { 2, 2, 2},
    {
         {{0, 0, 0}, 5},
         {{0, 1, 0}, 5},
         {{1, 0, 0}, 5},
         {{1, 1, 0}, 5},
         {{0, 0, 1}, 5},
         {{0, 1, 1}, 5},
         {{1, 0, 1}, 5},
         {{1, 1, 1}, 5},
      }, GetParam());
   SetBasicRasterProperties(b);

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

   KiLib::Rasters::Raster<double> b_div_result( {2,2,2}, 0.2);
   SetBasicRasterProperties(b_div_result);
   b_div_result.set_name("b_div_result");
   b_div_result.convert_type_to(GetParam());
   EXPECT_EQ(1 / b, b_div_result);

   // Testing Addition
   KiLib::Rasters::Raster<double> c( { 2, 2, 2},
    {
         {{0, 0, 0}, 6},
         {{0, 1, 0}, 6},
         {{1, 0, 0}, 6},
         {{1, 1, 0}, 6},
         {{0, 0, 1}, 6},
         {{0, 1, 1}, 6},
         {{1, 0, 1}, 6},
         {{1, 1, 1}, 6},
      }, GetParam());
   SetBasicRasterProperties(c);

   EXPECT_EQ(a + b, c);
   EXPECT_EQ(b + a, c); // Make sure the order doesn't matter

   //
   // Testing Subtraction
   KiLib::Rasters::Raster<double> d( { 2, 2, 2},
    {
         {{0, 0, 0}, 4},
         {{0, 1, 0}, 4},
         {{1, 0, 0}, 4},
         {{1, 1, 0}, 4},
         {{0, 0, 1}, 4},
         {{0, 1, 1}, 4},
         {{1, 0, 1}, 4},
         {{1, 1, 1}, 4},
      }, GetParam());
   SetBasicRasterProperties(d);



   EXPECT_EQ(b - 1, d);
   EXPECT_EQ(b - a, d);
   EXPECT_NE(a - b, d); // MAke sure the order does matter
   
   // Make sure that nodata cells are staying no data
   a.set((size_t)0, 1, a.get_nodata_value());

   auto b_r = b; // copying b and making it a result raster for the tests.
   b_r.set((size_t)0, 1, c.get_nodata_value());

   c.set((size_t)0, 1, c.get_nodata_value());
   d.set((size_t)0, 1, c.get_nodata_value());
 
   // This makes sure the nodata values for each operation are respected
   EXPECT_EQ(a + b, c);
   EXPECT_EQ(b + a, c); // Make sure the order doesn't matter
   EXPECT_EQ(b - a, d);
   EXPECT_NE(a - b, d); // MAke sure the order does matter
   EXPECT_EQ(a * b, b_r);
   EXPECT_EQ(b * a, b_r);
   EXPECT_EQ(a * 5, b_r);
   EXPECT_EQ(5 * a, b_r);
   EXPECT_EQ(b / a, b_r);
   EXPECT_NE(a / b, b_r); // Make sure the ordering matters
   EXPECT_EQ(1 / a, a);



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


TEST_P(Rasters, Invalid_Index_set) {

   KiLib::Rasters::Raster<double> c( std::make_tuple(4, 4, 1), {}, GetParam());
   SetBasicRasterProperties(c);
   EXPECT_ANY_THROW(c.set((size_t)3, 2, 1, c.get_nodata_value()));

}


TEST_P(Rasters, Element_Min_Max) {

   KiLib::Rasters::Raster<double> a( {2, 2, 2},
      {
         { {0,0,1}, 44},
         { {0,1,1}, 93}, //This is sparse on purpose
         { {1,1,0}, 42},
      }
   , GetParam());

   SetBasicRasterProperties(a);
   EXPECT_EQ(std::min(a),42); 
   EXPECT_EQ(std::max(a),93); 

   // Test with NAN in the data
   EXPECT_EQ(std::min(a),42); 
   EXPECT_EQ(std::max(a),93); 

}

TEST_P(Rasters, Raster_Min_Max) {

   KiLib::Rasters::Raster<double> a( {2, 2, 2},
      {
         { {0,0,1}, 44},
         { {0,1,1}, 93}, //This is sparse on purpose
         { {1,1,0}, 42},
      }
   , GetParam());
   SetBasicRasterProperties(a);


   KiLib::Rasters::Raster<double> b( {2, 2, 2},
      {
         { {0,0,1}, 44},
         { {0,1,1}, 92}, //This is sparse on purpose
         { {1,1,0}, 48},
      }
   , GetParam());
   SetBasicRasterProperties(b);

   KiLib::Rasters::Raster<double> max( {2, 2, 2},
      {
         { {0,0,1}, 44},
         { {0,1,1}, 93}, //This is sparse on purpose
         { {1,1,0}, 48},
      }
   , GetParam());
   SetBasicRasterProperties(max);

   EXPECT_EQ(std::max(a,b), max);



   KiLib::Rasters::Raster<double> min( {2, 2, 2},
      {
         { {0,0,1}, 44},
         { {0,1,1}, 92}, //This is sparse on purpose
         { {1,1,0}, 42},
      }
   , GetParam());
   SetBasicRasterProperties(min);

   EXPECT_EQ(std::min(a,b), min);
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

TEST_P(Rasters, Init_3d_from_2d) {

   KiLib::Rasters::Raster<double> a( {2, 2, 1},
      {
         { {0,0,0}, 44},
         { {0,1,0}, 92}, //This is sparse on purpose
         { {1,1,0}, 42},
      }
   , GetParam());
   SetBasicRasterProperties(a);

   const size_t Z = 6;
   KiLib::Rasters::Raster<double> b( a, {0,0,Z});
   SetBasicRasterProperties(b);

   EXPECT_EQ(b.get_zindex(), Z);
   EXPECT_EQ( *(b.get((size_t)0,0,0).data), *(a.get((size_t)0,0,0).data));
   EXPECT_EQ( *(b.get((size_t)0,1,0).data), *(a.get((size_t)0,1,0).data));
   EXPECT_EQ( *(b.get((size_t)1,1,0).data), *(a.get((size_t)1,1,0).data));

   // Allow us to set a value in the new zindex
   b.set((size_t)0, 0, 3, 4);

}

TEST_P(Rasters, Init_3d_from_2d_with_value) {

   KiLib::Rasters::Raster<double> a( {2, 2, 1},
      {
         { {0,0,0}, 44},
         { {0,1,0}, 92}, //This is sparse on purpose
         { {1,1,0}, 42},
      }
   , GetParam());
   SetBasicRasterProperties(a);

   const size_t Z = 6;
   KiLib::Rasters::Raster<double> b( a, {0,0,Z}, 8);
   SetBasicRasterProperties(b);

   EXPECT_EQ(b.get_zindex(), Z);
   EXPECT_EQ( *(b.get((size_t)0,0,0).data), 8);
   EXPECT_EQ( *(b.get((size_t)0,1,0).data), 8);
   EXPECT_EQ( *(b.get((size_t)1,1,0).data), 8);

   b.set((size_t)0, 0, 3, 4);
}


TEST_P(Rasters, valid_cell_count) {

   KiLib::Rasters::Raster<double> a( {2, 2, 1},
      {
         { {0,0,0}, 44},
         { {0,1,0}, 92}, //This is sparse on purpose
         { {1,1,0}, 42},
      }
   , GetParam());
   SetBasicRasterProperties(a);

   EXPECT_EQ(a.get_valid_cell_count(), 3);
}

//Add sin/cos/etcc

TEST(Rasters, erfc) {
   
   std::valarray<double> a = {0, 0, 0};

   auto b = std::erfc(a);

   for (auto n : b)
      EXPECT_EQ(1, n);
}


TEST_P(Rasters, ierfc) {

   KiLib::Rasters::Raster<double> in( {2, 2, 2}, 0);
   in.convert_type_to(GetParam());
   SetBasicRasterProperties(in);

   KiLib::Rasters::Raster<double> expected( {2, 2, 2}, 0.56418958354775628);
   expected.convert_type_to(GetParam());
   SetBasicRasterProperties(expected);

   auto c = std::ierfc(in);

   EXPECT_EQ(c, expected);

}
