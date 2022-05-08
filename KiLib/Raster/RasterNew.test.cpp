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


#include <KiLib/Raster/RasterNew.hpp>
#include <filesystem>
#include <gtest/gtest.h>
#include <spdlog/fmt/ostr.h>
#include <spdlog/spdlog.h>

#ifdef __APPLE__
namespace fs = std::__fs::filesystem;
#else
namespace fs = std::filesystem;
#endif

namespace KiLib
{
   TEST(RasterNew, RasterNew)
   {
      RasterNew rast = RasterNew();

      ASSERT_EQ(rast.nodata_value, RASTER_DEFAULT_NODATA_VALUE);

      ASSERT_EQ(rast.nRows, 0);
      ASSERT_EQ(rast.nCols, 0);
      ASSERT_EQ(rast.data.rows(), 0);
      ASSERT_EQ(rast.data.cols(), 0);
   }

   TEST(RasterNew, RasterNewSized)
   {
      RasterNew rast = RasterNew(10, 10);

      ASSERT_EQ(rast.nodata_value, RASTER_DEFAULT_NODATA_VALUE);

      ASSERT_EQ(rast.nRows, 10);
      ASSERT_EQ(rast.nCols, 10);
      ASSERT_EQ(rast.data.rows(), 10);
      ASSERT_EQ(rast.data.cols(), 10);
   }

   TEST(RasterNew, FillLike)
   {
      auto cwd  = fs::current_path();
      auto path = fs::path(std::string(TEST_DIRECTORY) + "/ComputeSlope/7x3_NODATA.dem");

      RasterNew dem(path.string());

      RasterNew filledNODATA = RasterNew::FillLike(dem, 1.0, true);
      RasterNew fullFilled   = RasterNew::FillLike(dem, 0.0, false);

      double sumNODATA = filledNODATA.data.sum();
      ASSERT_EQ(filledNODATA.nRows, dem.nRows);
      ASSERT_EQ(filledNODATA.nCols, dem.nCols);
      ASSERT_EQ(filledNODATA.xllcorner, dem.xllcorner);
      ASSERT_EQ(filledNODATA.yllcorner, dem.yllcorner);
      ASSERT_EQ(filledNODATA.cellsize, dem.cellsize);
      ASSERT_EQ(filledNODATA.nodata_value, dem.nodata_value);
      ASSERT_EQ(sumNODATA, -99979.0);

      double sumFull = fullFilled.data.sum();
      ASSERT_EQ(fullFilled.nRows, dem.nRows);
      ASSERT_EQ(fullFilled.nCols, dem.nCols);
      ASSERT_EQ(fullFilled.xllcorner, dem.xllcorner);
      ASSERT_EQ(fullFilled.yllcorner, dem.yllcorner);
      ASSERT_EQ(fullFilled.cellsize, dem.cellsize);
      ASSERT_EQ(fullFilled.nodata_value, dem.nodata_value);
      ASSERT_EQ(sumFull, 0);
   }

   TEST(RasterNew, Resize)
   {
      RasterNew rast = RasterNew();

      rast.Resize(10, 7);
      ASSERT_EQ(rast.nRows, 10);
      ASSERT_EQ(rast.nCols, 7);
      ASSERT_EQ(rast.data.rows(), rast.nRows);
      ASSERT_EQ(rast.data.cols(), rast.nCols);

      rast.Resize(2, 15);
      ASSERT_EQ(rast.nRows, 2);
      ASSERT_EQ(rast.nCols, 15);
      ASSERT_EQ(rast.data.rows(), rast.nRows);
      ASSERT_EQ(rast.data.cols(), rast.nCols);
   }

   TEST(RasterNew, at)
   {
      RasterNew rast = RasterNew(10, 10);
      rast.at(3, 3)  = 1.0;

      ASSERT_DOUBLE_EQ(rast.at(3, 3), 1.0);
      ASSERT_DOUBLE_EQ(rast.data(3, 3), 1.0);
   }

   TEST(RasterNew, IndexOperator)
   {
      RasterNew rast = RasterNew(10, 10);
      rast(3, 3)     = 1.0;

      ASSERT_DOUBLE_EQ(rast.at(3, 3), 1.0);
      ASSERT_DOUBLE_EQ(rast.data(3, 3), 1.0);
   }

   TEST(RasterNew, FlattenIndex)
   {
      RasterNew rast = RasterNew(10, 7);

      ASSERT_EQ(rast.FlattenIndex(0, 3), 3);
      ASSERT_EQ(rast.FlattenIndex(1, 3), 10);
      ASSERT_EQ(rast.FlattenIndex(3, 2), 23);
   }


   TEST(RasterNew, GetRowCol)
   {
      RasterNew rast = RasterNew(10, 7);

      ASSERT_EQ(rast.GetRowCol(3), std::make_pair(0L, 3L));
      ASSERT_EQ(rast.GetRowCol(10), std::make_pair(1L, 3L));
      ASSERT_EQ(rast.GetRowCol(23), std::make_pair(3L, 2L));
   }

   TEST(RasterNew, OOBAccess)
   {
      RasterNew rast = RasterNew(10, 7);

      ASSERT_ANY_THROW(rast.at(10, 7));
      ASSERT_ANY_THROW(rast.at(0, 7));
      ASSERT_ANY_THROW(rast.at(10, 0));

#ifdef NDEBUG
      ASSERT_NO_THROW(rast(10, 7));
      ASSERT_NO_THROW(rast(0, 7));
      ASSERT_NO_THROW(rast(10, 0));
#endif
   }

   TEST(RasterNew, GetMinValue)
   {
      auto      path = fs::path(std::string(TEST_DIRECTORY) + "/ComputeSlope/7x7.dem");
      RasterNew dem(path.string());

      ASSERT_DOUBLE_EQ(dem.GetMinValue(), 1906.551147);
   }

   TEST(RasterNew, GetMaxValue)
   {
      auto      path = fs::path(std::string(TEST_DIRECTORY) + "/ComputeSlope/7x7.dem");
      RasterNew dem(path.string());

      ASSERT_DOUBLE_EQ(dem.GetMaxValue(), 1912.436157);
   }

   TEST(RasterNew, GetMeanValue)
   {
      auto      path = fs::path(std::string(TEST_DIRECTORY) + "/ComputeSlope/7x7.dem");
      RasterNew dem(path.string());

      ASSERT_DOUBLE_EQ(dem.GetMeanValue(), 1909.7339340000005);
   }

   TEST(RasterNew, GetNDataPoints)
   {
      auto      path = fs::path(std::string(TEST_DIRECTORY) + "/ComputeSlope/7x7.dem");
      RasterNew dem(path.string());

      ASSERT_EQ(dem.GetNDataPoints(), 44);
   }

   TEST(RasterNew, GetNNoDataPoints)
   {
      auto      path = fs::path(std::string(TEST_DIRECTORY) + "/ComputeSlope/7x7.dem");
      RasterNew dem(path.string());

      ASSERT_EQ(dem.GetNNoDataPoints(), 5);
   }

   TEST(RasterNew, GetValidIndices)
   {
      auto      path = fs::path(std::string(TEST_DIRECTORY) + "/ComputeSlope/7x7.dem");
      RasterNew dem(path.string());

      std::vector<Index> expected{0,  1,  2,  3,  4,  5,  7,  8,  9,  11, 12, 13, 14, 15, 16,
                                  17, 18, 19, 20, 22, 23, 25, 26, 27, 28, 29, 30, 31, 32, 33,
                                  34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 47, 48};

      auto out = dem.GetValidIndices();

      ASSERT_EQ(expected, out);
   }

   TEST(RasterNew, RandPoint)
   {
      auto      path = fs::path(std::string(TEST_DIRECTORY) + "/ComputeSlope/7x7.dem");
      RasterNew dem(path.string());

      auto gen = std::mt19937_64(std::random_device{}());

      for (int i = 0; i < 10000; ++i)
      {
         Vec3 p = dem.RandPoint(gen);

         // Check if its in bounds
         ASSERT_GE(p.x, dem.xllcorner);
         ASSERT_GE(p.y, dem.yllcorner);
         ASSERT_LE(p.x, dem.xllcorner + dem.width);
         ASSERT_LE(p.y, dem.yllcorner + dem.height);
      }
   }

   TEST(RasterNew, GetInterpBilinear)
   {
      auto path = fs::path(std::string(TEST_DIRECTORY) + "/ComputeSlope/5x5.dem");

      RasterNew dem(path.string());

      Vec3 p1{780099.2947926898, 205426.59516664856, 1909.8001915739701};
      Vec3 p2{780099.3343145008, 205427.51547149016, 1910.3570077815384};
      Vec3 p3{780096.0843788842, 205427.65816391885, 1910.520074732899};

      double z1 = dem(p1);
      ASSERT_DOUBLE_EQ(z1, p1.z);

      double z2 = dem(p2);
      ASSERT_DOUBLE_EQ(z2, p2.z);

      double z3 = dem(p3);
      ASSERT_DOUBLE_EQ(z3, p3.z);
   }

   TEST(RasterNew, GetNearestCell)
   {
      auto      path = fs::path(std::string(TEST_DIRECTORY) + "/ComputeSlope/7x7.dem");
      RasterNew dem(path.string());

      Vec3 pos1{780096, 205422, 0};
      Vec3 pos2{780096 + 1, 205422 + 1, 0};
      Vec3 pos3{780096 + 2.0, 205422 + 2.0, 0};
      Vec3 pos4{780096 + 2.1, 205422 + 2.1, 0};

      Index idx1 = dem.GetNearestCell(pos1);
      Index idx2 = dem.GetNearestCell(pos2);
      Index idx3 = dem.GetNearestCell(pos3);
      Index idx4 = dem.GetNearestCell(pos4);

      ASSERT_EQ(idx1, 0);
      ASSERT_EQ(idx2, 0);
      ASSERT_EQ(idx3, 8);
      ASSERT_EQ(idx4, 8);
   }

   TEST(RasterNew, GetCellPos)
   {
      auto      path = fs::path(std::string(TEST_DIRECTORY) + "/ComputeSlope/7x7.dem");
      RasterNew dem(path.string());

      Vec3 p1 = dem.GetCellPos(0);
      Vec3 p2 = dem.GetCellPos(6);
      Vec3 p3 = dem.GetCellPos(7);
      Vec3 p4 = dem.GetCellPos(9);

      ASSERT_DOUBLE_EQ(p1.x, dem.xllcorner);
      ASSERT_DOUBLE_EQ(p1.y, dem.yllcorner);

      ASSERT_DOUBLE_EQ(p2.x, dem.xllcorner + dem.cellsize * 6);
      ASSERT_DOUBLE_EQ(p2.y, dem.yllcorner);

      ASSERT_DOUBLE_EQ(p3.x, dem.xllcorner);
      ASSERT_DOUBLE_EQ(p3.y, dem.yllcorner + dem.cellsize);

      ASSERT_DOUBLE_EQ(p4.x, dem.xllcorner + 2 * dem.cellsize);
      ASSERT_DOUBLE_EQ(p4.y, dem.yllcorner + 1 * dem.cellsize);
   }

   TEST(RasterNew, GetCellCenter)
   {
      auto      path = fs::path(std::string(TEST_DIRECTORY) + "/ComputeSlope/7x7.dem");
      RasterNew dem(path.string());

      Vec3 p1 = dem.GetCellCenter(0);
      Vec3 p2 = dem.GetCellCenter(6);
      Vec3 p3 = dem.GetCellCenter(7);
      Vec3 p4 = dem.GetCellCenter(9);

      double hs = dem.cellsize / 2.0;

      ASSERT_DOUBLE_EQ(p1.x, dem.xllcorner + hs);
      ASSERT_DOUBLE_EQ(p1.y, dem.yllcorner + hs);

      ASSERT_DOUBLE_EQ(p2.x, dem.xllcorner + dem.cellsize * 6 + hs);
      ASSERT_DOUBLE_EQ(p2.y, dem.yllcorner + hs);

      ASSERT_DOUBLE_EQ(p3.x, dem.xllcorner + hs);
      ASSERT_DOUBLE_EQ(p3.y, dem.yllcorner + dem.cellsize + hs);

      ASSERT_DOUBLE_EQ(p4.x, dem.xllcorner + 2 * dem.cellsize + hs);
      ASSERT_DOUBLE_EQ(p4.y, dem.yllcorner + 1 * dem.cellsize + hs);
   }

   TEST(RasterNew, Rasterize)
   {
      class TestClass
      {
      public:
         TestClass(KiLib::Vec3 pos, double safetyFactor)
         {
            this->pos          = pos;
            this->safetyFactor = safetyFactor;
         }

         KiLib::Vec3 pos;
         double      safetyFactor;
      };

      auto                   cwd  = fs::current_path();
      auto                   path = fs::path(std::string(TEST_DIRECTORY) + "/ComputeSlope/7x3_NODATA.dem");
      RasterNew              dem(path.string());
      std::vector<TestClass> objs{
         {{0.40, 0.30, -1.00}, 0.03}, // 0, 0
         {{0.10, 0.49, -1.00}, 0.12}, // 0, 0
         {{1.00, 1.00, -1.00}, 0.30}, // 1, 1
         {{1.20, 5.10, -1.00}, 0.70}, // 5, 1
         {{1.90, 5.60, -1.00}, 0.30}, // 5, 1
         {{1.40, 5.10, -1.00}, 1.10}, // 5, 1
      };

      //clang-format off
      spdlog::info("Rasterizing No Trunc");
      RasterNew rasterizedNoTrunc = RasterNew::Rasterize<TestClass>(
         dem,                                             // Reference dem to construct sizes
         objs,                                            // Our vector of objects to rasterize
         [](const auto &obj) { return obj.pos; },         // Our objects position
         [](const auto &obj) { return obj.safetyFactor; } // Our objects attribute to rasterize
      );
      //clang-format on

      ASSERT_DOUBLE_EQ(rasterizedNoTrunc(5, 1), 0.70);
      ASSERT_DOUBLE_EQ(rasterizedNoTrunc(1, 1), 0.30);
      ASSERT_DOUBLE_EQ(rasterizedNoTrunc(0, 0), 0.075);

      //clang-format off
      spdlog::info("Rasterizing No Trunc Width");
      RasterNew rasterizedNoTruncWidth = RasterNew::Rasterize<TestClass>(
         dem,                                              // Reference dem to construct sizes
         objs,                                             // Our vector of objects to rasterize
         [](const auto &obj) { return obj.pos; },          // Our objects position
         [](const auto &obj) { return obj.safetyFactor; }, // Our objects attribute to rasterize
         1                                                 // Expand width by 1
      );
      //clang-format on

      ASSERT_DOUBLE_EQ(rasterizedNoTruncWidth(5, 1), 0.70);
      ASSERT_DOUBLE_EQ(rasterizedNoTruncWidth(1, 1), 0.15);
      ASSERT_DOUBLE_EQ(rasterizedNoTruncWidth(0, 0), 0.15);


      //clang-format off
      spdlog::info("Rasterizing Trunk");
      RasterNew rasterizedTrunc = RasterNew::Rasterize<TestClass>(
         dem,                                                   // Reference dem to construct sizes
         objs,                                                  // Our vector of objects to rasterize
         [](const auto &obj) { return obj.pos; },               // Our objects position
         [](const auto &obj) { return obj.safetyFactor < 1.0; } // Our objects attribute to rasterize
      );
      //clang-format on

      ASSERT_DOUBLE_EQ(rasterizedTrunc(5, 1), 2.0 / 3.0);
      ASSERT_DOUBLE_EQ(rasterizedTrunc(1, 1), 1.0);
      ASSERT_DOUBLE_EQ(rasterizedTrunc(0, 0), 1.0);
   }

   TEST(RasterNew, DistFromBoundary)
   {
      auto      path = fs::path(std::string(TEST_DIRECTORY) + "/ComputeSlope/7x7.dem");
      RasterNew dem(path.string());

      Vec3 p1{780096.0, 205422.0, -1.0};
      Vec3 p2{780096.0 + 2.0, 205422.0 + 2.0, -1.0};
      Vec3 p3{780096.0 + 0.1, 205422.0 + 3.0, -1.0};
      Vec3 p4{780096.0 + dem.width - 0.1, 205422.0 + dem.height - 1.0, -1.0};

      ASSERT_DOUBLE_EQ(dem.DistFromBoundary(p1), 0.0);
      ASSERT_DOUBLE_EQ(dem.DistFromBoundary(p2), 2.0);
      ASSERT_NEAR(dem.DistFromBoundary(p3), 0.1, 0.000001); // Float nonsense
      ASSERT_NEAR(dem.DistFromBoundary(p4), 0.1, 0.000001); // Float nonsense
   }

   TEST(RasterNew, GetAverage)
   {
      auto      cwd  = fs::current_path();
      auto      path = fs::path(std::string(TEST_DIRECTORY) + "/ComputeSlope/7x3_NODATA.dem");
      RasterNew dem(path.string());

      double avg1 = dem.GetAverage(0, 4.0);
      ASSERT_DOUBLE_EQ(avg1, 6.0);

      double avg2 = dem.GetAverage(4, 1.0);
      ASSERT_DOUBLE_EQ(avg2, 4.0);

      double avg3 = dem.GetAverage(14, 2.0);
      ASSERT_DOUBLE_EQ(avg3, 13.5);
   }

   TEST(RasterNew, RowColIter)
   {
      RasterNew dem(40, 50);

      std::vector<std::pair<Index, Index>> out;
      for (auto [row, col] : dem.RowColIndexIter())
      {
         out.push_back({row, col});
      }

      Index i = 0;
      for (Index row = 0; row < dem.nRows; row++)
      {
         for (Index col = 0; col < dem.nCols; col++)
         {
            ASSERT_EQ(out[i].first, row);
            ASSERT_EQ(out[i].second, col);
            i += 1;
         }
      }
   }

   TEST(RasterNew, RowColParIter)
   {
      RasterNew dem(40, 50);

      std::vector<std::pair<Index, Index>> out;

      #pragma omp parallel for
      for (auto [row, col] : dem.RowColIndexIter())
      {
         #pragma omp critical
         {
            out.push_back(std::make_pair(row, col));
         }
      }
      // sort vector of pairs by first element (row) and then by second element (col)
      std::sort(
         out.begin(), out.end(),
         [](const auto &a, const auto &b)
         { return (a.first < b.first) || (a.first == b.first && a.second < b.second); });

      std::vector<std::pair<Index, Index>> expected;
      for (Index row = 0; row < dem.nRows; row++)
      {
         for (Index col = 0; col < dem.nCols; col++)
         {
            expected.push_back(std::make_pair(row, col));
         }
      }

      for (Index i = 0; i < expected.size(); i++)
      {
         spdlog::debug("({} {}) | ({}, {})", out[i].first, out[i].second, expected[i].first, expected[i].second);
      }


      ASSERT_EQ(out, expected);
   }
} // namespace KiLib
