#include <KiLib/Raster/Raster.hpp>
#include <spdlog/fmt/ostr.h>
#include <spdlog/spdlog.h>

namespace KiLib
{

   Raster::Raster()
   {
      this->constructed = false;
      this->nRows       = 0;
      this->nCols       = 0;
      this->nData       = 0;
   }

   // Load data in Raster format from specified path
   Raster::Raster(std::string path)
   {
      this->constructed = false;

      std::ifstream      rasterFile;
      std::string        line, key;
      std::istringstream stream(line);

      // Open Elevation file
      rasterFile.open(path, std::ios::in);
      if (!rasterFile.is_open())
      {
         printf("Could not open %s\n", path.c_str());
         exit(EXIT_FAILURE);
      }

      // Load header
      for (int i = 0; i < 6; i++)
      {
         getline(rasterFile, line);
         stream.str(line);
         stream.clear();

         stream >> key;

         // Force lower-case keys
         std::transform(key.begin(), key.end(), key.begin(), ::tolower);

         if (key == "ncols")
         {
            stream >> this->nCols;
         }
         else if (key == "nrows")
         {
            stream >> this->nRows;
         }
         else if (key == "xllcorner")
         {
            stream >> this->xllcorner;
         }
         else if (key == "yllcorner")
         {
            stream >> this->yllcorner;
         }
         else if (key == "cellsize")
         {
            stream >> this->cellsize;
         }
         else if (key == "nodata_value")
         {
            stream >> this->nodata_value;
         }
         else
         {
            std::cerr << "Unexpected value in header!" << std::endl;
            return;
         }
      }

      this->data.resize(nRows * nCols);
      // Load elevations
      for (size_t row = 0; row < this->nRows; row++)
      {
         getline(rasterFile, line);
         stream.str(line);
         stream.clear();

         // Moving along a row (across columns) is movement through X
         // Moving down a column (across rows) is movement through Y
         for (size_t col = 0; col < this->nCols; col++)
         {
            double value;
            stream >> value;
            this->at(this->nRows - row - 1, col) = value;
         }
      }

      this->width       = (this->nCols - 1) * this->cellsize;
      this->height      = (this->nRows - 1) * this->cellsize;
      this->nData       = this->nRows * this->nCols;
      this->constructed = true;

      rasterFile.close();
   }

   // Returns (bilinear) interpolated data value at specified position
   // Takes in a vec3 for convenience, ignores Z
   double Raster::getInterpBilinear(const Vec3 &pos) const
   {
      double r = std::clamp((pos.y - this->yllcorner) / this->cellsize, 0.0, (double)this->nRows - 1);
      double c = std::clamp((pos.x - this->xllcorner) / this->cellsize, 0.0, (double)this->nCols - 1);

      if (r == floor(r) && c == floor(c))
      {
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
      if (x2 != x1)
      {
         fy1 = ((x2 - x) / (x2 - x1)) * q11 + ((x - x1) / (x2 - x1)) * q21;
         fy2 = ((x2 - x) / (x2 - x1)) * q12 + ((x - x1) / (x2 - x1)) * q22;
      }

      double z = 0;
      if (y2 != y1)
      {
         z = ((y2 - y) / (y2 - y1)) * fy1 + ((y - y1) / (y2 - y1)) * fy2;
      }
      else
      {
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

      for (size_t row = 1; row <= this->nRows; row++)
      {
         for (size_t col = 0; col < this->nCols; col++)
         {
            double val = this->operator()(this->nRows - row, col);

            if (val == this->nodata_value)
            {
               fmt::print("{} ", this->nodata_value);
            }
            else
            {
               fmt::print("{: .3f} ", val);
            }
         }
         fmt::print("\n");
      }
   }

   void Raster::writeToFile(std::string path) const
   {
      std::ofstream outFile = std::ofstream(path);
      if (!outFile.is_open())
      {
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

      for (size_t row = 1; row <= this->nRows; row++)
      {
         for (size_t col = 0; col < this->nCols; col++)
         {
            double val = this->operator()(this->nRows - row, col);

            if (val == this->nodata_value)
            {
               fmt::print(outFile, "{} ", this->nodata_value);
            }
            else
            {
               fmt::print(outFile, "{: .8f} ", val);
            }
         }
         fmt::print(outFile, "\n");
      }

      outFile.close();
   }

   KiLib::Vec3 Raster::randPoint()
   {
      const double x = this->xllcorner + ((double)rand() / (double)RAND_MAX) * this->width;
      const double y = this->yllcorner + ((double)rand() / (double)RAND_MAX) * this->height;
      const double z = this->getInterpBilinear({x, y, 0});

      return KiLib::Vec3{x, y, z};
   }
} // namespace KiLib
