#pragma once

#include "IRaster.hpp"
#include <KiLib/Rasters/IRaster.hpp>
#include <algorithm>
#include <functional>
#include <iostream>
#include <memory>
#include <stddef.h>
#include <optional>
#include <stdexcept>
#include <valarray>


namespace KiLib::Rasters
{
   template <typename T> class SparseRaster : public IValArrayRaster<T>
   {
   public:
      SparseRaster()
      {
      }
      /*
          SparseRaster(const KiLib::Raster& from_raster, double threshold, T default_value, std::function<void(T& obj,
         double value)> set_cell) : rows(from_raster.nRows), cols(from_raster.nCols)
          {
              // get NNZ
              SparseRaster::nnz = 0;
              for (size_t row = 0; row < from_raster.nRows; row++)
              {
                  for (size_t col = 0; col < from_raster.nCols; col++)
                  {
                      if (from_raster.at(row, col) > threshold)
                      {
                          nnz++; // get a total amount of cells we will be processing
                      }
                  }
              }

              this->V = new T[nnz];
              std::fill_n(V, nnz, default_value);

              this->COL_INDEX = new size_t[nnz];
              this->ROW_INDEX = new size_t[rows + 1];
              this->default_value = default_value;

              // import nnz values into the new system
              size_t row_index = 0;
              size_t v_index = 0;

              for (size_t row = 0; row < from_raster.nRows; row++)
              {
                  ROW_INDEX[row_index] = v_index;
                  for (size_t col = 0; col < from_raster.nCols; col++)
                  {
                      if (from_raster.at(row, col) > threshold)
                      {
                          // Put value into the value array
                          set_cell(V[v_index], from_raster.at(row,col));

                          //Link col_index to V
                          COL_INDEX[v_index] = col;

                          // increase the next index
                          v_index++;
                      }
                  }
                  row_index++;
              }

              ROW_INDEX[row_index] = v_index;

              this->xllcorner = from_raster.xllcorner;
              this->yllcorner = from_raster.yllcorner;
              this->cellsize = from_raster.cellsize;
              this->width = from_raster.width;
              this->height = from_raster.height;
              this->nodata_value = from_raster.nodata_value;
          }
  */
      SparseRaster( const std::tuple<size_t, size_t, size_t>& dims, size_t count ) : nnz( count ), V( count ), COL_INDEX(nnz), ROW_INDEX(std::get<0>(dims) + 1)
      {
         const auto rows   = std::get<0>( dims );
         const auto cols   = std::get<1>( dims );
         const auto zindex = std::get<2>( dims );

         this->rows   = rows;
         this->cols   = cols;
         this->zindex = zindex;


         // import nnz values into the new system
         /*
         size_t row_index = 0;
         size_t v_index = 0;


         for (size_t row = 0; row < from_raster.get_rows(); row++)
         {
             ROW_INDEX[row_index] = v_index;
             for (size_t col = 0; col < from_raster.get_cols(); col++)
             {
                 if (!from_raster.get(row, col).is_nodata)
                 {
                     // Put value into the value array
                     V[v_index] = *(from_raster.at(row, col).data);

                     //Link col_index to V
                     COL_INDEX[v_index] = col;

                     // increase the next index
                     v_index++;
                 }
             }
             row_index++;
         }

         ROW_INDEX[row_index] = v_index;

         this->xllcorner = from_raster.get_xllcorner();
         this->yllcorner = from_raster.get_yllcorner();
         this->cellsize = from_raster.get_cellsize();
         this->width = from_raster.get_width();
         this->height = from_raster.get_height();
         this->nodata_value = from_raster.get_nodata_value();
                   */
      }

        SparseRaster(const SparseRaster<T>& other, const std::valarray<T>& new_data) : nnz( other.nnz ), V( new_data ), COL_INDEX(other.COL_INDEX), ROW_INDEX(other.ROW_INDEX)
         {

         this->nnz  = other.get_ndata();
         this->rows = other.get_rows();
         this->cols = other.get_cols();
         this->zindex = other.get_zindex();

         this->set_xllcorner( other.get_xllcorner() );
         this->set_yllcorner( other.get_yllcorner() );
         this->set_cellsize( other.get_cellsize() );
         this->set_nodata_value( other.get_nodata_value() );
         this->set_name( other.get_name() );
         this->set_width( other.get_width() );
         this->set_height( other.get_height() );

        }


      ~SparseRaster()
      {
      }

     const std::valarray<T>& get_valarray() const override { return V; }

      using IRaster<T>::get;
      KiLib::Rasters::Cell<T> get( size_t i, size_t j, size_t k = 0 ) const override
      {
        auto d = _get_data_index(i, j, k);

        if ( d.has_value() ) {
               return KiLib::Rasters::Cell<T>( *this, i, j, V[d.value()] );
        }
        else {

        return KiLib::Rasters::Cell<T>( *this, i, j );
        }
      }

      using IRaster<T>::set;
      void set( size_t i, size_t j, size_t k, const T& value ) override
      {


        auto d = _get_data_index(i, j, k);

        if ( d.has_value() ) {
               V[d.value()]  = value;
        }
        else {
            throw std::invalid_argument("Invalid index. Could not set value");
        }
      }

      size_t get_ndata() const override
      {
         return this->nnz;
      }

      T min() const {
         T m = std::numeric_limits<double>::max();
         for( size_t i = 0; i < nnz; i++ ) {
               m = std::min(m, V[i]);
         }
         return m;
      }

      T max() const {
         T m = std::numeric_limits<double>::min();
         for( size_t i = 0; i < nnz; i++ ) {
               m = std::max(m, V[i]);
         }
         return m;
      }


    bool operator==(const SparseRaster<T>& rhs) const {
        if ( ! can_perform_operation(rhs) ) return false;

         if ( ! std::equal(std::begin(this->V), std::end(this->V), std::begin(rhs.V) )) return false;
         if ( ! std::equal(std::begin(this->COL_INDEX), std::end(this->COL_INDEX), std::begin(rhs.COL_INDEX) )) return false;
         if ( ! std::equal(std::begin(this->ROW_INDEX), std::end(this->ROW_INDEX), std::begin(rhs.ROW_INDEX) )) return false;

        return true;

    }

template <typename C> SparseRaster<T> operator*(C k ) const
{
    KiLib::Rasters::SparseRaster<T> out(*this, V * k);
   return out;
}



    SparseRaster<T> operator*( const SparseRaster<T>& b) const
    {
    if ( ! can_perform_operation(b) ) {
        throw std::invalid_argument("Rasters cannot be multiplied together because they aren't the same dimensions");
    }
       KiLib::Rasters::SparseRaster<T> out(b, V * b.V);
    out.set_name(this->get_name() + " * " + b.get_name());
   return out;
    }

template <typename C> KiLib::Rasters::SparseRaster<T> operator/(const C k ) const
{
    KiLib::Rasters::SparseRaster<T> out(*this, V / k);
   return out;
}

        template <typename C> friend KiLib::Rasters::SparseRaster<T> operator/( const C, const KiLib::Rasters::SparseRaster<T>&  );


SparseRaster<T> operator/(const SparseRaster<T>& b )
{
    if ( ! can_perform_operation(b) ) {
        throw std::invalid_argument("Rasters cannot be divided because they aren't the same dimensions");
    }
   KiLib::Rasters::SparseRaster<T> out(*this, V / b.V);
    out.set_name(this->get_name() + " / " + b.get_name());
   return out;
}

SparseRaster<T> operator+(const SparseRaster<T>& b )
{
    if ( ! can_perform_operation(b) ) {
        throw std::invalid_argument("Rasters cannot be divided because they aren't the same dimensions");
    }
   SparseRaster<T> out(*this, V + b.V);
    out.set_name(this->get_name() + " + " + b.get_name());
   return out;
}

SparseRaster<T> operator-(const SparseRaster<T>& b )
{
    if ( ! can_perform_operation(b) ) {
        throw std::invalid_argument("Rasters cannot be divided because they aren't the same dimensions");
    }
   SparseRaster<T> out(*this, V - b.V);
    out.set_name(this->get_name() + " - " + b.get_name());
   return out;
}
        friend KiLib::Rasters::SparseRaster<T> std::clamp( const KiLib::Rasters::SparseRaster<T>&  ,const T&, const T&);



   private:
      size_t           nnz;
      T                default_value;
      std::valarray<T> V;
      std::vector<size_t> COL_INDEX, ROW_INDEX;

      bool is_valid_cell( size_t i, size_t j, size_t k ) const override
      {
        return _get_data_index(i,j,k).has_value();
      }

         T get_data(size_t i, size_t j, size_t k) const override {
            return V[_get_data_index(i,j,k).value()];
         }

    std::optional<T> _get_data_index(size_t i, size_t j, size_t k) const  {
        // Get row
         size_t       row_start = ROW_INDEX[i];
         const size_t row_end   = ROW_INDEX[i + 1];

         // No data? Return default data
         if ( row_start == row_end )
                return {};

         for ( ; row_start < row_end; row_start++ )
         {
            if ( COL_INDEX[row_start] == j )
            {
               return row_start + k;
            }
            else if ( COL_INDEX[row_start] > j )
            { // We have gone too far and won't find anything now.
                return {};
            }
         }
                return {};

         }


        bool can_perform_operation(const SparseRaster<T>& other) const {
         // Shortcut true
         if ( this == &other ) return true;

         // Check each property
         if ( this->get_xllcorner() != other.get_xllcorner() ) return false;
         if ( this->get_yllcorner() != other.get_yllcorner() ) return false;
         if ( this->get_height() != other.get_height() ) return false;
         if ( this->get_width() != other.get_width() ) return false;
         if ( this->get_nodata_value() != other.get_nodata_value() ) return false;
         if ( this->get_cellsize() != other.get_cellsize() ) return false;
         if ( this->get_rows() != other.get_rows() ) return false;
         if ( this->get_cols() != other.get_cols() ) return false;
         if ( this->get_zindex() != other.get_zindex() ) return false;

        return true;

    }
   }; //SparseRaster
} // namespace KiLib::Rasters


template <typename T, typename C> KiLib::Rasters::SparseRaster<T> operator*( const C k, const KiLib::Rasters::SparseRaster<T>& a )
{
   return a * k;
}

template <class T, typename C> KiLib::Rasters::SparseRaster<T> operator/( const C k, const KiLib::Rasters::SparseRaster<T>& a )
{
    KiLib::Rasters::SparseRaster<T> out(a, k / a.V);
    return out;
}

template <class T, typename C> KiLib::Rasters::SparseRaster<T> operator-( const C k, const KiLib::Rasters::SparseRaster<T>& a )
{
   KiLib::Rasters::SparseRaster<T> out(a, k - a.V );
   return out;
}
template <class T, typename C> KiLib::Rasters::SparseRaster<T> operator+( const C k, const KiLib::Rasters::SparseRaster<T>& a )
{
   return a + k;
}


template <class T> KiLib::Rasters::SparseRaster<T> operator-( const KiLib::Rasters::SparseRaster<T>& a )
{
   return -1 * a;
}


namespace std {
   template <class T> KiLib::Rasters::SparseRaster<T> clamp( const KiLib::Rasters::SparseRaster<T>& a, const T& lo, const T& hi)
   {
      auto in = a.V;
      std::transform(std::begin(in), std::end(in), std::begin(in), [&lo, &hi](T& v) { return std::clamp(v, lo, hi); });
      KiLib::Rasters::SparseRaster<T> out( a, in );
      return out;
   }

   template <class T> KiLib::Rasters::SparseRaster<T> ierfc ( const KiLib::Rasters::SparseRaster<T>& a) {
      const auto n = a.V;
      const auto r = 1 / std::sqrt(M_PI) * std::exp( -1 * std::pow(n, 2)) - n * std::erfc(n);
      KiLib::Rasters::SparseRaster<T> out( a, r );
      return out;
   }

   template <class T> KiLib::Rasters::SparseRaster<T> atan( const KiLib::Rasters::SparseRaster<T>& a )
   {
      std::valarray<T>          result = std::atan( a.V );
      KiLib::Rasters::SparseRaster<T> out( a, result );
      return out;
   }

   template <class T> KiLib::Rasters::SparseRaster<T> sin( const KiLib::Rasters::SparseRaster<T>& a )
   {
      std::valarray<T>          result = std::sin( a.V );
      KiLib::Rasters::SparseRaster<T> out( a, result );
      return out;
   }

   template <class T> KiLib::Rasters::SparseRaster<T> cos( const KiLib::Rasters::SparseRaster<T>& a )
   {
      std::valarray<T>          result = std::cos( a.V );
      KiLib::Rasters::SparseRaster<T> out( a, result );
      return out;
   }

   template <class T> KiLib::Rasters::SparseRaster<T> tan( const KiLib::Rasters::SparseRaster<T>& a )
   {
      std::valarray<T>          result = std::tan( a.V );
      KiLib::Rasters::SparseRaster<T> out( a, result );
      return out;
   }

   template <class T> T min( const KiLib::Rasters::SparseRaster<T>& a )
   {
      return a.min();
   }

   template <class T> T max( const KiLib::Rasters::SparseRaster<T>& a )
   {
      return a.max();
   }


};
