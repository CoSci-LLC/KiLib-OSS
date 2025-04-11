#pragma once

#include "IRaster.hpp"
#include <KiLib/Exceptions/NotImplemented.hpp>
#include <KiLib/Rasters/IRaster.hpp>
#include <algorithm>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <sstream>
#include <stddef.h>
#include <stdexcept>
#include <tuple>
#include <valarray>

namespace KiLib::Rasters
{

   template <typename T> class MetadataRaster : virtual public IRaster<T>
   {
   public:
      MetadataRaster()
      {
      }

      using IRaster<T>::get;
      KiLib::Rasters::Cell<T> get( size_t, size_t, size_t ) const override
      {
         throw NotImplementedException( "This raster is only for metadata purposes" );
      }

      using IRaster<T>::set;
      void set( size_t, size_t, size_t, const T& ) override
      {
         throw NotImplementedException( "This raster is only for metadata purposes" );
      }

      size_t get_ndata() const override
      {
         throw NotImplementedException( "This raster is only for metadata purposes" );
      }

   private:
      bool is_valid_cell( size_t, size_t, size_t ) const override
      {

         throw NotImplementedException( "This raster is only for metadata purposes" );
      }

      T get_data( size_t, size_t, size_t ) const override
      {
         throw NotImplementedException( "This raster is only for metadata purposes" );
      }
   };
} // namespace KiLib::Rasters
