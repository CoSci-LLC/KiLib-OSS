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
#include <libtiff/tiffio.hxx>
#ifdef _OPENMP
#include <omp.h>
#endif
#include <spdlog/fmt/ostr.h>
#include <spdlog/spdlog.h>

#define GEOTIFFTAG_KEYDIRECTORY 34735
#define GEOTIFFTAG_DOUBLEPARAMS 34736
#define GEOTIFFTAG_ASCIIPARAMS 34737
#define GEOTIFFTAG_MODELPIXELSCALE 33550
#define GEOTIFFTAG_MODELTIEPOINT 33922
#define GEOTIFFTAG_NODATAVALUE 42113

// The following code was retrieved from: http://www.simplesystems.org/libtiff/addingtags.html
#define N(a) (sizeof(a) / sizeof(a[0]))
static TIFFExtendProc _ParentExtender = NULL;

/**
 * @brief Defines the GeoTiff tags that are needed to read and write tiff files.
 *
 * @param tif the tiff file
 */
static void _XTIFFDefaultDirectory(TIFF *tif)
{

   // Definitions for GeoTiff fields needed to parse them correctly
   static const TIFFFieldInfo xtiffFieldInfo[] = {
      {GEOTIFFTAG_KEYDIRECTORY, -1, -1, TIFF_SHORT, FIELD_CUSTOM, true, true, (char *)"GeoKeyDirectory"},
      {GEOTIFFTAG_DOUBLEPARAMS, -1, -1, TIFF_DOUBLE, FIELD_CUSTOM, true, true, (char *)"GeoDoubleParams"},
      {GEOTIFFTAG_ASCIIPARAMS, -1, -1, TIFF_ASCII, FIELD_CUSTOM, true, false, (char *)"GeoASCIIParams"},
      {GEOTIFFTAG_MODELPIXELSCALE, -1, -1, TIFF_DOUBLE, FIELD_CUSTOM, false, true, (char *)"GeoPixelScale"},
      {GEOTIFFTAG_MODELTIEPOINT, -1, -1, TIFF_DOUBLE, FIELD_CUSTOM, false, true, (char *)"GeoTiePoints"},
      {GEOTIFFTAG_NODATAVALUE, -1, -1, TIFF_ASCII, FIELD_CUSTOM, true, false, (char *)"GeoNoDataValue"},
   };

   /* Install the extended Tag field info */
   TIFFMergeFieldInfo(tif, xtiffFieldInfo, N(xtiffFieldInfo));

   /* Since an XTIFF client module may have overridden
    * the default directory method, we call it now to
    * allow it to set up the rest of its own methods.
    */

   if (_ParentExtender)
      (*_ParentExtender)(tif);
}

/**
 * @brief Extends the tags that are defined by Libtiff while respecting other extensions which may be defined.
 *
 */
static void _XTIFFInitialize(void)
{
   static bool b = false;

   if (b)
      return;

   b = true;

   /* Grab the inherited method and install */
   _ParentExtender = TIFFSetTagExtender(_XTIFFDefaultDirectory);
}

namespace KiLib
{

   void Raster::fromTiff(const std::string path)
   {
      _XTIFFInitialize();

      TIFF *tiff = TIFFOpen(path.c_str(), "r");

      if (tiff == NULL)
      {
         spdlog::error("Failed to open {} for reading", path);
         exit(EXIT_FAILURE);
      }

      size_t  free_flag = 0;
      uint16  count     = 0;
      uint32  w         = 0;
      uint32  h         = 0;
      double *scaling;
      double *tiepoint;
      char *  nodat;

      size_t num_dir = 1;

      while (TIFFReadDirectory(tiff))
         num_dir++;

      // The file is can to only contain one directory for now as multiple directories would correspond to multiple
      // rasters
      if (num_dir > 1)
      {
         spdlog::error("Multi-raster images are not supported as of yet.");
         exit(EXIT_FAILURE);
      }

      // Retrieve the width and height of the image. Fail if either can't be retrieved.
      if (!(TIFFGetField(tiff, TIFFTAG_IMAGEWIDTH, &w) && TIFFGetField(tiff, TIFFTAG_IMAGELENGTH, &h)))
      {
         spdlog::error("Failed to read image width or image height.");
         exit(EXIT_FAILURE);
      }

      if (!TIFFGetField(tiff, GEOTIFFTAG_MODELPIXELSCALE, &count, &scaling))
      {
         spdlog::critical("Failed to find pixel scaling. Assuming 1:1");
         scaling = new double[2]{1, 1};
         free_flag |= 1;
      }

      if (!TIFFGetField(tiff, GEOTIFFTAG_MODELTIEPOINT, &count, &tiepoint))
      {
         spdlog::critical("Failed to find model tiepoint. Assuming 0, 0");
         tiepoint = new double[6]{0};
         free_flag |= 2;
      }

      if (!TIFFGetField(tiff, GEOTIFFTAG_NODATAVALUE, &nodat))
      {
         spdlog::critical("Failed to find nodata value. Assuming -9999");
         nodat = new char[7]{'-', '9', '9', '9', '9', '\n'};
         free_flag |= 4;
      }

      this->nCols        = w;
      this->nRows        = h;
      this->width        = (this->nCols - 1) * scaling[0];
      this->height       = (this->nRows - 1) * scaling[1];
      this->xllcorner    = tiepoint[3];
      this->yllcorner    = tiepoint[4] - (this->nRows * scaling[1]);
      this->cellsize     = scaling[0];
      this->nodata_value = std::stod(nodat);

      this->nData = this->nRows * this->nCols;
      this->data.resize(this->nData);

      if (TIFFIsTiled(tiff))
      {
         spdlog::error("There is no support for tiled images yet.");
         exit(EXIT_FAILURE);
      }
      else
      {
         uint16 bps = 1;

         // Format is currently undefined: https://www.awaresystems.be/imaging/tiff/tifftags/sampleformat.html
         uint16 format = 4;

         // The number of bytes a strip occupies
         uint64 sls = TIFFScanlineSize64(tiff);

         TIFFGetField(tiff, TIFFTAG_BITSPERSAMPLE, &bps);
         TIFFGetField(tiff, TIFFTAG_SAMPLEFORMAT, &format);

         tdata_t buf = _TIFFmalloc((signed int)sls);
         for (size_t row = 0; row < this->nRows; row++)
         {

            if (TIFFReadScanline(tiff, buf, row) == -1)
            {
               spdlog::error("Error when reading scanline");
               exit(EXIT_FAILURE);
            }

            for (size_t col = 0; col < this->nCols; col++)
            {
               double val = 0;

               switch (format)
               {
               case 1:
                  if (bps == 8)
                     val = (double)((uint8 *)buf)[col];
                  else if (bps == 16)
                     val = (double)((uint16 *)buf)[col];
                  else if (bps == 32)
                     val = (double)((uint32 *)buf)[col];
                  else
                     val = (double)((uint64 *)buf)[col];
                  break;
               case 2:
                  if (bps == 8)
                     val = (double)((int8 *)buf)[col];
                  else if (bps == 16)
                     val = (double)((int16 *)buf)[col];
                  else if (bps == 32)
                     val = (double)((int32 *)buf)[col];
                  else
                     val = (double)((int64 *)buf)[col];
                  break;
               case 3:
                  if (bps == 32)
                     val = (double)((float *)buf)[col];
                  else
                     val = ((double *)buf)[col];
                  break;
               default:
                  spdlog::error("Unknown data format.");
                  exit(EXIT_FAILURE);
               }
               this->at(this->nRows - row - 1, col) = val;
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
   }

   void Raster::toTiff(const std::string path) const
   {
      // clang-format off
        // Key Directory
        // Is a vector in case it needs to expand programatically with new keys
      std::vector<uint16> kd = {
         // The first 4 entries specify the following:
         // GeoTIFF Version Major, Key Revision, Minor Revision, Number of Keys
         1, 1, 0, 2,
         // The following entries are a set of sorted keys organized in the following manner:
         // KeyID, Tag Location, Count, Value Offset
         // The meaning of the values are specified in the GeoTIFF specification, Annex B, section 1, paragraph 4
         // (GeoTIFF File and "Key" Structure) as of GeoTIFF version 1.1
         1024, 0, 1, 1,
         1025, 0, 1, 1,
      };
      // clang-format on

      // model pixel scale values
      double mps[3] = {this->cellsize, this->cellsize, 0.0};

      // model tiepoint values
      double mtp[6] = {0.0, 0.0, 0.0, this->xllcorner, this->yllcorner + (this->nRows * this->cellsize), 0.0};

      _XTIFFInitialize();

      TIFF *tiff = TIFFOpen(path.c_str(), "w");

      if (tiff == NULL)
      {
         spdlog::error("Failed to open {} for writing", path);
         exit(EXIT_FAILURE);
      }

      // TIFF tags
      TIFFSetField(tiff, TIFFTAG_IMAGEWIDTH, this->nCols);
      TIFFSetField(tiff, TIFFTAG_IMAGELENGTH, this->nRows);
      TIFFSetField(tiff, TIFFTAG_SOFTWARE, "KiLib");
      TIFFSetField(tiff, TIFFTAG_BITSPERSAMPLE, 64);
      TIFFSetField(tiff, TIFFTAG_SAMPLESPERPIXEL, 1);
      TIFFSetField(tiff, TIFFTAG_SAMPLEFORMAT, 3);
      TIFFSetField(tiff, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
      TIFFSetField(tiff, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_MINISBLACK);

      // GeoTIFF tags
      if (kd.size() != (size_t)(4 + kd[3] * 4))
      {
         spdlog::error("Invalid number of entries in the GeoTIFF Key Dictionary");
         exit(EXIT_FAILURE);
      }

      TIFFSetField(tiff, GEOTIFFTAG_KEYDIRECTORY, kd.size(), kd.data());
      TIFFSetField(tiff, GEOTIFFTAG_MODELPIXELSCALE, 3, mps);
      TIFFSetField(tiff, GEOTIFFTAG_MODELTIEPOINT, 6, mtp);
      TIFFSetField(tiff, GEOTIFFTAG_NODATAVALUE, fmt::format("{} ", this->nodata_value).c_str());

      // Writing data to file
      uint64  sls = TIFFScanlineSize64(tiff);
      tdata_t buf = _TIFFmalloc((signed int)sls);

      // There is no use in parallelizing this as the file has to be written in order
      for (size_t row = 0; row < this->nRows; row++)
      {
         for (size_t col = 0; col < this->nCols; col++)
            ((double *)buf)[col] = this->at(this->nRows - row - 1, col);

         if (TIFFWriteScanline(tiff, buf, row) == -1)
         {
            spdlog::error("Failed to write data to {}", path);
            exit(EXIT_FAILURE);
         }
      }

      _TIFFfree(buf);
      TIFFClose(tiff);
   }
} // namespace KiLib