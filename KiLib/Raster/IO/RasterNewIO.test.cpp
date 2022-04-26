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
#include <KiLib/Raster/RasterNew.hpp>
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
   TEST(RasterNew, fromDEM)
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
      auto      path = fs::path(std::string(TEST_DIRECTORY) + "/ComputeSlope/5x5.dem");
      RasterNew dem(path.string());

      ASSERT_EQ(dem.nRows, 5);
      ASSERT_EQ(dem.nCols, 5);
      ASSERT_EQ(dem.nData, 25);
      ASSERT_EQ(dem.nodata_value, -99999);
      ASSERT_DOUBLE_EQ(dem.xllcorner, 780096.0);
      ASSERT_DOUBLE_EQ(dem.yllcorner, 205422.0);
      ASSERT_DOUBLE_EQ(dem.cellsize, 2.0);

      ASSERT_DOUBLE_EQ(dem.at(0, 0), 1906.551147);      // top left, but lower left in file
      ASSERT_DOUBLE_EQ(dem.at(4, 0), 1912.436157);      // bottom left, but top left in file
      ASSERT_DOUBLE_EQ(dem.at(0, 4), dem.nodata_value); // top right, but lower left in file
      ASSERT_DOUBLE_EQ(dem.at(4, 4), 1912.221558);      // bottom right, but top left in file
   }

   TEST(RasterNew, toDEM)
   {
      auto cwd  = fs::current_path();
      auto path = fs::path(std::string(TEST_DIRECTORY) + "/ComputeSlope/");

      fs::current_path(path);

      for (const auto &it : fs::directory_iterator(path))
      {
         if (!it.is_regular_file() && it.path().extension() != ".dem")
            continue;

         RasterNew orig_in = RasterNew(it.path().string());

         fs::path outpath = fs::temp_directory_path() / it.path().filename();
         orig_in.writeToFile(outpath.string());

         RasterNew new_in = RasterNew(outpath.string());

         ASSERT_EQ(orig_in.nRows, new_in.nRows);
         ASSERT_EQ(orig_in.nCols, new_in.nCols);
         ASSERT_EQ(orig_in.nData, new_in.nData);
         ASSERT_EQ(orig_in.nodata_value, new_in.nodata_value);
         ASSERT_DOUBLE_EQ(orig_in.xllcorner, new_in.xllcorner);
         ASSERT_DOUBLE_EQ(orig_in.yllcorner, new_in.yllcorner);
         ASSERT_DOUBLE_EQ(orig_in.cellsize, new_in.cellsize);

         ASSERT_EQ(orig_in.data.isApprox(new_in.data), true);
      }
   }

   TEST(RasterNew, toTiff)
   {
      auto path = fs::path(std::string(TEST_DIRECTORY) + "/TIFF/5x5.tif");

      RasterNew orig_in = RasterNew(path.string());

      fs::path outpath = fs::temp_directory_path() / path.filename();
      orig_in.writeToFile(outpath.string());

      RasterNew new_in = RasterNew(outpath.string());

      ASSERT_EQ(orig_in.nRows, new_in.nRows);
      ASSERT_EQ(orig_in.nCols, new_in.nCols);
      ASSERT_EQ(orig_in.nData, new_in.nData);
      ASSERT_EQ(orig_in.nodata_value, new_in.nodata_value);
      ASSERT_DOUBLE_EQ(orig_in.xllcorner, new_in.xllcorner);
      ASSERT_DOUBLE_EQ(orig_in.yllcorner, new_in.yllcorner);
      ASSERT_DOUBLE_EQ(orig_in.cellsize, new_in.cellsize);

      ASSERT_EQ(orig_in.data.isApprox(new_in.data), true);
   }

   TEST(RasterNew, fromTiff)
   {
      auto      path = fs::path(std::string(TEST_DIRECTORY) + "/TIFF/5x5.tif");
      RasterNew dem(path.string());

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
