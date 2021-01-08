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

      for (auto it : fs::directory_iterator(path))
      {
         std::string base = (path / fs::path(it.path().stem().string())).string();
         files.insert(std::make_pair<std::string, std::string>(base + ".tif", base + ".asc"));
      }

      for (const auto &[tif, asc] : files)
         compare_raster(Raster(tif), Raster(asc), 1e-300, 1e-100);

      fs::current_path(cwd);
   }

   TEST(Raster, Produce_CompareDEM2Tiff)
   {
      std::set<std::pair<std::string, std::string>> files;

      auto cwd  = fs::current_path();
      auto path = fs::path(std::string(TEST_DIRECTORY) + "/TIFF/");

      fs::current_path(path);

      for (auto it : fs::directory_iterator(path))
      {
         if (!it.is_regular_file() && it.path().extension() != ".tif")
            continue;

         std::string base = (path / fs::path(it.path().stem().string())).string();

         files.insert(std::make_pair<std::string, std::string>(base + ".asc", base + "_comparison.tif"));

         Raster tmp(base + ".asc");
         tmp.writeToFile(base + "_comparison.tif");
      }

      for (const auto &[asc, tif] : files)
      {
         compare_raster(Raster(asc), Raster(tif), 1e-300, 1e-100);
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

      for (auto it : fs::directory_iterator(path))
      {
         if (!it.is_regular_file() && it.path().extension() != ".tif")
            continue;

         std::string base = (path / fs::path(it.path().stem().string())).string();

         files.insert(std::make_pair<std::string, std::string>(base + ".asc", base + "_comparison.asc"));

         Raster tmp(base + ".asc");
         tmp.writeToFile(base + "_comparison.asc");
      }

      for (const auto &[asc, cmp] : files)
      {
         compare_raster(Raster(asc), Raster(cmp), 1e-300, 1e-100);
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

      for (auto it : fs::directory_iterator(path))
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

      for (auto it : fs::directory_iterator(path))
      {
         if (!it.is_regular_file() && it.path().extension() != ".tif")
            continue;

         std::string base = (path / fs::path(it.path().stem().string())).string();

         files.insert(std::make_pair<std::string, std::string>(base + ".tif", base + "_comparison.asc"));

         Raster tmp(base + ".tif");
         tmp.writeToFile(base + "_comparison.asc");
      }

      for (const auto &[tif, asc] : files)
      {
         compare_raster(Raster(tif), Raster(asc), 1e-300, 1e-100);
         fs::remove(fs::path(asc));
      }

      fs::current_path(cwd);
   }
} // namespace KiLib