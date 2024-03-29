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


#include <KiLib/Raster/Raster.hpp>
#include <filesystem>
#include <gtest/gtest.h>
#include <numeric>
#include <spdlog/fmt/ostr.h>
#include <spdlog/spdlog.h>

#ifdef __APPLE__
namespace fs = std::__fs::filesystem;
#else
namespace fs = std::filesystem;
#endif


namespace KiLib
{
   void compareMSE(const Raster &r1, const Raster &r2, double meta_dt, double data_dt)
   {
      // METADATA
      ASSERT_EQ(r1.nCols, r2.nCols);
      ASSERT_EQ(r1.nRows, r2.nRows);
      ASSERT_EQ(r1.nData, r2.nData);

      ASSERT_LT(std::abs(r1.xllcorner - r2.xllcorner), meta_dt);
      ASSERT_LT(std::abs(r1.yllcorner - r2.yllcorner), meta_dt);
      ASSERT_LT(std::abs(r1.cellsize - r2.cellsize), meta_dt);
      ASSERT_LT(std::abs(r1.cellsize - r2.cellsize), meta_dt);
      ASSERT_LT(std::abs(r1.width - r2.width), meta_dt);
      ASSERT_LT(std::abs(r1.height - r2.height), meta_dt);
      ASSERT_LT(std::abs(r1.nodata_value - r2.nodata_value), meta_dt);

      // DATA
      auto mse = std::inner_product(
                    r1.data.begin(), r1.data.end(), r2.data.begin(), 0, std::plus<double>(),
                    [](double a, double b) { return std::pow(a - b, 2); }) /
                 r1.data.size();

      ASSERT_LE(mse, data_dt);
   }

   TEST(Raster, ComputeSlopeZevenbergenThorne)
   {
      KiLib::Raster::SlopeMethod method = KiLib::Raster::SlopeMethod::ZevenbergenThorne;

      auto                     cwd  = fs::current_path();
      auto                     path = fs::path(std::string(TEST_DIRECTORY) + "/ComputeSlope/");
      std::vector<std::string> sizes{"5x5", "7x7"};

      fs::current_path(path);

      for (const std::string &size : sizes)
      {
         std::string base = (path / size).string();

         Raster dem(base + ".dem");
         Raster slope_ref(base + "_slope.dem");
         Raster slope_cal = dem.ComputeSlope(method);

         compareMSE(slope_ref, slope_cal, 1e-300, 1e-100);
      }
   }

   TEST(Raster, getCellPos)
   {
      auto                     cwd  = fs::current_path();
      auto                     path = fs::path(std::string(TEST_DIRECTORY) + "/ComputeSlope/");
      std::vector<std::string> sizes{"5x5", "7x7", "7x3"};

      fs::current_path(path);

      for (const std::string &size : sizes)
      {
         std::string base = (path / size).string();

         Raster dem(base + ".dem");
         for (size_t r = 0; r < dem.nRows; r++)
         {
            for (size_t c = 0; c < dem.nCols; c++)
            {
               size_t      ind = dem.flattenIndex(r, c);
               KiLib::Vec3 pos = dem.getCellPos(ind);
               double      z   = dem.at(r, c);

               ASSERT_DOUBLE_EQ(pos.z, z);
               ASSERT_DOUBLE_EQ(pos.y, dem.yllcorner + (double)r * dem.cellsize);
               ASSERT_DOUBLE_EQ(pos.x, dem.xllcorner + (double)c * dem.cellsize);
               ASSERT_EQ(ind, dem.getNearestCell(pos));
            }
         }
      }
   }

   TEST(Raster, getInterpBilinear)
   {
      auto cwd  = fs::current_path();
      auto path = fs::path(std::string(TEST_DIRECTORY) + "/ComputeSlope/");

      fs::current_path(path);

      Raster      dem("5x5.dem");
      KiLib::Vec3 p1{780099.2947926898, 205426.59516664856, 1909.8001915739701};
      KiLib::Vec3 p2{780099.3343145008, 205427.51547149016, 1910.3570077815384};
      KiLib::Vec3 p3{780096.0843788842, 205427.65816391885, 1910.520074732899};

      double z1 = dem(p1);
      ASSERT_DOUBLE_EQ(z1, p1.z);

      double z2 = dem(p2);
      ASSERT_DOUBLE_EQ(z2, p2.z);

      double z3 = dem(p3);
      ASSERT_DOUBLE_EQ(z3, p3.z);
   }

   TEST(Raster, GetCoordMinDistance)
   {
      auto cwd  = fs::current_path();
      auto path = fs::path(std::string(TEST_DIRECTORY) + "/ComputeSlope/7x7_CA.dem");

      Raster dem(path.string());

      size_t                     ind       = 30;
      const auto                 zInd      = 100.0;
      const auto                 radius    = 8.0;
      const auto                 threshold = 21.0;
      std::optional<KiLib::Vec3> pos       = dem.GetCoordMinDistance(ind, zInd, dem, radius, threshold);
      if (pos)
      {
         ASSERT_DOUBLE_EQ(0, (*pos).z);
      }
      else
      {
         ASSERT_DOUBLE_EQ(1.0, 2.0);
      }
   }

   TEST(Raster, Rasterize)
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
      Raster                 dem(path.string());
      std::vector<TestClass> objs{
         {{0.40, 0.30, -1.00}, 0.03}, // 0, 0
         {{0.10, 0.49, -1.00}, 0.12}, // 0, 0
         {{1.00, 1.00, -1.00}, 0.30}, // 1, 1
         {{1.20, 5.10, -1.00}, 0.70}, // 5, 1
         {{1.90, 5.60, -1.00}, 0.30}, // 5, 1
         {{1.40, 5.10, -1.00}, 1.10}, // 5, 1
      };

      //clang-format off
      KiLib::Raster rasterizedNoTrunc = KiLib::Raster::Rasterize<TestClass>(
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
      KiLib::Raster rasterizedNoTruncWidth = KiLib::Raster::Rasterize<TestClass>(
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
      KiLib::Raster rasterizedTrunc = KiLib::Raster::Rasterize<TestClass>(
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


   TEST(Raster, GetAverage)
   {
      auto   cwd  = fs::current_path();
      auto   path = fs::path(std::string(TEST_DIRECTORY) + "/ComputeSlope/7x3_NODATA.dem");
      Raster dem(path.string());

      double avg1 = dem.GetAverage(0, 4.0);
      ASSERT_DOUBLE_EQ(avg1, 6.0);

      double avg2 = dem.GetAverage(4, 1.0);
      ASSERT_DOUBLE_EQ(avg2, 4.0);

      double avg3 = dem.GetAverage(14, 2.0);
      ASSERT_DOUBLE_EQ(avg3, 13.5);
   }

   TEST(Raster, fillLike)
   {
      auto   cwd  = fs::current_path();
      auto   path = fs::path(std::string(TEST_DIRECTORY) + "/ComputeSlope/7x3_NODATA.dem");
      Raster dem(path.string());

      Raster filledNODATA = Raster::fillLike(dem, 1.0, true);
      Raster fullFilled   = Raster::fillLike(dem, 0.0, false);

      double sumNODATA = std::accumulate(filledNODATA.data.begin(), filledNODATA.data.end(), 0.0);
      ASSERT_EQ(filledNODATA.nRows, dem.nRows);
      ASSERT_EQ(filledNODATA.nCols, dem.nCols);
      ASSERT_EQ(filledNODATA.xllcorner, dem.xllcorner);
      ASSERT_EQ(filledNODATA.yllcorner, dem.yllcorner);
      ASSERT_EQ(filledNODATA.cellsize, dem.cellsize);
      ASSERT_EQ(filledNODATA.nodata_value, dem.nodata_value);
      ASSERT_EQ(sumNODATA, -99979.0);

      double sumFull = std::accumulate(fullFilled.data.begin(), fullFilled.data.end(), 0.0);
      ASSERT_EQ(fullFilled.nRows, dem.nRows);
      ASSERT_EQ(fullFilled.nCols, dem.nCols);
      ASSERT_EQ(fullFilled.xllcorner, dem.xllcorner);
      ASSERT_EQ(fullFilled.yllcorner, dem.yllcorner);
      ASSERT_EQ(fullFilled.cellsize, dem.cellsize);
      ASSERT_EQ(fullFilled.nodata_value, dem.nodata_value);
      ASSERT_EQ(sumFull, 0);
   }

   TEST(Raster, assertAgreeDim)
   {
      auto cwd = fs::current_path();

      auto path1 = fs::path(std::string(TEST_DIRECTORY) + "/ComputeSlope/7x7.dem");
      auto path2 = fs::path(std::string(TEST_DIRECTORY) + "/ComputeSlope/7x7_slope.dem");
      auto path3 = fs::path(std::string(TEST_DIRECTORY) + "/ComputeSlope/7x3.dem");

      Raster dem1(path1.string());
      Raster dem2(path2.string());
      Raster dem3(path3.string());

      Raster::assertAgreeDim({&dem1, &dem2, &dem1, &dem2});

      // this tests that the expected exception is thrown
      EXPECT_THROW(Raster::assertAgreeDim({&dem1, &dem3}), std::invalid_argument);
   }

   TEST(Raster, getValidIndices)
   {
      auto cwd = fs::current_path();

      auto path1 = fs::path(std::string(TEST_DIRECTORY) + "/ComputeSlope/validRast1.dem");
      auto path2 = fs::path(std::string(TEST_DIRECTORY) + "/ComputeSlope/validRast2.dem");
      auto path3 = fs::path(std::string(TEST_DIRECTORY) + "/ComputeSlope/7x3.dem");

      Raster dem1(path1.string());
      Raster dem2(path2.string());
      Raster dem3(path3.string());

      // this tests that the expected exception is thrown
      EXPECT_THROW(Raster::getValidIndices({&dem1, &dem3}), std::invalid_argument);

      std::vector<size_t> inds = Raster::getValidIndices({&dem1, &dem2});
      ASSERT_TRUE(std::is_sorted(inds.begin(), inds.end()));

      // Make sure all inds are valid data
      for (auto ind : inds)
      {
         ASSERT_NE(dem1(ind), dem1.nodata_value);
         ASSERT_NE(dem2(ind), dem2.nodata_value);
      }

      // Make sure all valid indices are in inds
      for (size_t i = 0; i < dem1.data.size(); i++)
      {
         if ((dem1(i) == dem1.nodata_value) or (dem2(i) == dem2.nodata_value))
         {
            continue;
         }

         ASSERT_TRUE(std::find(inds.begin(), inds.end(), i) != inds.end());
      }
   }
} // namespace KiLib
