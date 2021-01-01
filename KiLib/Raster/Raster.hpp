#pragma once

#include <KiLib/Utils/Vec3.hpp>
#include <algorithm>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

namespace KiLib
{

   /**
    * @brief Loads in Rasters (like DEMs) and provides nice helper funcitons such
    * as interpolation, matrix-like access, and so on.
    *
    */
   class Raster
   {
   public:
      std::vector<double> data;

      double xllcorner;    // Lower left corner x value in absolute coordinates
      double yllcorner;    // Lower left corner y value in absolute coordinates
      double cellsize;     // [m] Distance between values
      double width;        // [m] Width in X
      double height;       // [m] Height in Y
      double nodata_value; // Value associated with no data from DEM

      size_t nCols = 0; // Number of columns (x)
      size_t nRows = 0; // Number of rows (y)

      bool constructed; // Flag indicating whether a file was loaded

      Raster(std::string path);
      Raster();

      // Creates a raster filled with zeros with same metadata as other.
      static KiLib::Raster zerosLike(const KiLib::Raster &other)
      {
         KiLib::Raster new_(other);
         std::fill(new_.data.begin(), new_.data.end(), 0.0);
         return new_;
      }

      void writeToFile(std::string path) const;

      /**
       * @brief Prints basic information about this Raster
       */
      void print();

      KiLib::Vec3 randPoint();

      /**
       * @brief Returns distance between pos and nearest boundary point
       *
       * @param pos Position to get distance from
       * @return double Distance
       */
      double distFromBoundary(const Vec3 &pos)
      {
         const double left   = pos.x - this->xllcorner;
         const double right  = this->xllcorner + this->width - pos.x;
         const double top    = this->yllcorner + this->height - pos.y;
         const double bottom = pos.y - this->yllcorner;

         return std::min(std::min(left, right), std::min(top, bottom));
      }

      /**
       * @brief Interpolates raster value at pos (takes in 3D vector but ignores Z)
       *
       * @param pos Pos to interpolate
       * @return double Raster value, using bilinear interpolation
       */
      double operator()(const Vec3 &pos) const
      {
         return this->getInterpBilinear(pos);
      }

      /**
       * @brief Returns a REFERENCE to the (row, col) index into the Raster
       *        Does bounds checking.
       *
       * @param row Y value
       * @param col X value
       * @return double& Reference to raster value at position
       */
      double &at(size_t row, size_t col)
      {
         return this->data.at(row * this->nCols + col);
      }

      /**
       * @brief Returns the (row, col) index into the DEM. Does bounds checking
       *
       * @param row Y value
       * @param col X value
       * @return double Value at position
       */
      double at(size_t row, size_t col) const
      {
         return this->data.at(row * this->nCols + col);
      }

      /**
       * @brief Returns a REFERENCE to the (row, col) index into the Raster
       *        Doesn't do bounds checking.
       *
       * @param row Y value
       * @param col X value
       * @return double& Reference to raster value at position
       */
      double &operator()(size_t row, size_t col)
      {
         return this->data[row * this->nCols + col];
      }

      /**
       * @brief Returns the (row, col) index into the DEM. Doesn't do bounds checking.
       *
       * @param row Y value
       * @param col X value
       * @return double Value at position
       */
      double operator()(size_t row, size_t col) const
      {
         return this->data[row * this->nCols + col];
      }

   private:
      void fromDEM(const std::string path);
      void fromTiff(const std::string path);

      double getInterpBilinear(const Vec3 &pos) const;
   };

} // namespace KiLib
