#pragma once

#include <KiLib/Rasters/IRaster.hpp>
#include <iterator>
#include <stddef.h>

namespace KiLib::Rasters
{
   template <typename T> class RasterIterator
   {
   public:
      using iterator_category = std::input_iterator_tag;
      using value_type        = T; // Typically will be doubles
      using difference_type   = double;
      using pointer           = const T*;
      using reference         = T;

      RasterIterator<T>( KiLib::Rasters::IRaster<T>* ptr ) : raster( ptr )
      {
      }

      reference operator*() const
      {
         return raster.get_data( i, j, k );
      }

      RasterIterator<T>& operator++()
      {

         do
         {
            if ( i < ( raster.get_rows() - 1 ) )
            {
               i++;
            }
            else if ( j < ( raster.get_cols() - 1 ) )
            {
               j++;
            }
            else if ( k < ( raster.get_zindex() - 1 ) )
            {
               k++;
            }
            else
            {
               break;
            }
         } while ( raster.is_valid_cell( i, j, k ) );

         return *this;
      }

      RasterIterator<T> operator++(int) {
         RasterIterator<T> tmp = *this;
         ++(*this);
         return tmp;
      }

      friend bool operator==(const RasterIterator<T>& a, const RasterIterator<T>& b) {
         return a.j == b.j && a.i == b.i && a.k == b.k;
      }

      friend bool operator!=(const RasterIterator<T>& a, const RasterIterator<T>& b) {
         return ! ( a == b);
      }

   private:
      KiLib::Rasters::IRaster<T>* raster;
      size_t                      i, j, k;
   }
} // namespace KiLib::Rasters
