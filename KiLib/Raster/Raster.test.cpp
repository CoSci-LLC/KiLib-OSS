#include <KiLib/Raster/Raster.hpp>
#include <cstdlib>
#include <filesystem>
#include <gtest/gtest.h>
#include <numeric>
#include <spdlog/spdlog.h>
#include <unistd.h>

namespace fs = std::filesystem;

namespace KiLib
{

   std::vector<std::pair<std::string, std::string>> generate_comparison()
   {
      std::vector<std::pair<std::string, std::string>> out;

      auto cwd  = fs::current_path();
      auto path = fs::path(std::string(TEST_DIRECTORY) + "/TIFF/");

      fs::current_path(path);

      spdlog::debug("Looking for tif files in: {}", path.string());

      for (auto it : fs::directory_iterator(path))
      {
         if (it.is_regular_file() && it.path().extension() == ".tif")
         {
            std::system(
               fmt::format("gdal_translate -of AAIGrid {} discard.asc", it.path().filename().string()).c_str());
            std::system(
               fmt::format(
                  "gdal_translate -of AAIGrid -co FORCE_CELLSIZE=TRUE discard.asc {}.asc", it.path().stem().string())
                  .c_str());
         }
         auto tif = path / fs::path(it.path().stem().string() + ".tif");
         auto asc = path / fs::path(it.path().stem().string() + ".asc");
         out.emplace_back(std::make_pair<std::string, std::string>(tif, asc));
      }

      fs::current_path(cwd);

      return out;
   }

   void cleanup_comparison()
   {
      auto cwd  = fs::current_path();
      auto path = fs::path(std::string(TEST_DIRECTORY) + "/TIFF/");

      fs::current_path(path);

      for (auto it : fs::directory_iterator(path))
      {
         if (!it.is_regular_file())
            continue;

         fs::path    tmp = it;
         std::string ext = "";

         while (tmp.has_extension())
         {
            ext = tmp.extension().string() + ext;
            tmp.replace_extension();
         }

         if (ext != ".tif")
            fs::remove(it);
      }

      fs::current_path(cwd);
   }

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

   TEST(Raster, CompareTiff2GDAL)
   {
      for (const auto &[tif, asc] : generate_comparison())
         compare_raster(Raster(tif), Raster(asc), 1e-300, 1e-100);

      cleanup_comparison();
   }

   TEST(Raster, CompareTiff2Tiff)
   {
      auto cwd  = fs::current_path();
      auto path = fs::path(std::string(TEST_DIRECTORY) + "/TIFF/");

      fs::current_path(path);

      for (auto it : fs::directory_iterator(path))
      {
         if (!it.is_regular_file() && it.path().extension() != ".tif")
            continue;

         fs::path cp_path = it;

         cp_path.replace_extension(".copy.tif");

         Raster org(it.path().string());

         org.writeToFile(cp_path);

         Raster copy(cp_path.string());

         compare_raster(org, copy, 1e-300, 1e-100);
      }

      fs::current_path(cwd);

      cleanup_comparison();
   }

   TEST(Raster, CompareTiff2DEM)
   {
      auto cwd  = fs::current_path();
      auto path = fs::path(std::string(TEST_DIRECTORY) + "/TIFF/");

      fs::current_path(path);

      for (auto it : fs::directory_iterator(path))
      {
         if (!it.is_regular_file() && it.path().extension() != ".tif")
            continue;

         fs::path cp_path = it;

         cp_path.replace_extension(".asc");

         Raster org(it.path().string());

         org.writeToFile(cp_path);

         Raster copy(cp_path.string());

         compare_raster(org, copy, 1e-300, 1e-100);
      }

      fs::current_path(cwd);

      cleanup_comparison();
   }
} // namespace KiLib