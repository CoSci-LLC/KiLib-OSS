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


#pragma once

#include <KiLib/Utils/Vec3.hpp>
#include <algorithm>
#include <cmath>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <map>
#include <random>
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
      size_t nData = 0; // Total number of datapoints

      bool constructed; // Flag indicating whether a file was loaded

      Raster(const std::string &path);
      Raster();

      // Creates a raster filled with zeros with same metadata as other.
      static KiLib::Raster zerosLike(const KiLib::Raster &other)
      {
         KiLib::Raster new_(other);
         std::fill(new_.data.begin(), new_.data.end(), 0.0);
         return new_;
      }

      // Creates a raster with same metadata as other, filled with fillValue.
      // If keepNoData is true, returned raster will have nodata in same locations as other.
      // Otherwise every value will be fillValue
      static KiLib::Raster fillLike(const KiLib::Raster &other, double fillValue, bool keepNoData)
      {
         KiLib::Raster new_(other);
         for (size_t i = 0; i < other.nData; i++)
         {
            if (new_(i) != new_.nodata_value)
            {
               new_(i) = fillValue;
            }
         }
         return new_;
      }

      // Creates a raster filled with nodatavalue with same metadata as other.
      static KiLib::Raster nodataLike(const KiLib::Raster &other)
      {
         KiLib::Raster new_(other);
         std::fill(new_.data.begin(), new_.data.end(), other.nodata_value);
         return new_;
      }

      void writeToFile(const std::string &path) const;

      /**
       * @brief Prints basic information about this Raster
       */
      void print();

      KiLib::Vec3 randPoint(std::mt19937_64 &gen);

      /**
       * Returns flat index to nearest cell in raster
       */
      size_t getNearestCell(const KiLib::Vec3 &pos);

      /**
       * Returns flat index to nearest cell in raster
       */
      size_t flattenIndex(size_t r, size_t c);

      KiLib::Vec3 getCellPos(size_t ind);

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

      /**
       * @brief Returns the flat index into the DEM. Doesn't do bounds checking.
       *
       * @param ind Flat index
       * @return double Value at position
       */
      double operator()(size_t ind) const
      {
         return this->data[ind];
      }

      /**
       * @brief Returns a REFERENCE to the flat index into the DEM. Doesn't do bounds checking.
       *
       * @param ind Flat index
       * @return double Value at position
       */
      double &operator()(size_t ind)
      {
         return this->data[ind];
      }

      /**
       * @brief Returns a REFERENCE to the flat index into the DEM. Does bounds checking.
       *
       * @param ind Flat index
       * @return double Value at position
       */
      double &at(size_t ind)
      {
         return this->data.at(ind);
      }

      /**
       * @brief Returns the flat index into the DEM. Does bounds checking.
       *
       * @param ind Flat index
       * @return double Value at position
       */
      double at(size_t ind) const
      {
         return this->data.at(ind);
      }

      // Slope methods
      enum SlopeMethod
      {
         ZevenbergenThorne,
      };

      KiLib::Raster        ComputeSlope(KiLib::Raster::SlopeMethod method) const;
      static KiLib::Raster ComputeSlopeZevenbergenThorne(const KiLib::Raster &inp);

      /**
       * @brief Takes in a vector of objects, and takes the mean of a given attribute at each cell position in a raster.
       * The attributes and corresponding positions are mapped to the nearest cell in the raster, and the mean is taken
       * over cells.
       *
       * @tparam T obj
       * @param ref Reference raster to determine shape, size, nodata, etc
       * @param objs vector of objects
       * @param posP Pointer to position attribute (i.e. &Landslide::pos)
       * @param attrP Pointer to attribute to rasterize (i.e. &Landslide::safetyFactor)
       * @param doPopulate function that dictates whether or not that instace will be rasterized
       * @return KiLib::Raster
       *
       */
      template <class T>
      static KiLib::Raster Rasterize(
         const KiLib::Raster &ref, const std::vector<T> &objs, KiLib::Vec3 T::*posP, double T::*attrP,
         std::function<bool(const T &)> doPopulate)
      {
         KiLib::Raster            outRast = KiLib::Raster::fillLike(ref, 0.0, true);
         std::map<size_t, double> counts;

         // Set up getters for the attributes
         auto getPos  = std::bind(posP, std::placeholders::_1);
         auto getAttr = std::bind(attrP, std::placeholders::_1);

         for (auto &obj : objs)
         {
            if (!doPopulate(obj))
            {
               continue;
            }

            KiLib::Vec3 pos  = getPos(obj);
            double      attr = getAttr(obj);

            size_t flatInd  = outRast.getNearestCell(pos);
            counts[flatInd] = counts.count(flatInd) == 0 ? 1.0 : counts[flatInd] + 1;

            outRast.at(flatInd) += attr;
         }

         for (auto [ind, count] : counts)
         {
            outRast.at(ind) /= count;
         }

         return outRast;
      }

   private:
      void fromDEM(const std::string path);
      void fromTiff(const std::string path);
      void toDEM(const std::string path) const;
      void toTiff(const std::string path) const;

      double getInterpBilinear(const Vec3 &pos) const;
   };

} // namespace KiLib
