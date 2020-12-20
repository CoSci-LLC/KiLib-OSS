#include <KiLib/Raster/Raster.hpp>
#include <libtiff/tiffio.hxx>
#include <spdlog/fmt/ostr.h>
#include <spdlog/spdlog.h>

namespace KiLib
{

   Raster::Raster()
   {
      this->constructed = false;
   }

   // Load data in Raster format from specified path
   Raster::Raster(std::string path)
   {
      this->constructed = false;

      std::ifstream rasterFile;
      std::string line, key;
      std::istringstream stream(line);

      // Open Elevation file
      rasterFile.open(path, std::ios::in);
      if (!rasterFile.is_open()) {
         printf("Could not open %s\n", path.c_str());
         exit(EXIT_FAILURE);
      }

      // Load header
      for (int i = 0; i < 6; i++) {
         getline(rasterFile, line);
         stream.str(line);
         stream.clear();

         stream >> key;

         // Force lower-case keys
         std::transform(key.begin(), key.end(), key.begin(), ::tolower);

         if (key == "ncols") {
            stream >> this->nCols;
         } else if (key == "nrows") {
            stream >> this->nRows;
         } else if (key == "xllcorner") {
            stream >> this->xllcorner;
         } else if (key == "yllcorner") {
            stream >> this->yllcorner;
         } else if (key == "cellsize") {
            stream >> this->cellsize;
         } else if (key == "nodata_value") {
            stream >> this->nodata_value;
         } else {
            std::cerr << "Unexpected value in header!" << std::endl;
            return;
         }
      }

      this->data.resize(nRows * nCols);
      // Load elevations
      for (int row = 0; row < this->nRows; row++) {
         getline(rasterFile, line);
         stream.str(line);
         stream.clear();

         // Moving along a row (across columns) is movement through X
         // Moving down a column (across rows) is movement through Y
         for (int col = 0; col < this->nCols; col++) {
            double value;
            stream >> value;
            this->at(this->nRows - row - 1, col) = value;
         }
      }

      this->width       = (this->nCols - 1) * this->cellsize;
      this->height      = (this->nRows - 1) * this->cellsize;
      this->constructed = true;

      rasterFile.close();
   }

   KiLib::Raster fromTiff(const std::string path)
   {
      TIFF *tiff = TIFFOpen(path.c_str(), "r");

      if (tiff == NULL) {
         spdlog::error("Failed to open {}", path);
         exit(EXIT_FAILURE);
      }

      size_t img_width   = 0;
      size_t img_height  = 0;
      size_t tile_width  = 0;
      size_t tile_height = 0;
      size_t x           = 0;
      size_t y           = 0;

      tdata_t buf;

      TIFFGetField(tiff, TIFFTAG_IMAGEWIDTH, &img_width);
      TIFFGetField(tiff, TIFFTAG_IMAGELENGTH, &img_height);
      TIFFGetField(tiff, TIFFTAG_TILEWIDTH, &tile_width);
      TIFFGetField(tiff, TIFFTAG_TILELENGTH, &tile_height);

      buf = _TIFFmalloc(TIFFTileSize(tiff));

      for (y = 0; y < img_height; y += tile_height) {
         for (x = 0; x < img_width; x += tile_width) {
            if (TIFFReadTile(tiff, buf, x, y, 0, 0) == -1) {
               spdlog::error("Error reading raster data");
               exit(EXIT_FAILURE);
            }
         }
      }

      TIFFClose(tiff);

      return Raster();
   }

   // Returns (bilinear) interpolated data value at specified position
   // Takes in a vec3 for convenience, ignores Z
   double Raster::getInterpBilinear(const Vec3 &pos) const
   {
      double r = std::clamp((pos.y - this->yllcorner) / this->cellsize, 0.0, (double)this->nRows - 1);
      double c = std::clamp((pos.x - this->xllcorner) / this->cellsize, 0.0, (double)this->nCols - 1);

      if (r == floor(r) && c == floor(c)) {
         return this->at(r, c);
      }

      const double y1  = floor(r) * this->cellsize;
      const double y2  = ceil(r) * this->cellsize;
      const double x1  = floor(c) * this->cellsize;
      const double x2  = ceil(c) * this->cellsize;
      const double q11 = this->operator()(floor(r), floor(c));
      const double q12 = this->operator()(ceil(r), floor(c));
      const double q21 = this->operator()(floor(r), ceil(c));
      const double q22 = this->operator()(ceil(r), ceil(c));

      const double x = c * this->cellsize;
      const double y = r * this->cellsize;

      double fy1 = q12;
      double fy2 = q11;
      if (x2 != x1) {
         fy1 = ((x2 - x) / (x2 - x1)) * q11 + ((x - x1) / (x2 - x1)) * q21;
         fy2 = ((x2 - x) / (x2 - x1)) * q12 + ((x - x1) / (x2 - x1)) * q22;
      }

      double z = 0;
      if (y2 != y1) {
         z = ((y2 - y) / (y2 - y1)) * fy1 + ((y - y1) / (y2 - y1)) * fy2;
      } else {
         z = (fy1 + fy2) / 2.0;
      }

      return z;
   }

   // Print Raster metadata
   void Raster::print()
   {
      fmt::print(
         "ncols {}\n"
         "nrows {}\n"
         "xllcorner {:.4f}\n"
         "yllcorner {:.4f}\n"
         "cellsize {:.4f}\n"
         "NODATA_value {}\n",
         this->nCols, this->nRows, this->xllcorner, this->yllcorner, this->cellsize, this->nodata_value);

      for (int row = (this->nRows - 1); row >= 0; row--) {
         for (int col = 0; col < this->nCols; col++) {
            double val = this->operator()(row, col);

            if (val == this->nodata_value) {
               fmt::print("{} ", this->nodata_value);
            } else {
               fmt::print("{: .3f} ", val);
            }
         }
         fmt::print("\n");
      }
   }

   void Raster::writeToFile(std::string path) const
   {
      std::ofstream outFile = std::ofstream(path);
      if (!outFile.is_open()) {
         spdlog::error("Cannot open output {}", path);
         exit(EXIT_FAILURE);
      }

      fmt::print(
         outFile,
         "ncols {}\n"
         "nrows {}\n"
         "xllcorner {:.4f}\n"
         "yllcorner {:.4f}\n"
         "cellsize {:.4f}\n"
         "NODATA_value {}\n",
         this->nCols, this->nRows, this->xllcorner, this->yllcorner, this->cellsize, this->nodata_value);

      for (int row = (this->nRows - 1); row >= 0; row--) {
         for (int col = 0; col < this->nCols; col++) {
            double val = this->operator()(row, col);

            if (val == this->nodata_value) {
               fmt::print(outFile, "{} ", this->nodata_value);
            } else {
               fmt::print(outFile, "{: .8f} ", val);
            }
         }
         fmt::print(outFile, "\n");
      }

      outFile.close();
   }

   KiLib::Vec3 Raster::randPoint()
   {
      KiLib::Vec3 pos(
         ((double)rand() / (double)RAND_MAX) * this->width, ((double)rand() / (double)RAND_MAX) * this->height, 0);

      pos.z = this->getInterpBilinear(pos);

      return pos;
   }
} // namespace KiLib
