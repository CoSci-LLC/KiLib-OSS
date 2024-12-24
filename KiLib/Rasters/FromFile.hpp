#pragma  once

#include <KiLib/Rasters/Raster.hpp>
#include <KiLib/Rasters/IO/Raster_DEM.hpp>
#include <KiLib/Rasters/IO/Raster_TIFF.hpp>

#include <filesystem>
namespace KiLib::Rasters {

   // Load data in Raster format from specified path
    template<typename T>
   Raster<T> FromFile(const std::string &path, std::function<bool(T&, double, bool)> construct_val)
   {
      auto ext = std::filesystem::path(path).extension();

      if (ext == ".asc" || ext == ".dem")
         return fromDEM( path, construct_val );
      else if (ext == ".tif" || ext == ".tiff")
         return fromTiff( path, construct_val );
      else
      {
          std::string err = fmt::format("Unsupported file type given to raster constructor: {}", ext.string());
         throw std::invalid_argument(err);
      }
   }

}
