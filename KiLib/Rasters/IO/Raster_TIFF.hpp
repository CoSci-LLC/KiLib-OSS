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


#include <KiLib/Rasters/Raster.hpp>
#include <KiLib/Rasters/MetadataRaster.hpp>
#include <cstdint>
#include <stdexcept>
#include <tiffio.hxx>
#include <optional>
#ifdef _OPENMP
#include <omp.h>
#endif
#include <spdlog/fmt/ostr.h>
#include <spdlog/spdlog.h>

#define GEOKEYDIRECTORYTAG_RASTERTYPE 1025

#define GEOTIFFTAG_KEYDIRECTORY 34735
#define GEOTIFFTAG_DOUBLEPARAMS 34736
#define GEOTIFFTAG_ASCIIPARAMS 34737
#define GEOTIFFTAG_MODELPIXELSCALE 33550
#define GEOTIFFTAG_MODELTIEPOINT 33922
#define GEOTIFFTAG_NODATAVALUE 42113
#define GEOTIFFTAG_GDALMETADATA 42112
#define TIFFTAG_BANKFORMAP_METADATA 53100

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
      {GEOTIFFTAG_GDALMETADATA, -1, -1, TIFF_ASCII, FIELD_CUSTOM, true, false, (char *)"GDALMetadata"},
      {GEOTIFFTAG_NODATAVALUE, -1, -1, TIFF_ASCII, FIELD_CUSTOM, true, false, (char *)"GeoNoDataValue"},
      {TIFFTAG_BANKFORMAP_METADATA, -1, -1, TIFF_ASCII, FIELD_CUSTOM, true, false, (char *)"BankforMAPData"},
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
static inline void _XTIFFInitialize(void)
{
   static bool b = false;

   if (b)
      return;

   b = true;

   /* Grab the inherited method and install */
   _ParentExtender = TIFFSetTagExtender(_XTIFFDefaultDirectory);
}

namespace KiLib::Rasters
{
   /**
    * @brief Load a raster from a tiff file
    * @param path path The path of the raster
    * @param construct_val A callback to build the proper value before it is placed in the raster.
    * @return A full raster with values from the TIFF file
    * @deprecated since 7.0.0
    * @see fromTIFF(...) for the more flexible replacement
    */ 
   template<typename T>
   static KiLib::Rasters::Raster<T> fromTiff(const std::string &path, std::function<bool(T&, double, bool)> construct_val)
   {
      _XTIFFInitialize();

      TIFF *tiff = TIFFOpen(path.c_str(), "r");

      if (tiff == NULL)
      {
         spdlog::error("Failed to open {} for reading", path);
         exit(EXIT_FAILURE);
      }

      size_t   free_flag = 0;
      uint16_t count     = 0;
      uint32_t w         = 0;
      uint32_t h         = 0;
      double  *scaling   = nullptr;
      double  *tiepoint  = nullptr;
      char    *nodat     = nullptr;
      char*   metadata  = nullptr;
      uint16_t key_directory_count = 0;
      uint16_t *key_directory     = 0;

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
         spdlog::trace("Failed to find pixel scaling. Assuming 1:1");
         scaling = new double[2]{1, 1};
         free_flag |= 1;
      }

      if (!TIFFGetField(tiff, GEOTIFFTAG_MODELTIEPOINT, &count, &tiepoint))
      {
         spdlog::trace("Failed to find model tiepoint. Assuming 0, 0");
         tiepoint = new double[6]{0};
         free_flag |= 2;
      }

      if (!TIFFGetField(tiff, GEOTIFFTAG_NODATAVALUE, &nodat))
      {
         spdlog::trace("Failed to find nodata value. Assuming -9999");
         nodat = new char[7]{'-', '9', '9', '9', '9', '\n'};
         free_flag |= 4;
      }


      if (!TIFFGetField(tiff, GEOTIFFTAG_GDALMETADATA, &metadata))
      {
         free_flag |= 8;
      }
   
      if (!TIFFGetField(tiff, GEOTIFFTAG_KEYDIRECTORY, &key_directory_count, &key_directory))
      {
         spdlog::trace("Failed to find raster type. Assuming raster is POINT style");
         free_flag |= 16;
      }

      bool raster_is_area = false;
      // Determine if raster type is point or area
      if ( key_directory_count != 0 ) {
         for (size_t i = 0; i < key_directory_count; ++i) {
            uint16_t tiff_tag_location = 0;
            uint16_t count = 0;
            uint16_t value_offset = 0;

            switch(key_directory[i]) {
            case GEOKEYDIRECTORYTAG_RASTERTYPE:
               tiff_tag_location = key_directory[i + 1];
               count = key_directory[i + 2];
               value_offset = key_directory[ i + 3];

               if (tiff_tag_location != 0 || count != 1) {
                  spdlog::error("Malformed Raster Type GeoKey");
                  break;
               }

               if (value_offset == 1 ) {
                  raster_is_area = true;
               }
               break;
            default:
               continue;
               
            }
         }
      }



      const size_t nRows = h;
      const size_t nCols = w;

      KiLib::Rasters::Raster<T> raster( nRows, nCols );
      raster.set_width( nCols * scaling[0] );
      raster.set_height( nRows * scaling[1] );
      raster.set_xllcorner( tiepoint[3] );
      raster.set_yllcorner( tiepoint[4] - (nRows * scaling[1]) );
      raster.set_cellsize( scaling[0] );
      raster.set_nodata_value ( std::stod(nodat) );
      raster.set_name(path);

      if (raster_is_area) {

         raster.set_yllcorner( raster.get_yllcorner() - raster.get_cellsize() / 2);
         raster.set_xllcorner( raster.get_xllcorner() + raster.get_cellsize() / 2);

      }


      // There is a piece of software out there that doesn't do the nodata_value correctly. This is here to fix that:
      if ( raster.get_nodata_value() == 3.40282346600000016e+38) {
         raster.set_nodata_value(3.4028234663852886e+38);
      }


      if (TIFFIsTiled(tiff))
      {
         throw std::invalid_argument("There is no support for tiled images yet.");
      }
      else
      {
         uint16_t bps = 1;

         // Format is currently undefined: https://www.awaresystems.be/imaging/tiff/tifftags/sampleformat.html
         uint16_t format = 4;

         // The number of bytes a strip occupies
         uint64_t sls = TIFFScanlineSize64(tiff);

         TIFFGetField(tiff, TIFFTAG_BITSPERSAMPLE, &bps);
         TIFFGetField(tiff, TIFFTAG_SAMPLEFORMAT, &format);

         tdata_t buf = _TIFFmalloc((signed int)sls);
         for (size_t row = 0; row < nRows; row++)
         {

            if (TIFFReadScanline(tiff, buf, row) == -1)
            {
               throw std::invalid_argument("Error when reading scanline");
            }

            for (size_t col = 0; col < nCols; col++)
            {
               double val = 0;

               switch (format)
               {
               case 1:
                  if (bps == 8)
                     val = (double)((uint8_t *)buf)[col];
                  else if (bps == 16)
                     val = (double)((uint16_t *)buf)[col];
                  else if (bps == 32)
                     val = (double)((uint32_t *)buf)[col];
                  else
                     val = (double)((uint64_t *)buf)[col];
                  break;
               case 2:
                  if (bps == 8)
                     val = (double)((int8_t *)buf)[col];
                  else if (bps == 16)
                     val = (double)((int16_t *)buf)[col];
                  else if (bps == 32)
                     val = (double)((int32_t *)buf)[col];
                  else
                     val = (double)((int64_t *)buf)[col];
                  break;
               case 3:
                  if (bps == 32)
                     val = (double)((float *)buf)[col];
                  else
                     val = ((double *)buf)[col];
                  break;
               default:
                  throw std::invalid_argument("Unknown data format.");
               }
               T v;
               if( construct_val( v, val, val == raster.get_nodata_value() ) ) {
                    raster.set(nRows - row - 1, col, v);
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

      if (free_flag & 8)
         delete metadata;

      if (free_flag & 16) 
         delete key_directory;

      TIFFClose(tiff);

      return raster;
   }


    template<class T>
    struct RasterDirectory {
      
        const IRaster<T>& raster;
        std::function<double(const Cell<T>&)> getter;
         size_t zindex;
        
        /**
         * @brief Get the value or nodata value of a cell. This will count cells
         * without a valid CA as invalid
         *
         * @param cell The cell to investigate
         * @param v The value to check
         */
         static inline double get_value_or_nodata(const Cell<T>& cell, std::function<std::optional<double>()> v) {
            if (cell.is_nodata || !cell.data->ca.has_value()) {
                return cell.parent_raster.get_nodata_value();
            } 
            
            return v().value_or(cell.parent_raster.get_nodata_value());
        };
    };

    template<class T>
    void toTiff(const std::string& filepath, const std::vector<RasterDirectory<T>>& directories)
    {
         // clang-format off
        // Key Directory
        // Is a vector in case it needs to expand programatically with new keys
        std::vector<uint16_t> kd = {
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

        _XTIFFInitialize();

        TIFF *tiff = TIFFOpen(filepath.c_str(), "w8");

        if (tiff == NULL)
        {
            spdlog::error("Failed to open {} for writing", filepath);
            exit(EXIT_FAILURE);
        }

        

        size_t count = 0;
        do {
            //spdlog::info("Writing {}", directories[count].name);
            //TIFFSetField(tiff, TIFFTAG_COMPRESSION, 5); // LZW Compression

        const auto& raster = directories[count].raster;
            
        // model pixel scale values
        double mps[3] = {raster.get_cellsize(), raster.get_cellsize(), 0.0};

        // model tiepoint values
        double mtp[6] = {0.0, 0.0, 0.0, raster.get_xllcorner(), raster.get_yllcorner() + (raster.get_rows() * raster.get_cellsize()), 0.0};



            // GeoTIFF tags
            if (kd.size() != (size_t)(4 + kd[3] * 4))
            {
                spdlog::error("Invalid number of entries in the GeoTIFF Key Dictionary");
                exit(EXIT_FAILURE);
            }
            // TIFF tags
            
            // Add layer into name if applicable
            std::string tmp_name;
            if ( directories[count].zindex != 0 ) {
               tmp_name = (directories[count].raster.get_name() + " [" + std::to_string(directories[count].zindex) + "]");
            } else {
               tmp_name = (directories[count].raster.get_name());
            }

            const char* name = tmp_name.c_str();

            TIFFSetField(tiff, TIFFTAG_IMAGEDESCRIPTION, name);
            TIFFSetField(tiff, TIFFTAG_DOCUMENTNAME, name);
            TIFFSetField(tiff, TIFFTAG_IMAGEWIDTH, raster.get_cols());
            TIFFSetField(tiff, TIFFTAG_IMAGELENGTH, raster.get_rows());
            TIFFSetField(tiff, TIFFTAG_SOFTWARE, "CoSci LLC: KiLib");
            TIFFSetField(tiff, TIFFTAG_BITSPERSAMPLE, 64);


            time_t     now = time(0);
            struct tm  tstruct;
            char       time_buf[20];
            tstruct = *localtime(&now);
            // Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
            // for more information about date/time format
            strftime(time_buf, sizeof(time_buf), "%Y:%m:%d %X", &tstruct);

            TIFFSetField(tiff, TIFFTAG_DATETIME, time_buf);


            TIFFSetField(tiff, TIFFTAG_SAMPLESPERPIXEL, 1);
            TIFFSetField(tiff, TIFFTAG_SAMPLEFORMAT, 3);
            TIFFSetField(tiff, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
            TIFFSetField(tiff, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_MINISBLACK);
            TIFFSetField(tiff, GEOTIFFTAG_KEYDIRECTORY, kd.size(), kd.data());
            TIFFSetField(tiff, GEOTIFFTAG_MODELPIXELSCALE, 3, mps);
            TIFFSetField(tiff, GEOTIFFTAG_MODELTIEPOINT, 6, mtp);
            TIFFSetField(tiff, GEOTIFFTAG_NODATAVALUE, fmt::format("{} ", raster.get_nodata_value()).c_str());



            //TODO: Add CoSci Tags and data informtation into this!
            const auto metadataString = fmt::format(
                "<GDALMetadata>"
                "\n\t<Item name=\"DESCRIPTION\" sample=\"0\" role=\"description\">{}</Item>"
                "\n\t<Item name=\"grid_name\">{}</Item>"
                "\n\t<Item name=\"UNITS\" sample=\"0\">Meters (elevation)</Item>"
                "\n</GDALMetadata>",
                  name, name, name
            );

            const char* gdalMetadata = metadataString.c_str();

            TIFFSetField(tiff, TIFFTAG_GDAL_METADATA, gdalMetadata);


            const char* cosciMetadata = "{ \"data\": \"KiLib Generated\"}";
            TIFFSetField(tiff, TIFFTAG_BANKFORMAP_METADATA, cosciMetadata);

            // Writing data to file
            uint64_t sls = TIFFScanlineSize64(tiff);
            tdata_t  buf = _TIFFmalloc((signed int)sls);

            // There is no use in parallelizing this as the file has to be written in order
            for (size_t row = 0; row < raster.get_rows(); row++)
            {
                for (size_t col = 0; col < raster.get_cols(); col++)
                    ((double *)buf)[col] = directories[count].getter(raster(raster.get_rows() - row - 1, col, directories[count].zindex));

                if (TIFFWriteScanline(tiff, buf, (uint32_t)row) == -1)
                {
                    spdlog::error("Failed to write data to {}", filepath);
                    exit(EXIT_FAILURE);
                }
            }

            TIFFWriteDirectory(tiff);
            _TIFFfree(buf);
            count++;
        } while (count < directories.size());

        
        TIFFClose(tiff);
    };


   
   /**
   * Load a TIFF File from filesystem
   * 
   * @param[in] path The location of the raster to load. There is no intelligence so be careful about relative vs absolute paths
   * @param[in] set_val A callback function to set the value at i and j coordinates
   * @param[in] metadata A reference to a Metadata Raster that will have the metadata set appropriately. This is necessary for rasters that need to be built before metadata can be applied. One such example is a SparseRaster.
   */
   static void fromTIFF(const std::string &path, std::function<void(size_t, size_t, double)> set_val, KiLib::Rasters::MetadataRaster<double>& metadata)
   {
      _XTIFFInitialize();

      TIFF *tiff = TIFFOpen(path.c_str(), "r");

      if (tiff == NULL)
      {
         spdlog::error("Failed to open {} for reading", path);
         exit(EXIT_FAILURE);
      }

      size_t   free_flag = 0;
      uint16_t count     = 0;
      uint32_t w         = 0;
      uint32_t h         = 0;
      double  *scaling   = nullptr;
      double  *tiepoint  = nullptr;
      char    *nodat     = nullptr;

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
         spdlog::trace("Failed to find pixel scaling. Assuming 1:1");
         scaling = new double[2]{1, 1};
         free_flag |= 1;
      }

      if (!TIFFGetField(tiff, GEOTIFFTAG_MODELTIEPOINT, &count, &tiepoint))
      {
         spdlog::trace("Failed to find model tiepoint. Assuming 0, 0");
         tiepoint = new double[6]{0};
         free_flag |= 2;
      }

      if (!TIFFGetField(tiff, GEOTIFFTAG_NODATAVALUE, &nodat))
      {
         spdlog::trace("Failed to find nodata value. Assuming -9999");
         nodat = new char[7]{'-', '9', '9', '9', '9', '\n'};
         free_flag |= 4;
      }

      const size_t nRows = h;
      const size_t nCols = w;

      metadata.set_width( nCols * scaling[0] );
      metadata.set_height( nRows * scaling[1] );
      metadata.set_xllcorner( tiepoint[3] );
      metadata.set_yllcorner( tiepoint[4] - (nRows * scaling[1]) );
      metadata.set_cellsize( scaling[0] );
      metadata.set_nodata_value ( std::stod(nodat) );
      metadata.set_name(path);


      // There is a piece of software out there that doesn't do the nodata_value correctly. This is here to fix that:
      if ( metadata.get_nodata_value() == 3.40282346600000016e+38) {
         metadata.set_nodata_value(3.4028234663852886e+38);
      }


      if (TIFFIsTiled(tiff))
      {
         throw NotImplementedException("There is no support for tiled images yet.");
      }
      else
      {
         uint16_t bps = 1;

         // Format is currently undefined: https://www.awaresystems.be/imaging/tiff/tifftags/sampleformat.html
         uint16_t format = 4;

         // The number of bytes a strip occupies
         uint64_t sls = TIFFScanlineSize64(tiff);

         TIFFGetField(tiff, TIFFTAG_BITSPERSAMPLE, &bps);
         TIFFGetField(tiff, TIFFTAG_SAMPLEFORMAT, &format);

         tdata_t buf = _TIFFmalloc((signed int)sls);
         for (size_t row = 0; row < nRows; row++)
         {

            if (TIFFReadScanline(tiff, buf, row) == -1)
            {
               throw std::invalid_argument("Error when reading scanline");
            }

            for (size_t col = 0; col < nCols; col++)
            {
               double val = 0;

               switch (format)
               {
               case 1:
                  if (bps == 8)
                     val = (double)((uint8_t *)buf)[col];
                  else if (bps == 16)
                     val = (double)((uint16_t *)buf)[col];
                  else if (bps == 32)
                     val = (double)((uint32_t *)buf)[col];
                  else
                     val = (double)((uint64_t *)buf)[col];
                  break;
               case 2:
                  if (bps == 8)
                     val = (double)((int8_t *)buf)[col];
                  else if (bps == 16)
                     val = (double)((int16_t *)buf)[col];
                  else if (bps == 32)
                     val = (double)((int32_t *)buf)[col];
                  else
                     val = (double)((int64_t *)buf)[col];
                  break;
               case 3:
                  if (bps == 32)
                     val = (double)((float *)buf)[col];
                  else
                     val = ((double *)buf)[col];
                  break;
               default:
                  throw std::invalid_argument("Unknown data format.");
               }
               
               // Use callback to let the user decided how to place this value. Let's
               // only use this when we have a true value, but if we detect there is 
               // a nodata val, we will just skip it. By default, the rasters should
               // init everything to the nodata value
               if ( val != metadata.get_nodata_value())
                  set_val(nRows - row - 1, col, val);
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



















} // namespace KiLib::Rasters
