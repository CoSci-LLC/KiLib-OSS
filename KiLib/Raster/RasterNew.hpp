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

#include <Eigen/Eigen>
#include <KiLib/Raster/RasterNewIterators.hpp>
#include <KiLib/Utils/Vec3.hpp>
#include <optional>
#include <random>
#include <string>

const double DOUBLE_INF = std::numeric_limits<double>::infinity();

// Eigen defaults to col-major, but we want row-major
// Typedef for a dynamic matrix of doubles stored in row-major order
typedef Eigen::Array<double, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> Matrix;
using Eigen::Index;

namespace KiLib
{
   const double RASTER_DEFAULT_NODATA_VALUE = -9999;

   /**
    * @brief Loads in RasterNews (like DEMs) and provides nice helper functions such
    * as interpolation, matrix-like access, and so on.
    *
    */
   class RasterNew
   {
      ////////////////////////////////////////////////////////////////////////////////
      // Data memebers
      ////////////////////////////////////////////////////////////////////////////////
   public:
      Matrix data;

      double xllcorner; // Lower left corner x value in absolute coordinates
      double yllcorner; // Lower left corner y value in absolute coordinates
      double cellsize;  // [m] Distance between values
      double width;     // [m] Width in X
      double height;    // [m] Height in Y

      double nodata_value = RASTER_DEFAULT_NODATA_VALUE; // Value associated with no data from DEM

      Index nCols = 0; // Number of columns (x)
      Index nRows = 0; // Number of rows (y)
      Index nData = 0; // Total number of datapoints

      ////////////////////////////////////////////////////////////////////////////////
      // Constructors
      ////////////////////////////////////////////////////////////////////////////////
      /**
       * @brief Construct a RasterNew object,
       * nRows and nCols are set to 0 with a nodata_value of -9999
       *
       */
      RasterNew();

      /**
       * @brief Construct a new RasterNew object from a raster on disk. Format is determined by extension.
       * Accepted formats are ASCII DEMs (.asc, .ddem), and GeoTIFF (.tif, .tiff).
       *
       * @param path
       */
      RasterNew(const std::string &path);

      /**
       * @brief Construct a RasterNew with the provided number of rows and cols. Data initialized to 0
       *
       * @param nRows
       * @param nCols
       */
      RasterNew(Index nRows, Index nCols);

      /**
       * @brief creates a RasterNew object with the same metadata and size as other, filled with fillValue.
       * If keepNoData is true, returned raster will have nodata in same locations as other.
       *
       * @param other Other raster to get metadata from
       * @param fillValue Value to fill with
       * @param keepNoData Whether to keep nodata in the same locations as other
       * @return RasterNew
       */
      static RasterNew FillLike(const RasterNew &other, double fillValue, bool keepNoData);

      ////////////////////////////////////////////////////////////////////////////////
      // Access
      ////////////////////////////////////////////////////////////////////////////////

      /**
       * @brief Iterates over row and column indices.
       * Row major, so (0, 0)->(0, 1)->...->(1,0)->(1,1)->...
       * Supports OpenMP
       *
       * @code
       *    for (auto [rowIndex, colIndex] : dem.RowColIndexIter()) {
       *      ...
       *    }
       * @endcode
       *
       * @return RowColIter
       */
      RowColIter RowColIndexIter() const;

      /**
       * @brief Iterate over a subview of the raster. This will *not* return out of bounds indices.
       *
       * @code
       *    for (auto [rowIndex, colIndex] : dem.RowColSubViewIndexIter(6, 6, 10, 10)) {
       *      ...
       *    }
       * @endcode
       *
       * @param centerRow Index of the center row in subview
       * @param centerCol index of the center column in subview
       * @param nRow Number of rows *around* center. ie 1 means 1 row above and below center
       * @param nCol Number of columns *around* center. ie 1 means 1 column to the left and right of center
       * @return RowColIter
       */
      RowColIter RowColSubViewIndexIter(Index centerRow, Index centerCol, Index nRow, Index nCol) const;

      /**
       * @brief Return a reference to element at (row, col). Boundary checks are performed.
       *
       * @param row row index
       * @param col col index
       * @return double& element
       */
      double &at(Index row, Index col);

      /**
       * @brief Return the value at (row, col). Boundary checks are performed.
       *
       * @param row row index
       * @param col col index
       * @return double& element
       */
      double at(Index row, Index col) const;

      /**
       * @brief Returns a reference to the element at flatIndex. Boundary checks are performed.
       *
       * @param flatIndex flat index of cell
       * @return double& element
       */
      double &at(Index flatIndex);

      /**
       * @brief Returns the value at flatIndex. Boundary checks are performed.
       *
       * @param row row index
       * @param col col index
       * @return double& element
       */
      double at(Index flatIndex) const;

      /**
       * @brief Return a reference to element at (row, col). Boundary checks are performed.
       *
       * @param row row index
       * @param col col index
       * @return double& element
       */
      double &operator()(Index row, Index col);

      /**
       * @brief Return the value at (row, col). Boundary checks are performed.
       *
       * @param row row index
       * @param col col index
       * @return double& element
       */
      double operator()(Index row, Index col) const;


      /**
       * @brief Returns a reference to the element at flatIndex
       *
       * @param flatIndex flat index of cell
       * @return double& element
       */
      double &operator()(Index flatIndex);

      /**
       * @brief Returns the value at flatIndex
       *
       * @param flatIndex flat index of cell
       * @return double element
       */
      double operator()(Index flatIndex) const;

      /**
       * @brief Interpolate the value at a given point.
       *
       * @param row row index
       * @param col col index
       * @return double& element
       */
      double operator()(Vec3 pos) const;

      /**
       * @brief Returns a flat index for a given row and column. This will bounds check row and col
       * If they are OOB, throws std::out_of_range
       *
       * @param row row index
       * @param col col index
       * @return Index flattened index of (row, col)
       */
      Index FlattenIndex(Index row, Index col) const;

      /**
       * @brief Unflatten an index into a row and column
       *
       * @param ind flattened index
       * @return std::pair<Index, Index>
       */
      std::pair<Index, Index> GetRowCol(Index ind) const;

      /**
       * @brief Generates a random point within the raster
       *
       * @param gen random number generator
       * @return KiLib::Vec3 random point in the raster
       */
      KiLib::Vec3 RandPoint(std::mt19937_64 &gen) const;

      /**
       * @brief Returns flat index to the nearest cell to pos.
       *
       * IMPORTANT NOTE: This *floors*, so it will return the nearest cell TOWARDS
       * THE LOWER LEFT CORNER.
       *
       * @param pos Coordinate in raster
       * @return Index flat index of nearest cell
       */
      Index GetNearestCell(KiLib::Vec3 pos) const;

      /**
       * @brief Get position of a cell in the raster in 3D space
       *
       * @param ind flat index of cell
       * @return KiLib::Vec3 position of cell
       */
      KiLib::Vec3 GetCellPos(Index ind) const;

      /**
       * @brief Get center of a cell in the raster in 3D space
       *
       * @param ind flat index of cell
       * @return KiLib::Vec3
       */
      KiLib::Vec3 GetCellCenter(Index ind) const;

      /**
       * @brief Resize the raster.
       * Will set nRows, nCols, nData, width, and height to the proper values.
       *
       * @param nRows new number of rows
       * @param nCols new number of columns
       */
      void Resize(Index nRows, Index nCols);

      ////////////////////////////////////////////////////////////////////////////////
      // I/O
      ////////////////////////////////////////////////////////////////////////////////
      /**
       * @brief Write a raster to disk. Format is determined by extension.
       * Accepted formats are ASCII DEMs (.asc, .dem), and GeoTIFF (.tif, .tiff).
       *
       * @param path path to write to
       */
      void WriteToFile(const std::string &path) const;

      /**
       * @brief Load a DEM from disk that is in ASCII DEM format.
       *
       * @param path path to DEM
       */
      void FromDEM(const std::string &path);

      /**
       * @brief Write the raster to disk as an ASCII DEM.
       *
       * @param path path to write to
       */
      void ToDEM(const std::string &path) const;

      /**
       * @brief Load a DEM from disk that is in GeoTIFF format.
       *
       * @param path path to DEM
       */
      void FromTiff(const std::string &path);

      /**
       * @brief Write the raster to disk as a GeoTIFF.
       *
       * @param path path to write to
       */
      void ToTiff(const std::string &path) const;

      ////////////////////////////////////////////////////////////////////////////////
      // Stats
      ////////////////////////////////////////////////////////////////////////////////
      /**
       * @brief Get the minimum value in the raster, excludes nodata points.
       *
       * @return double min
       */
      double GetMinValue() const;

      /**
       * @brief Get the maximum value in the raster, excludes nodata points.
       *
       * @return double max
       */
      double GetMaxValue() const;

      /**
       * @brief Get the mean value in the raster, excludes nodata points.
       *
       * @return double mean
       */
      double GetMeanValue() const;

      /**
       * @brief Return the number of points in the raster != nodata
       *
       * @return Index Number of data points
       */
      Index GetNDataPoints() const;

      /**
       * @brief Get the number of points in the raster == nodata
       *
       * @return Index Number of nodata points
       */
      Index GetNNoDataPoints() const;

      /**
       * @brief Get the indices of valid points in the raster.
       *
       * @return std::vector<Index> vector of valid indices
       */
      std::vector<Index> GetValidIndices() const;

      ////////////////////////////////////////////////////////////////////////////////
      // Science
      ////////////////////////////////////////////////////////////////////////////////
      // Slope methods
      enum SlopeMethod
      {
         ZevenbergenThorne,
      };

      KiLib::RasterNew        ComputeSlope(KiLib::RasterNew::SlopeMethod method) const;
      static KiLib::RasterNew ComputeSlopeZevenbergenThorne(const KiLib::RasterNew &inp);

      /**
       * @brief Searches for the nearest cell, pos, to provided cell, ind, that satisfies the following constraints:
       * pos is within radius meters from ind
       * z value at pos is higher than threshold
       * elevation at pos in elev is lower than the elevation at ind
       *
       * @param ind index to search around
       * @param zInd Threshold (max) for elevation
       * @param elev Elevation raster
       * @param radius raidus to search around ind (meters)
       * @param threshold z value returned must be higher than this value
       * @return std::optional<KiLib::Vec3>
       */
      std::optional<KiLib::Vec3>
      GetCoordMinDistance(Index ind, double zInd, const KiLib::RasterNew &elev, double radius, double threshold) const;

      /**
       * @brief Bilinearly interpolate a z value from the raster.
       *
       * @param pos position to interpolate from
       * @return double z
       */
      double GetInterpBilinear(Vec3 pos) const;

      /**
       * @brief Returns a points distance from the nearest boundary
       *
       * @param pos Position in raster
       * @return double distance
       */
      double DistFromBoundary(const Vec3 pos) const;

      /**
       * @brief Get average around a flat index into a raster. Skips nodata values
       *
       * @param ind index to get average around
       * @param radius circular radius
       * @return double average
       */
      double GetAverage(Index ind, double radius) const;

      /**
       * @brief Takes in a vector of objects, and takes the mean of a given attribute at each cell position in a raster.
       * The attributes and corresponding positions are mapped to the nearest cell in the raster, and the mean is taken
       * over cells.
       *
       * @tparam T Object that we are rasterizing
       * @param ref Reference raster to determine shape, size, nodata, etc
       * @param objs Vector of objects of type T
       * @param getPos Function to return position of object
       * @param getAttr Function that returns attribute to rasterize
       * @param width Number of cells to average over. 0 is just cell at i,j; 1 is 3x3 region around i,j; and so on.
       * @return KiLib::Raster
       *
       */
      template <class T>
      static RasterNew Rasterize(
         const RasterNew &ref, const std::vector<T> &objs, std::function<KiLib::Vec3(T)> getPos,
         std::function<double(T)> getAttr, int width = 0)
      {
         RasterNew sumRast = RasterNew::FillLike(ref, 0.0, true);
         RasterNew outRast = RasterNew::FillLike(ref, 0.0, true);

         std::unordered_map<Index, double> counts;

         for (auto &obj : objs)
         {
            KiLib::Vec3 pos  = getPos(obj);
            double      attr = getAttr(obj);

            Index flatInd   = sumRast.GetNearestCell(pos);
            counts[flatInd] = counts.count(flatInd) == 0 ? 1.0 : counts[flatInd] + 1;

            sumRast(flatInd) += attr;
         }

         for (auto [r, c] : sumRast.RowColIndexIter())
         {
            // Skip nodata points in the raster we're summing into
            if (sumRast.at(r, c) == sumRast.nodata_value)
            {
               continue;
            }

            double count = 0.0;
            double sum   = 0.0;
            for (auto [ri, ci] : sumRast.RowColSubViewIndexIter(r, c, width, width))
            {
               // Cant sum where we have no points
               if (counts.count(sumRast.FlattenIndex(ri, ci)) == 0)
               {
                  continue;
               }

               count += counts[sumRast.FlattenIndex(ri, ci)];
               sum += sumRast.at(ri, ci);
            }

            if (count != 0)
            {
               outRast.at(r, c) = sum / count;
            }
         }

         return outRast;
      }
      ////////////////////////////////////////////////////////////////////////////////
      // Utils
      ////////////////////////////////////////////////////////////////////////////////
      static void AssertAgreeDim(const std::vector<const KiLib::RasterNew *> &rasts);
   };

} // namespace KiLib
