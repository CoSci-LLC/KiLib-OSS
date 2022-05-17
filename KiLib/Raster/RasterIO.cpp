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

#include <KiLib/Raster/Raster.hpp>
#include <filesystem>
#include <gdal/gdal_priv.h>
#include <gdal/gdal_utils.h>

#ifdef __APPLE__
namespace fs = std::__fs::filesystem;
#else
namespace fs = std::filesystem;
#endif

bool GDAL_REGISTERED = false;

// GT[0] x-coordinate of the upper-left corner of the upper-left pixel.
// GT[1] row pixel resolution / pixel width.
// GT[2] row rotation (typically zero).
// GT[3] y-coordinate of the upper-left corner of the upper-left pixel.
// GT[4] column rotation (typically zero).
// GT[5] col pixel resolution / pixel height (negative value for a north-up image).

namespace KiLib
{
   ////////////////////////////////////////////////////////////////////////////////
   // I/O
   ////////////////////////////////////////////////////////////////////////////////
   Raster::Raster(const std::string &path)
   {
      if (!GDAL_REGISTERED)
      {
         GDALAllRegister();
         GDAL_REGISTERED = true;
      }

      GDALDataset *poDataset = (GDALDataset *)GDALOpen(path.c_str(), GA_ReadOnly);
      if (poDataset == nullptr)
      {
         auto err = fmt::format("RasterIO failed | Could not open {} | {}", path, CPLGetLastErrorMsg());
         throw std::runtime_error(err);
      }

      GDALRasterBand *poBand;
      poBand = poDataset->GetRasterBand(1);
      if (poBand == NULL)
      {
         spdlog::error("Raster Reading failed | Could not get band 1 | {}", path, CPLGetLastErrorMsg());
      }

      double adfGeoTransform[6];
      poDataset->GetGeoTransform(adfGeoTransform);

      double nodata_value = poBand->GetNoDataValue();
      int    nXSize       = poBand->GetXSize();
      int    nYSize       = poBand->GetYSize();
      double cellsizex    = std::abs(adfGeoTransform[1]);
      double cellsizey    = std::abs(adfGeoTransform[5]);
      double xllcorner    = adfGeoTransform[0];
      double yllcorner    = adfGeoTransform[3] - nYSize * cellsizey;
      int    nData        = nXSize * nYSize;

      double *pafScanline = (double *)CPLMalloc(sizeof(double) * nData);
      auto    err = poBand->RasterIO(GF_Read, 0, 0, nXSize, nYSize, pafScanline, nXSize, nYSize, GDT_Float64, 0, 0);
      if (err != CE_None)
      {
         auto err = fmt::format("RasterIO failed | Could not read from raster band | {}", CPLGetLastErrorMsg());
         throw std::runtime_error(err);
      }

      // Create raster object
      this->nodata_value = nodata_value;
      this->xllcorner    = xllcorner;
      this->yllcorner    = yllcorner;
      this->cellsize     = cellsizex;

      this->Resize(nYSize, nXSize);

      // Copy data to raster object
      for (auto [r, c] : this->RowColIndexIter())
      {
         this->at(r, c) = pafScanline[this->FlattenIndex(r, c)];
      }

      this->data.colwise().reverseInPlace();

      // Free everything
      CPLFree(pafScanline);
      GDALClose(poDataset);
   }

   void Raster::WriteToFile(const std::string &path) const
   {
      // if path ends in .dem then use AAIGrid, if .tif, .tiff, .gtiff then use GTiff
      std::string outDriverName;
      if (fs::path(path).extension().string() == ".dem")
      {
         outDriverName = "AAIGrid";
      }
      else if (
         fs::path(path).extension().string() == ".tif" || fs::path(path).extension().string() == ".tiff" ||
         fs::path(path).extension().string() == ".gtiff")
      {
         outDriverName = "GTiff";
      }
      else
      {
         spdlog::error("Raster Writing failed | Invalid file extension | {}", path);
         spdlog::error("Please use .dem (AAIGrid) or .tif/.tiff/.gtiff (GTiff)");
         throw std::runtime_error("");
      }

      const char *pszFormat = "MEM";
      GDALDriver *poDriver;
      poDriver = GetGDALDriverManager()->GetDriverByName(pszFormat);
      if (poDriver == NULL)
         exit(1);

      GDALDataset *poDstDS;
      poDstDS = poDriver->Create("unused_name", this->nCols, this->nRows, 1, GDT_Float64, nullptr);

      // Set geotransform and projection
      double GT[6] = {
         this->xllcorner,                                // X ul corner
         std::abs(this->cellsize),                       // Cellsize x
         0,                                              // Rotation of rows
         this->yllcorner + this->nRows * this->cellsize, // Y ul corner
         0,                                              // Rotation of columns
         -std::abs(this->cellsize),                      // Cellsize y
      };

      OGRSpatialReference oSRS;
      char               *pszSRS_WKT = NULL;
      poDstDS->SetGeoTransform(GT);
      oSRS.SetUTM(11, TRUE);
      oSRS.SetWellKnownGeogCS("NAD27");
      oSRS.exportToWkt(&pszSRS_WKT);
      poDstDS->SetProjection(pszSRS_WKT);
      CPLFree(pszSRS_WKT);

      // Write raster. Flip it upside down
      Matrix          outData = this->data.colwise().reverse();
      GDALRasterBand *poBand;
      poBand = poDstDS->GetRasterBand(1);
      poBand->SetNoDataValue(this->nodata_value);
      auto err = poBand->RasterIO(
         GF_Write, 0, 0, this->nCols, this->nRows, outData.data(), this->nCols, this->nRows, GDT_Float64, 0, 0);
      if (err != CE_None)
      {
         auto err = fmt::format("RasterIO failed | Could not write to raster band | {}", CPLGetLastErrorMsg());
         throw std::runtime_error(err);
      }

      const char           *args[]    = {"-of", outDriverName.c_str(), NULL};
      GDALTranslateOptions *psOptions = GDALTranslateOptionsNew((char **)args, nullptr);
      if (psOptions == nullptr)
      {
         auto err = fmt::format("Could not create translate options | {}", CPLGetLastErrorMsg());
         throw std::runtime_error(err);
      }

      int  trans_err;
      auto out = GDALTranslate(path.c_str(), poDstDS, psOptions, &trans_err);
      if (trans_err != 0)
      {
         auto err = fmt::format("Failed to write raster!");
         throw std::runtime_error(err);
      }

      // Free memory
      GDALClose(poDstDS);
      GDALClose(out);
      GDALTranslateOptionsFree(psOptions);

      // delete .prj file as we dont need it
      auto prj_path = fs::path(path).replace_extension(".prj");
      std::remove(prj_path.c_str());
   }
} // namespace KiLib
