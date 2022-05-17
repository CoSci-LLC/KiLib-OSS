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


#include <Eigen/Eigen>
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

void compareMSE(const KiLib::Raster &r1, const KiLib::Raster &r2, double meta_dt, double data_dt)
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

   ASSERT_EQ(r1.data.isApprox(r2.data, data_dt), true);
}

namespace KiLib
{
   TEST(Raster, fromDEM)
   {
      /*
         ncols        5
         nrows        5
         xllcorner    780096.000000000000
         yllcorner    205422.000000000000
         cellsize     2.000000000000
         NODATA_value  -99999
         1912.436157 1912.276245 1912.256348 1912.411377 1912.221558
         1910.791382 1910.591675 1910.676758 1910.956787 1911.081787
         1909.241943 1909.312012 1909.512085 1909.781982 1909.876953
         1907.911621 1908.201538 1908.396484 1908.451416 1908.31665
         1906.551147 1906.841187 1907.021243 1907.016235 -99999
      */
      auto   path = fs::path(std::string(TEST_DIRECTORY) + "/ComputeSlope/5x5.dem");
      Raster dem(path.string());

      ASSERT_EQ(dem.nRows, 5);
      ASSERT_EQ(dem.nCols, 5);
      ASSERT_EQ(dem.nData, 25);
      ASSERT_EQ(dem.nodata_value, -99999);
      ASSERT_DOUBLE_EQ(dem.xllcorner, 780096.0);
      ASSERT_DOUBLE_EQ(dem.yllcorner, 205422.0);
      ASSERT_DOUBLE_EQ(dem.cellsize, 2.0);

      ASSERT_DOUBLE_EQ(dem.at(0, 0), 1906.5511474609375); // top left, but lower left in file
      ASSERT_DOUBLE_EQ(dem.at(4, 0), 1912.4361572265625); // bottom left, but top left in file
      ASSERT_DOUBLE_EQ(dem.at(0, 4), dem.nodata_value);   // top right, but lower left in file
      ASSERT_DOUBLE_EQ(dem.at(4, 4), 1912.2215576171875); // bottom right, but top left in file
   }

   TEST(Raster, toDEM)
   {
      auto cwd  = fs::current_path();
      auto path = fs::path(std::string(TEST_DIRECTORY) + "/ComputeSlope/");

      fs::current_path(path);

      for (const auto &it : fs::directory_iterator(path))
      {
         if (!it.is_regular_file() || it.path().extension() != ".dem")
            continue;

         spdlog::info("###############################################");
         spdlog::info("Reading from: {}", it.path().string());
         Raster orig_in = Raster(it.path().string());

         fs::path outpath = fs::temp_directory_path() / it.path().filename();
         spdlog::info("Writing to:   {}", outpath.string());
         orig_in.WriteToFile(outpath.string());

         spdlog::info("Reading output from: {}", outpath.string());
         Raster new_in = Raster(outpath.string());

         compareMSE(orig_in, new_in, 1e-5, 1e-10);
      }
   }

   TEST(Raster, toTiff)
   {
      auto path = fs::path(std::string(TEST_DIRECTORY) + "/TIFF/5x5.tif");

      Raster orig_in = Raster(path.string());

      fs::path outpath = fs::temp_directory_path() / path.filename();
      orig_in.WriteToFile(outpath.string());

      Raster new_in = Raster(outpath.string());
      compareMSE(orig_in, new_in, 1e-5, 1e-10);
   }

   TEST(Raster, fromTiff)
   {
      auto   path = fs::path(std::string(TEST_DIRECTORY) + "/TIFF/5x5.tif");
      Raster dem(path.string());

      ASSERT_EQ(dem.nRows, 5);
      ASSERT_EQ(dem.nCols, 5);
      ASSERT_EQ(dem.nData, 25);
      ASSERT_EQ(dem.nodata_value, -99999);
      ASSERT_DOUBLE_EQ(dem.xllcorner, 780096.0);
      ASSERT_DOUBLE_EQ(dem.yllcorner, 205422.0);
      ASSERT_DOUBLE_EQ(dem.cellsize, 2.0);

      ASSERT_DOUBLE_EQ(dem.at(0, 0), 1906.5511474609375); // top left, but lower left in file
      ASSERT_DOUBLE_EQ(dem.at(4, 0), 1912.4361572265625); // bottom left, but top left in file
      ASSERT_DOUBLE_EQ(dem.at(0, 4), dem.nodata_value);   // top right, but lower left in file
      ASSERT_DOUBLE_EQ(dem.at(4, 4), 1912.2215576171875); // bottom right, but top left in file
   }
} // namespace KiLib
