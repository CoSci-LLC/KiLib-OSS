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
   void compare_raster(const Raster& r1, const Raster& r2, double meta_dt, double data_dt)
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
} // namespace KiLib
