#include <KiLib/Raster/Raster.hpp>
#include <libtiff/tiffio.hxx>
#include <spdlog/fmt/ostr.h>
#include <spdlog/spdlog.h>

#define GEOTIFFTAG_MODELPIXELSCALE 33550
#define GEOTIFFTAG_MODELTIEPOINT 33922
#define GEOTIFFTAG_NODATAVALUE 42113

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
      for (size_t row = 0; row < this->nRows; row++) {
         getline(rasterFile, line);
         stream.str(line);
         stream.clear();

         // Moving along a row (across columns) is movement through X
         // Moving down a column (across rows) is movement through Y
         for (size_t col = 0; col < this->nCols; col++) {
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

   KiLib::Raster Raster::fromTiff(const std::string path)
   {
      TIFFSetWarningHandler(0);

      TIFF *tiff = TIFFOpen(path.c_str(), "r");

      if (tiff == NULL) {
         spdlog::error("Failed to open {}", path);
         exit(EXIT_FAILURE);
      }

      size_t free_flag = 0;
      uint count       = 0;
      double *scaling;
      double *tiepoint;
      char *nodat;

      size_t num_dir = 1;

      while (TIFFReadDirectory(tiff))
         num_dir++;

      // The file is can to only contain one directory for now as multiple directories would correspond to multiple
      // rasters
      if (num_dir > 1) {
         spdlog::error("Multi-raster images are not supported as of yet.");
         exit(EXIT_FAILURE);
      }

      KiLib::Raster r;

      // Retrieve the width and height of the image. Fail if either can't be retrieved.
      if (!TIFFGetField(tiff, TIFFTAG_IMAGEWIDTH, &r.nCols) || !TIFFGetField(tiff, TIFFTAG_IMAGELENGTH, &r.nRows)) {
         spdlog::error("Failed to read image width or image height.");
         exit(EXIT_FAILURE);
      }

      if (!TIFFGetField(tiff, GEOTIFFTAG_MODELPIXELSCALE, &count, &scaling)) {
         spdlog::critical("Failed to find pixel scaling. Assuming 1:1");
         scaling = new double[2]{1, 1};
         free_flag |= 1;
      }

      if (!TIFFGetField(tiff, GEOTIFFTAG_MODELTIEPOINT, &count, &tiepoint)) {
         spdlog::critical("Failed to find model tiepoint. Assuming 0, 0");
         tiepoint = new double[6]{0};
         free_flag |= 2;
      }

      if (!TIFFGetField(tiff, GEOTIFFTAG_NODATAVALUE, &count, &nodat)) {
         spdlog::critical("Failed to find nodata value. Assuming -9999");
         nodat = new char[7]{'-', '9', '9', '9', '9', '\n'};
         free_flag |= 4;
      }

      r.width        = (r.nCols - 1) * scaling[0];
      r.height       = (r.nRows - 1) * scaling[1];
      r.xllcorner    = tiepoint[3];
      r.yllcorner    = tiepoint[4] - (r.nRows * scaling[1]);
      r.cellsize     = scaling[0];
      r.nodata_value = std::stod(nodat);

      r.data.reserve(r.nCols * r.nRows);

      if (TIFFIsTiled(tiff)) {
         spdlog::error("There is no support for tiled images yet.");
         exit(EXIT_FAILURE);
      } else {
         // Bits per sample
         size_t bps = 1;

         // Rows per strip
         size_t rps = 1;

         // Format is currently undefined: https://www.awaresystems.be/imaging/tiff/tifftags/sampleformat.html
         int16 format = 4;

         // The number of bytes a strip occupies
         uint64 ss = TIFFStripSize64(tiff);

         TIFFGetField(tiff, TIFFTAG_ROWSPERSTRIP, &rps);
         TIFFGetField(tiff, TIFFTAG_SAMPLEFORMAT, &format);
         TIFFGetField(tiff, TIFFTAG_BITSPERSAMPLE, &bps);

         // Strips are used to avoid having to deal with decompression of data.
         tdata_t buf = _TIFFmalloc(ss);
         for (tstrip_t strip = 0; strip < TIFFNumberOfStrips(tiff); strip += rps) {
            TIFFReadEncodedStrip(tiff, strip, buf, (tsize_t)-1);

            // There may be issues with in-memory alignment with this approach
            for (size_t i = 0; i < ss; i += ss / (rps * r.nCols)) {
               switch (format) {
               case 1:
                  if (bps == 8)
                     r.data.emplace_back(((uint8 *)buf)[i]);
                  else if (bps == 16)
                     r.data.emplace_back(((uint16 *)buf)[i]);
                  else if (bps == 32)
                     r.data.emplace_back(((uint32 *)buf)[i]);
                  else
                     r.data.emplace_back(((uint64 *)buf)[i]);
                  break;
               case 2:
                  if (bps == 8)
                     r.data.emplace_back(((int8 *)buf)[i]);
                  else if (bps == 16)
                     r.data.emplace_back(((int16 *)buf)[i]);
                  else if (bps == 32)
                     r.data.emplace_back(((int32 *)buf)[i]);
                  else
                     r.data.emplace_back(((int64 *)buf)[i]);
                  break;
               case 3:
                  if (bps == 32)
                     r.data.emplace_back(((float *)buf)[i]);
                  else
                     r.data.emplace_back(((double *)buf)[i]);
                  break;
               default:
                  spdlog::error("Unknown data format.");
                  exit(EXIT_FAILURE);
               }
            }
         }
         _TIFFfree(buf);
      }

      // Remember to free necessary variables
      if (free_flag & 1)
         delete scaling;

      // Remember to free necessary variables
      if (free_flag & 2)
         delete tiepoint;

      // Remember to free necessary variables
      if (free_flag & 4)
         delete nodat;

      TIFFClose(tiff);

      return r;
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

      for (size_t row = 1; row <= this->nRows; row++) {
         for (size_t col = 0; col < this->nCols; col++) {
            double val = this->operator()(this->nRows - row, col);

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

      for (size_t row = 1; row <= this->nRows; row++) {
         for (size_t col = 0; col < this->nCols; col++) {
            double val = this->operator()(this->nRows - row, col);

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
