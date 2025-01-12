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
#include <cstddef>
#include <netcdf.h>
#include <filesystem>
#include <stdexcept>
#include <limits.h>

namespace KiLib::Rasters
{
/**
* Load a NetCDF file into a KiLib Raster container.
*/ 
template<typename T>
static KiLib::Rasters::Raster<T> FromNetCDF(const std::string &path, const std::string var_name, size_t index, std::function<bool(T&, size_t, double)> construct_val) {

   if( ! std::filesystem::exists(path)) {
      throw std::invalid_argument("Path does not exist");
   }

   int retval, ncid;

   /* Open the file. */
   if ((retval = nc_open(path.c_str(), NC_NOWRITE, &ncid))) {
      //TODO: Change this to report the retval as well. That will make this more useful
      throw std::invalid_argument("NetCDF could not open file");
   }

   // Determine the details of the file. How many dims? vars? etc
   // ncid = 
   // ndims_in = Number of dimensions
   // nvars_in = Number of variables
   // ngatts_in = Number of global attributes
   // unlimdimid_in = The id of a domain that is unlimited dimensions (like time). Should be -1 if none.
   int ndims_in, nvars_in, ngatts_in, unlimdimid_in;
   if ((retval = nc_inq(ncid, &ndims_in, &nvars_in, &ngatts_in, &unlimdimid_in))) {
      throw std::invalid_argument("Could not inquire details about the file!");
   }

   if ( ndims_in != 3 ) {
      throw std::invalid_argument("Invalid amount of dims for loading from a NetCDF file");
   }

   /*if ( unlimdimid_in == -1 ) {  
      throw std::invalid_argument("Missing time (unlimited) dimension");
   }*/

   // Determine VAR ID from name given in parameters
   int varid_in;
   if ((retval = nc_inq_varid(ncid, var_name.c_str(), &varid_in))) {
      throw std::invalid_argument("Invalid variable name for NetCDF File");
   }

   // Determine Dimension ids
   int dim_y_id, dim_x_id, dim_time_id;
   if (( retval = nc_inq_dimid( ncid, "y", &dim_y_id ))) {
      throw std::invalid_argument("Could not find project_y_coordinate dimension");
   }
   if (( retval = nc_inq_dimid( ncid, "y", &dim_x_id ))) {
      throw std::invalid_argument("Could not find project_x_coordinate dimension");
   }
   if (( retval = nc_inq_dimid( ncid, "time", &dim_time_id ))) {
      throw std::invalid_argument("Could not find project_y_coordinate dimension");
   }

   // Determine dimension sizes
   size_t dim_y_len, dim_x_len, dim_time_len;
   if (( retval = nc_inq_dimlen( ncid, dim_y_id, &dim_y_len ))) {
      throw std::invalid_argument("Could not find 'y' length");
   }
   if (( retval = nc_inq_dimlen( ncid, dim_x_id, &dim_x_len ))) {
      throw std::invalid_argument("Could not find 'x' length");
   }
   if (( retval = nc_inq_dimlen( ncid, dim_time_id, &dim_time_len ))) {
      throw std::invalid_argument("Could not find 'time' length");
   }

   // Now we can read it into a raster... abiet a little slowly right now but hey, it should work and
   // it won't be that massive right now.
   KiLib::Rasters::Raster<T> raster( dim_x_len, dim_y_len );

   // A bit slower, but easier to be precise here
   double yllcorner = std::numeric_limits<double>::max();
   double xllcorner = std::numeric_limits<double>::max();
   for( size_t x = 0; x < dim_x_len; x++ ) {
      for( size_t y = 0; y < dim_y_len; y++ ) {
         // Let's check to see if we have our lower corners for the raster later
         double y_loc;
         if (( retval = nc_get_var1_double(ncid, dim_y_id, &y, &y_loc))) {
            throw std::invalid_argument("Could not retrieve y from netcdf");
         }
         yllcorner = std::min(yllcorner, y_loc);

         double x_loc;
         if (( retval = nc_get_var1_double(ncid, dim_x_id, &x, &x_loc))) {
            throw std::invalid_argument("Could not retrieve value from netcdf");
         }
         xllcorner = std::min(xllcorner, x_loc);

         T v;
            size_t indexp[] = { index, y, x };
            double result;
            if (( retval = nc_get_var1_double(ncid, dim_time_id, indexp, &result))) {
               throw std::invalid_argument("Could not retrieve value from netcdf");
            }

            if( ! construct_val( v, index, result ) ) {
               throw std::invalid_argument("Could not construct values for given type");

            }

         raster.set(x, y, v);
      }
   }

   size_t cellsize = 1;

   raster.set_yllcorner(yllcorner);
   raster.set_xllcorner(xllcorner);
   raster.set_nodatavalue(9999);

   raster.set_width(dim_y_len * cellsize);
   raster.set_height(dim_x_len * cellsize);
   raster.set_cellsize(cellsize);

   if (( retval = nc_close( ncid ))) {
      throw std::invalid_argument("Could not close NetCDF handle");
   }

   return raster;
};


struct NetCDFDims {
   int id;
   std::string name;
   size_t length;
   bool is_unlimited;
};

struct NetCDFVar {
   int id;
   std::string name;
   nc_type type;
   std::vector<int> dims;
   int natts;
};

struct NetCDFInfo {
   std::string path;
   std::vector<NetCDFDims> dims;
   std::vector<NetCDFVar> vars;
};


static NetCDFInfo GetNetCDFInfo(const std::string &path) {

   if( ! std::filesystem::exists(path)) {
      throw std::invalid_argument("Path does not exist");
   }

   int retval, ncid;

   /* Open the file. */
   if ((retval = nc_open(path.c_str(), NC_NOWRITE, &ncid))) {
      //TODO: Change this to report the retval as well. That will make this more useful
      throw std::invalid_argument("NetCDF could not open file");
   }

   // Determine the details of the file. How many dims? vars? etc
   // ncid = 
   // ndims_in = Number of dimensions
   // nvars_in = Number of variables
   // ngatts_in = Number of global attributes
   // unlimdimid_in = The id of a domain that is unlimited dimensions (like time). Should be -1 if none.
   int ndims_in, nvars_in, ngatts_in, unlimdimid_in;
   if ((retval = nc_inq(ncid, &ndims_in, &nvars_in, &ngatts_in, &unlimdimid_in))) {
      throw std::invalid_argument("Could not inquire details about the file!");
   }

   NetCDFInfo r;
   for ( int i = 0; i < ndims_in; i++ ) {
      char* name = (char*)malloc((NC_MAX_NAME + 1) * sizeof(char));
      size_t len;

      if (( retval = nc_inq_dim( ncid, i, name, &len ))) {
         throw std::invalid_argument("Could not retrieve dimension information");
      }  

      r.dims.push_back({
         .id = i,
         .name = std::string(name),
         .length = len,
         .is_unlimited = i == unlimdimid_in
      });



      free(name);
   }

   for ( int i = 0; i < nvars_in; i++ ) {

      char* name = (char*)malloc((NC_MAX_NAME + 1) * sizeof(char));
      nc_type xtype;
      int ndims, natts;
      int dimids[NC_MAX_VAR_DIMS];
      

      if (( retval = nc_inq_var( ncid, i, name, &xtype, &ndims, dimids, &natts ))) {
         throw std::invalid_argument("Could not retrieve dimension information");
      }  

      r.vars.push_back({
         .id = i,
         .name = std::string(name),
         .type = xtype,
         .dims = std::vector<int>(dimids, dimids + ndims),
         .natts = natts
      });


      free(name);
   }

   return r;
}




} //namespace KiLib::Rasters
