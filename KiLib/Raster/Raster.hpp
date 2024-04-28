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
#include <optional>
#include <random>
#include <string>
#include <vector>

namespace KiLib
{

   /**
    * @brief Loads in Rasters (like DEMs) and provides nice helper functions such
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

      Raster(const std::string &path);
      Raster();

      // Creates a raster with same metadata as other, filled with fillValue.
      // If keepNoData is true, returned raster will have nodata in same locations as other.
      // Otherwise every value will be fillValue
      static KiLib::Raster fillLike(const KiLib::Raster &other, double fillValue, bool keepNoData);

      void writeToFile(const std::string &path) const;

      /**
       * @brief Prints basic information about this Raster
       */
      void print() const;

      KiLib::Vec3 randPoint(std::mt19937_64 &gen) const;

      /**
       * Returns flat index to nearest cell in raster
       */
      size_t getNearestCell(const KiLib::Vec3 &pos) const;

      /**
       * Returns flat index to nearest cell in raster
       */
      size_t flattenIndex(size_t r, size_t c) const;

      KiLib::Vec3 getCellPos(size_t ind) const;
      KiLib::Vec3 getCellCenter(size_t ind) const;

      /**
       * @brief Returns distance between pos and nearest boundary point
       *
       * @param pos Position to get distance from
       * @return double Distance
       */
      double distFromBoundary(const Vec3 &pos) const;

      /**
       * @brief Returns row and col of raster flat index
       *
       * @param ind Flatten index
       * @return pair row, col
       */
      std::pair<int, int> GetRowCol(const size_t ind) const;

      /**
       * @brief Interpolates raster value at pos (takes in 3D vector but ignores Z)
       *
       * @param pos Pos to interpolate
       * @return double Raster value, using bilinear interpolation
       */
      double operator()(const Vec3 &pos) const;

      /**
       * @brief Returns a REFERENCE to the (row, col) index into the Raster
       *        Does bounds checking.
       *
       * @param row Y value
       * @param col X value
       * @return double& Reference to raster value at position
       */
      double &at(size_t row, size_t col);

      /**
       * @brief Returns the (row, col) index into the DEM. Does bounds checking
       *
       * @param row Y value
       * @param col X value
       * @return double Value at position
       */
      double at(size_t row, size_t col) const;

      /**
       * @brief Returns a REFERENCE to the (row, col) index into the Raster
       *        Doesn't do bounds checking.
       *
       * @param row Y value
       * @param col X value
       * @return double& Reference to raster value at position
       */
      double &operator()(size_t row, size_t col);

      /**
       * @brief Returns the (row, col) index into the DEM. Doesn't do bounds checking.
       *
       * @param row Y value
       * @param col X value
       * @return double Value at position
       */
      double operator()(size_t row, size_t col) const;

      /**
       * @brief Returns the flat index into the DEM. Doesn't do bounds checking.
       *
       * @param ind Flat index
       * @return double Value at position
       */
      double operator()(size_t ind) const;

      /**
       * @brief Returns a REFERENCE to the flat index into the DEM. Doesn't do bounds checking.
       *
       * @param ind Flat index
       * @return double Value at position
       */
      double &operator()(size_t ind);

      /**
       * @brief Returns a REFERENCE to the flat index into the DEM. Does bounds checking.
       *
       * @param ind Flat index
       * @return double Value at position
       */
      double &at(size_t ind);

      /**
       * @brief Returns the flat index into the DEM. Does bounds checking.
       *
       * @param ind Flat index
       * @return double Value at position
       */
      double at(size_t ind) const;

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
       * @param width Number of cells to average over. 0 is just cell at i,j; 1 is 9x9 region around i,j; and so on.
       * @return KiLib::Raster
       *
       */
      template <class T>
      static KiLib::Raster Rasterize(
         const KiLib::Raster &ref, const std::vector<T> &objs, std::function<KiLib::Vec3(T)> getPos,
         std::function<double(T)> getAttr, int width = 0)
      {
         KiLib::Raster sumRast = KiLib::Raster::fillLike(ref, 0.0, true);
         KiLib::Raster outRast = KiLib::Raster::fillLike(ref, 0.0, true);

         std::unordered_map<size_t, double> counts;

         for (auto &obj : objs)
         {
            KiLib::Vec3 pos  = getPos(obj);
            double      attr = getAttr(obj);

            size_t flatInd  = sumRast.getNearestCell(pos);
            counts[flatInd] = counts.count(flatInd) == 0 ? 1.0 : counts[flatInd] + 1;

            sumRast(flatInd) += attr;
         }

         for (int r = 0; r < (int)sumRast.nRows; r++)
         {
            for (int c = 0; c < (int)sumRast.nCols; c++)
            {
               // Skip nodata
               if (sumRast.at((size_t)r,(size_t) c) == sumRast.nodata_value)
               {
                  continue;
               }
               double count = 0.0;
               double sum   = 0.0;
               for (int ri = r - width; ri <= (r + width); ri++)
               {
                  for (int ci = c - width; ci <= (c + width); ci++)
                  {
                     // Dont sum out of bounds, cant sum where we have no points
                     if (
                        counts.count(sumRast.flattenIndex((size_t)ri, (size_t)ci)) == 0 or ri < 0 or ri >= (int)sumRast.nRows or
                        ci < 0 or ci >= (int)sumRast.nCols)
                     {
                        continue;
                     }

                     count += counts[sumRast.flattenIndex((size_t)ri, (size_t) ci)];
                     sum += sumRast.at((size_t)ri, (size_t)ci);
                  }
               }
               if (count != 0)
               {
                  outRast.at((size_t)r,(size_t) c) = sum / count;
               }
            }
         }

         return outRast;
      }

      double                     GetAverage(size_t ind, double radius) const;
      static std::vector<size_t> getValidIndices(const std::vector<const KiLib::Raster *> &rasts);
      static void                assertAgreeDim(const std::vector<const KiLib::Raster *> &rasts);
      std::optional<KiLib::Vec3> GetCoordMinDistance(size_t ind, const KiLib::Vec3 &inPos, const KiLib::Raster &elev, double radius, double threshold) const;
      //GetCoordMinDistance(size_t ind, double zInd, const KiLib::Raster &elev, double radius, double threshold) const;
      std::optional<KiLib::Vec3> FindClosestStreamCell(size_t ind, double zInd, const KiLib::Raster &elev, double radius, double threshold) const;

   private:
      void fromDEM(const std::string &path);
      void fromTiff(const std::string &path);
      void toDEM(const std::string &path) const;
      void toTiff(const std::string &path) const;

      double getInterpBilinear(const Vec3 &pos) const;
   };

} // namespace KiLib
