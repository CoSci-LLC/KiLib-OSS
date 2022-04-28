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
#include <KiLib/Utils/Vec3.hpp>
#include <algorithm>
#include <optional>
#include <random>
#include <string>
#include <vector>

const double DOUBLE_INF = std::numeric_limits<double>::infinity();

// Eigen defaults to col-major, but we want row-major
// Typedef for a dynamic matrix of doubles stored in row-major order
typedef Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> RowMatrixXd;
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
      RowMatrixXd data;

      double xllcorner;    // Lower left corner x value in absolute coordinates
      double yllcorner;    // Lower left corner y value in absolute coordinates
      double cellsize;     // [m] Distance between values
      double width;        // [m] Width in X
      double height;       // [m] Height in Y
      double nodata_value; // Value associated with no data from DEM

      long int nCols = 0; // Number of columns (x)
      long int nRows = 0; // Number of rows (y)
      long int nData = 0; // Total number of datapoints

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
       * @brief Construct a new Raster New object
       *
       * @param nRows
       * @param nCols
       */
      RasterNew(Index nRows, Index nCols);

      ////////////////////////////////////////////////////////////////////////////////
      // Access
      ////////////////////////////////////////////////////////////////////////////////
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
       * @brief Returns a flat index for a given row and column
       *
       * @param row row index
       * @param col col index
       * @return Index flattened index of (row, col)
       */
      Index flattenIndex(Index row, Index col) const;

      /**
       * @brief Unflatten an index into a row and column
       *
       * @param ind flattened index
       * @return std::pair<Index, Index>
       */
      std::pair<Index, Index> GetRowCol(const Index ind) const;

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
      void writeToFile(const std::string &path) const;

      /**
       * @brief Load a DEM from disk that is in ASCII DEM format.
       *
       * @param path path to DEM
       */
      void fromDEM(const std::string &path);

      /**
       * @brief Write the raster to disk as an ASCII DEM.
       *
       * @param path path to write to
       */
      void toDEM(const std::string &path) const;

      /**
       * @brief Load a DEM from disk that is in GeoTIFF format.
       *
       * @param path path to DEM
       */
      void fromTiff(const std::string &path);

      /**
       * @brief Write the raster to disk as a GeoTIFF.
       *
       * @param path path to write to
       */
      void toTiff(const std::string &path) const;

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
       * @return size_t Number of data points
       */
      size_t GetNDataPoints() const;

      /**
       * @brief Get the number of points in the raster == nodata
       *
       * @return size_t Number of nodata points
       */
      size_t GetNNoDataPoints() const;

      /**
       * @brief Get the indices of valid points in the raster.
       *
       * @return std::vector<Index> vector of valid indices
       */
      std::vector<Index> GetValidIndices() const;
   };
} // namespace KiLib
