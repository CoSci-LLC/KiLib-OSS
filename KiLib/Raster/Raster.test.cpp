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
#include <spdlog/spdlog.h>

namespace fs = std::filesystem;

namespace KiLib
{
   void compare_raster(Raster r1, Raster r2, double meta_dt, double data_dt)
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

   TEST(Raster, CompareTiff2DEM)
   {
      std::set<std::pair<std::string, std::string>> files;

      fs::path cwd  = fs::current_path();
      fs::path path = fs::path(std::string(TEST_DIRECTORY) + "/TIFF/");

      fs::current_path(path);

      for (const auto &it : fs::directory_iterator(path))
      {
         std::string base = (path / fs::path(it.path().stem().string())).string();
         files.insert(std::make_pair<std::string, std::string>(base + ".tif", base + ".dem"));
      }

      for (const auto &[tif, dem] : files)
         compare_raster(Raster(tif), Raster(dem), 1e-300, 1e-100);

      fs::current_path(cwd);
   }

   TEST(Raster, Produce_CompareDEM2Tiff)
   {
      std::set<std::pair<std::string, std::string>> files;

      auto cwd  = fs::current_path();
      auto path = fs::path(std::string(TEST_DIRECTORY) + "/TIFF/");

      fs::current_path(path);

      for (const auto &it : fs::directory_iterator(path))
      {
         if (!it.is_regular_file() && it.path().extension() != ".tif")
            continue;

         std::string base = (path / fs::path(it.path().stem().string())).string();

         files.insert(std::make_pair<std::string, std::string>(base + ".dem", base + "_comparison.tif"));

         Raster tmp(base + ".dem");
         tmp.writeToFile(base + "_comparison.tif");
      }

      for (const auto &[dem, tif] : files)
      {
         compare_raster(Raster(dem), Raster(tif), 1e-300, 1e-100);
         fs::remove(fs::path(tif));
      }

      fs::current_path(cwd);
   }

   TEST(Raster, Produce_CompareDEM2DEM)
   {
      std::set<std::pair<std::string, std::string>> files;

      auto cwd  = fs::current_path();
      auto path = fs::path(std::string(TEST_DIRECTORY) + "/TIFF/");

      fs::current_path(path);

      for (const auto &it : fs::directory_iterator(path))
      {
         if (!it.is_regular_file() && it.path().extension() != ".tif")
            continue;

         std::string base = (path / fs::path(it.path().stem().string())).string();

         files.insert(std::make_pair<std::string, std::string>(base + ".dem", base + "_comparison.dem"));

         Raster tmp(base + ".dem");
         tmp.writeToFile(base + "_comparison.dem");
      }

      for (const auto &[dem, cmp] : files)
      {
         compare_raster(Raster(dem), Raster(cmp), 1e-300, 1e-100);
         fs::remove(fs::path(cmp));
      }

      fs::current_path(cwd);
   }

   TEST(Raster, Produce_CompareTiff2Tiff)
   {
      std::set<std::pair<std::string, std::string>> files;

      auto cwd  = fs::current_path();
      auto path = fs::path(std::string(TEST_DIRECTORY) + "/TIFF/");

      fs::current_path(path);

      for (const auto &it : fs::directory_iterator(path))
      {
         if (!it.is_regular_file() && it.path().extension() != ".tif")
            continue;

         std::string base = (path / fs::path(it.path().stem().string())).string();

         files.insert(std::make_pair<std::string, std::string>(base + ".tif", base + "_comparison.tif"));

         Raster tmp(base + ".tif");
         tmp.writeToFile(base + "_comparison.tif");
      }

      for (const auto &[tif, cmp] : files)
      {
         compare_raster(Raster(tif), Raster(cmp), 1e-300, 1e-100);
         fs::remove(fs::path(cmp));
      }

      fs::current_path(cwd);
   }

   TEST(Raster, Produce_CompareTIFF2DEM)
   {
      std::set<std::pair<std::string, std::string>> files;

      auto cwd  = fs::current_path();
      auto path = fs::path(std::string(TEST_DIRECTORY) + "/TIFF/");

      fs::current_path(path);

      for (const auto &it : fs::directory_iterator(path))
      {
         if (!it.is_regular_file() && it.path().extension() != ".tif")
            continue;

         std::string base = (path / fs::path(it.path().stem().string())).string();

         files.insert(std::make_pair<std::string, std::string>(base + ".tif", base + "_comparison.dem"));

         Raster tmp(base + ".tif");
         tmp.writeToFile(base + "_comparison.dem");
      }

      for (const auto &[tif, dem] : files)
      {
         compare_raster(Raster(tif), Raster(dem), 1e-300, 1e-100);
         fs::remove(fs::path(dem));
      }

      fs::current_path(cwd);
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

         compare_raster(slope_ref, slope_cal, 1e-300, 1e-100);
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
         {{0.4, 0.3, -1.0}, 0.03},  // 0, 0
         {{0.1, 0.49, -1.0}, 0.12}, // 0, 0
         {{0.5, 0.5, -1.0}, 0.3},   // 1, 1
         {{1.2, 5.1, -1.0}, 0.7},   // 5, 1
         {{0.9, 4.6, -1.0}, 0.3},   // 5, 1
         {{1.4, 4.6, -1.0}, 1.1},   // 5, 1
      };

      //clang-format off
      KiLib::Raster rasterizedNoTrunc = KiLib::Raster::Rasterize<TestClass>(
         dem,                                                  // Reference dem to construct sizes
         objs,                                                 // Our vector of objects to rasterize
         [](const TestClass &obj) { return obj.pos; },         // Our objects position
         [](const TestClass &obj) { return obj.safetyFactor; } // Our objects attribute to rasterize
      );
      //clang-format on

      ASSERT_DOUBLE_EQ(rasterizedNoTrunc(5, 1), 0.70);
      ASSERT_DOUBLE_EQ(rasterizedNoTrunc(1, 1), 0.30);
      ASSERT_DOUBLE_EQ(rasterizedNoTrunc(0, 0), 0.075);

      //clang-format off
      KiLib::Raster rasterizedTrunc = KiLib::Raster::Rasterize<TestClass>(
         dem,                                                        // Reference dem to construct sizes
         objs,                                                       // Our vector of objects to rasterize
         [](const TestClass &obj) { return obj.pos; },               // Our objects position
         [](const TestClass &obj) { return obj.safetyFactor < 1.0; } // Our objects attribute to rasterize
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
} // namespace KiLib