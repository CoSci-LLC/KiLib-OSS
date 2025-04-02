/**
 * This file holds tests for the raster class
 * @author Owen T. Parkins
 * @date Jan 2025
 */

#include <KiLib/Rasters/SparseRaster.hpp>
#include <KiLib/Rasters/Raster.hpp>
#include <cmath>
#include <gtest/gtest.h>

void SetBasicRasterProperties(KiLib::Rasters::Raster<double>&);
void Print(KiLib::Rasters::Raster<double>) ;

void SetBasicRasterProperties(KiLib::Rasters::SparseRaster<double>& a) 
{
   a.set_xllcorner(0);
   a.set_yllcorner(0);
   a.set_cellsize(1);
   a.set_nodata_value(-9999);
   a.set_height(a.get_cellsize() * a.get_rows());
   a.set_width(a.get_cellsize() * a.get_cols());
}



// Demonstrate some basic assertions.
TEST(SparseRasters, Basic_Operations) {

   KiLib::Rasters::SparseRaster<double> sparse_a({2, 2, 1},
   {
      {{0, 0, 0}, 1},
      {{0, 1, 0}, 1},
      {{1, 0, 0}, 1},
      {{1, 1, 0}, 1}
   });
   SetBasicRasterProperties(sparse_a);

   KiLib::Rasters::SparseRaster<double> sparse_b({2, 2, 1},
   {
      {{0, 0, 0}, 5},
      {{0, 1, 0}, 5},
      {{1, 0, 0}, 5},
      {{1, 1, 0}, 5}
   });
   SetBasicRasterProperties(sparse_b);

   KiLib::Rasters::Raster<double> a (sparse_a);
   KiLib::Rasters::Raster<double> b (sparse_b);

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

   // Testing Addition
   KiLib::Rasters::SparseRaster<double> sparse_c({2, 2, 1},
   {
      {{0, 0, 0}, 6},
      {{0, 1, 0}, 6},
      {{1, 0, 0}, 6},
      {{1, 1, 0}, 6}
   });
   SetBasicRasterProperties(sparse_c);
   KiLib::Rasters::Raster<double> c (sparse_c);

   EXPECT_EQ(a + b, c);
   EXPECT_EQ(b + a, c); // Make sure the order doesn't matter

   //
   // Testing Subtraction
   KiLib::Rasters::SparseRaster<double> sparse_d({2, 2, 1},
   {
      {{0, 0, 0}, 4},
      {{0, 1, 0}, 4},
      {{1, 0, 0}, 4},
      {{1, 1, 0}, 4}
   });
   SetBasicRasterProperties(sparse_d);
   KiLib::Rasters::Raster<double> d (sparse_d);

   EXPECT_EQ(b - a, d);
   EXPECT_NE(a - b, d); // MAke sure the order does matter
}



TEST(SparseRasters, Different_Sized_Rasters_Operatins) {


   KiLib::Rasters::SparseRaster<double> sparse_a({2, 2, 1},
   {
      {{0, 0, 0}, 2},
      {{0, 1, 0}, 2},
      {{1, 0, 0}, 2},
      {{1, 1, 0}, 2}
   });
   SetBasicRasterProperties(sparse_a);
   KiLib::Rasters::Raster<double> a (sparse_a);

   KiLib::Rasters::SparseRaster<double> sparse_b({4, 4, 1},
   {
      {{0, 0, 0}, 5},
      {{0, 1, 0}, 5},
      {{0, 2, 0}, 5},
      {{0, 3, 0}, 5},
      {{1, 0, 0}, 5},
      {{1, 1, 0}, 5},
      {{1, 2, 0}, 5},
      {{1, 3, 0}, 5},
      {{2, 0, 0}, 5},
      {{2, 1, 0}, 5},
      {{2, 2, 0}, 5},
      {{2, 3, 0}, 5},
      {{3, 0, 0}, 5},
      {{3, 1, 0}, 5},
      {{3, 2, 0}, 5},
      {{3, 3, 0}, 5},
   });
   SetBasicRasterProperties(sparse_b);
   KiLib::Rasters::Raster<double> b (sparse_b);

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

   SetBasicRasterProperties(c);

   const auto d = a * b;
   const auto e = a * b;

   EXPECT_EQ(e, c);
   EXPECT_EQ(d, c);
   EXPECT_EQ(b * a, a * b);

}


// Demonstrate some basic assertions.
TEST(SparseRasters, Clamp) {

   KiLib::Rasters::SparseRaster<double> sparse_a({2, 2, 1}, 
   {
      {{0, 0, 0}, 1},
      {{0, 1, 0}, 1},
      {{1, 0, 0}, 1},
      {{1, 1, 0}, 1}
   });

   SetBasicRasterProperties(sparse_a);
   KiLib::Rasters::Raster<double> a (sparse_a);

   KiLib::Rasters::SparseRaster<double> sparse_b({2, 2, 1},
   {
      {{0, 0, 0}, 5},
      {{0, 1, 0}, 5},
      {{1, 0, 0}, 5},
      {{1, 1, 0}, 5}
   });


   SetBasicRasterProperties(sparse_b);
   KiLib::Rasters::Raster<double> b (sparse_b);

   auto c = std::clamp(b, 0.0, 1.0);

   // Ensure equality works
   EXPECT_EQ(a, c);
}



TEST(SparseRasters, Layers) {

   KiLib::Rasters::SparseRaster<double> sparse_a({2, 2, 2},
   {
      {{0, 0, 0}, 1},
      {{0, 0, 1}, 1},
      {{0, 1, 0}, 1},
      {{0, 1, 1}, 1},
      {{1, 0, 0}, 1},
      {{1, 0, 1}, 1},
      {{1, 1, 0}, 1},
      {{1, 1, 1}, 1},
   });
   SetBasicRasterProperties(sparse_a);
   KiLib::Rasters::Raster<double> a (sparse_a);

   KiLib::Rasters::SparseRaster<double> sparse_b({2, 2, 2},
   {
      {{0, 0, 0}, 5},
      {{0, 0, 1}, 5},
      {{0, 1, 0}, 5},
      {{0, 1, 1}, 5},
      {{1, 0, 0}, 5},
      {{1, 0, 1}, 5},
      {{1, 1, 0}, 5},
      {{1, 1, 1}, 5},
   });

   SetBasicRasterProperties(sparse_b);
   KiLib::Rasters::Raster<double> b (sparse_b);
   //
   // Ensure equality works
   EXPECT_EQ(a, a);
   EXPECT_NE(a, b);

   EXPECT_EQ(a * 5, b);
   EXPECT_EQ(5 * a, b);

   EXPECT_EQ(b / 5, a);
}

TEST(SparseRasters, InitFromMap) {


   KiLib::Rasters::SparseRaster<double> sparse_a({2, 2, 2},
   {
      {{0, 0, 0}, 1},
      {{0, 0, 1}, 2},
      {{0, 1, 0}, 3},
      {{0, 1, 1}, 4},
      {{1, 0, 0}, 5},
      {{1, 0, 1}, 6},
      {{1, 1, 0}, 7},
      {{1, 1, 1}, 8},
   });
   KiLib::Rasters::Raster<double> a (sparse_a);

   EXPECT_EQ(*(a.get((size_t)0,0,0).data), 1);
   EXPECT_EQ(*(a.get((size_t)0,0,1).data), 2);
   EXPECT_EQ(*(a.get((size_t)0,1,0).data), 3);
   EXPECT_EQ(*(a.get((size_t)0,1,1).data), 4);
   EXPECT_EQ(*(a.get((size_t)1,0,0).data), 5);
   EXPECT_EQ(*(a.get((size_t)1,0,1).data), 6);
   EXPECT_EQ(*(a.get((size_t)1,1,0).data), 7);
   EXPECT_EQ(*(a.get((size_t)1,1,1).data), 8);
}


TEST(SparseRasters, Layers_Different_Sized_Rasters_Operations) {


   KiLib::Rasters::SparseRaster<double> sparse_a({2, 2, 2},
   {
      {{0, 0, 0}, 2},
      {{0, 0, 1}, 2},
      {{0, 1, 0}, 2},
      {{0, 1, 1}, 2},
      {{1, 0, 0}, 2},
      {{1, 0, 1}, 2},
      {{1, 1, 0}, 2},
      {{1, 1, 1}, 2},
   });


   SetBasicRasterProperties(sparse_a);
   KiLib::Rasters::Raster<double> a (sparse_a);

   KiLib::Rasters::SparseRaster<double> sparse_b({4, 4, 2},
   {
      {{0, 0, 0},5},
      {{0, 1, 0},5},
      {{0, 2, 0},5},
      {{0, 3, 0},5},
      {{1, 0, 0},5},
      {{1, 1, 0},5},
      {{1, 2, 0},5},
      {{1, 3, 0},5},
      {{2, 0, 0},5},
      {{2, 1, 0},5},
      {{2, 2, 0},5},
      {{2, 3, 0},5},
      {{3, 0, 0},5},
      {{3, 1, 0},5},
      {{3, 2, 0},5},
      {{3, 3, 0},5},
      {{0, 0, 1},5},
      {{0, 1, 1},5},
      {{0, 2, 1},5},
      {{0, 3, 1},5},
      {{1, 0, 1},5},
      {{1, 1, 1},5},
      {{1, 2, 1},5},
      {{1, 3, 1},5},
      {{2, 0, 1},5},
      {{2, 1, 1},5},
      {{2, 2, 1},5},
      {{2, 3, 1},5},
      {{3, 0, 1},5},
      {{3, 1, 1},5},
      {{3, 2, 1},5},
      {{3, 3, 1},5}
   });
   SetBasicRasterProperties(sparse_b);
   KiLib::Rasters::Raster<double> b (sparse_b);



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


   SetBasicRasterProperties(c);

   const auto d = a * b;
   const auto e = b * a;


   EXPECT_EQ(e, c);
   EXPECT_EQ(d, c);
   EXPECT_EQ(b * a, a * b);

}


TEST(SparseRasters, Invalid_Index_set) {

   KiLib::Rasters::SparseRaster<double> sparse_c({4, 4, 1}, {});
   SetBasicRasterProperties(sparse_c);
   KiLib::Rasters::Raster<double> c (sparse_c);
   EXPECT_ANY_THROW(c.set((size_t)3, 2, 1, c.get_nodata_value()));

}


TEST(SparseRasters, Min_Max) {

   KiLib::Rasters::SparseRaster<double> sparse_a({2, 2,1},
   {
      {{ 0, 0, 0}, 44},
      {{ 0, 1, 0}, 93},
      {{ 1, 1, 0}, 42}
   });
   SetBasicRasterProperties(sparse_a);
   KiLib::Rasters::Raster<double> a (sparse_a);

   EXPECT_EQ(std::min(a),42); 
   EXPECT_EQ(std::max(a),93); 


   // Test with INF in the data
//   a.set((size_t)1, 0, INFINITY);
//   EXPECT_EQ(std::min(a),42); 
//   EXPECT_EQ(std::max(a),93); 
}


TEST(SparseRasters, CopyFromRaster) {

   KiLib::Rasters::Raster<double> a(2, 2);
   SetBasicRasterProperties(a);
   a.set((size_t)0, 0, 1);
   a.set((size_t)0, 1, 1);
   a.set((size_t)1, 0, 1);
   a.set((size_t)1, 1, 1); 

   KiLib::Rasters::SparseRaster<double> b({2, 2, 1},
   {
      {{0, 0, 0}, 1},
      {{0, 1, 0}, 1},
      {{1, 0, 0}, 1},
      {{1, 1, 0}, 1}
   });
   SetBasicRasterProperties(b);


   KiLib::Rasters::SparseRaster<double> c(a, [](const KiLib::Rasters::Cell<double>& c) { return *(c.data); } );

   EXPECT_EQ(c, b);

}
