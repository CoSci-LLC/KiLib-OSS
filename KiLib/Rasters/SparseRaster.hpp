#pragma once

#include "IRaster.hpp"
#include <KiLib/Rasters/IRaster.hpp>
#include <algorithm>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <stddef.h>
#include <optional>
#include <stdexcept>
#include <valarray>
#include <vector>


namespace KiLib::Rasters
{

   template<typename T> class Raster;

   template <typename T> class SparseRaster : public IRaster<T>
   {
   public:
      SparseRaster()
      {
      }

      SparseRaster(const KiLib::Rasters::SparseRaster<T>& from_raster, std::function<T(const Cell<T>&)> get_val)
          {
             COL_INDEX = from_raster.COL_INDEX;
             Z_INDEX = from_raster.Z_INDEX;
            ROW_INDEX = from_raster.ROW_INDEX;
              V.resize(from_raster.get_ndata());
            for ( size_t idx = 0; idx < from_raster.get_ndata(); idx++) {
               // We don't need to pass in the i/j right now. Really this is about the values
               V[idx] = get_val( Cell<T>(from_raster, 0, 0, from_raster.V[idx]));
            }

            this->copy_metadata_from(from_raster);
            this->rows = from_raster.get_rows();
            this->cols = from_raster.get_cols();
            this->zindex = from_raster.get_zindex();
            this->nnz = from_raster.get_ndata();

          }





       SparseRaster(const KiLib::Rasters::Raster<T>& from_raster, std::function<T(const Cell<T>&)> get_val)
          {
              nnz = 0;
              for (auto it = from_raster.begin(); it != from_raster.end(); ++it) {
                  nnz++;
               }
              V.resize(nnz);
             COL_INDEX.resize(nnz);
             Z_INDEX.resize(nnz);
             ROW_INDEX.resize(from_raster.get_rows() + 1);

              // import nnz values into the new system
              size_t row_index = 0;
              size_t v_index = 0;

              for (size_t row = 0; row < from_raster.get_rows(); row++)
              {
                  ROW_INDEX[row_index] = v_index;
                  for (size_t col = 0; col < from_raster.get_cols(); col++)
                  {

                     for (size_t z = 0; z < from_raster.get_zindex(); z++) {

                        if ( ! from_raster.get( row, col, z).is_nodata ) {
                           V[v_index] =  get_val(from_raster.get(row,col,z));

                           //Link col_index to V
                           this->COL_INDEX[v_index] = col;

                           // Link z_index to V
                           Z_INDEX[v_index] = z;

                          // increase the next index
                          v_index++;
                        }
                     }

                  }

                  row_index++;
              }

              ROW_INDEX[row_index] = v_index;



            this->copy_metadata_from(from_raster);
            this->rows = from_raster.get_rows();
            this->cols = from_raster.get_cols();
            this->zindex = from_raster.get_zindex();


          }

      SparseRaster( const std::tuple<size_t, size_t, size_t>& dims, const std::map<std::tuple<size_t, size_t, size_t>, double>& values ) : nnz( values.size() ), V(nnz), COL_INDEX(nnz), ROW_INDEX(std::get<0>(dims) + 1), Z_INDEX(nnz)
      {
         const auto rows   = std::get<0>( dims );
         const auto cols   = std::get<1>( dims );
         const auto zindex = std::get<2>( dims );

         this->rows   = rows;
         this->cols   = cols;
         this->zindex = zindex;

         // Sort by the rows and then columns to be able to place data into the vector properly

         size_t row_index = 0;
         size_t v_index = 0;


         for (size_t row = 0; row < rows; row++)
         {
            this->ROW_INDEX[row_index] = v_index;
            for (size_t col = 0; col < cols; col++)
            {
               for (size_t z = 0; z < zindex; z++) {

                  if ( values.count( {row, col, z}) != 0 ) {
                     V[v_index] = values.at( {row,col, z});

                     //Link col_index to V
                     this->COL_INDEX[v_index] = col;

                     // Link z_index to V
                     Z_INDEX[v_index] = z;

                    // increase the next index
                    v_index++;
                  }
               }
            }
            row_index++;
         }

         ROW_INDEX[row_index] = v_index;
      }


        SparseRaster(const SparseRaster<T>& other, const std::valarray<T>& new_data) : nnz( other.nnz ), V( new_data ), COL_INDEX(other.COL_INDEX), ROW_INDEX(other.ROW_INDEX), Z_INDEX(other.Z_INDEX)
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

      using IRaster<T>::get;
      KiLib::Rasters::Cell<T> get( size_t i, size_t j, size_t k = 0 ) const override
      {
        auto d = _get_data_index(i, j, k);

        if ( d.has_value() ) {
               return KiLib::Rasters::Cell<T>( *this, i, j, k, V[d.value()] );
        }
        else {

        return KiLib::Rasters::Cell<T>( *this, i, j ,k );
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

      T min() const override {
         T m = std::numeric_limits<double>::max();
         for( size_t i = 0; i < nnz; i++ ) {
               m = std::min(m, V[i]);
         }
         return m;
      }

      T max() const override {
         T m = std::numeric_limits<double>::min();
         for( size_t i = 0; i < nnz; i++ ) {
               m = std::max(m, V[i]);
         }
         return m;
      }

      TYPE get_type() const override { return TYPE::SPARSE; }
      
      SparseRaster<T> op_divide(const T val) const {
         auto nd = V == this->get_nodata_value();
          KiLib::Rasters::SparseRaster<T> out(*this, val / V);
            out.V[nd] = this->get_nodata_value();
            return out;
         }  

      SparseRaster<T> op_minus(const T val) const {
          KiLib::Rasters::SparseRaster<T> out(*this, val - V);
            return out;
         }  

      SparseRaster<T> op_ierfc() const {
            const auto n = V;
            const auto r = 1 / std::sqrt(M_PI) * std::exp( -1 * std::pow(n, 2)) - n * std::erfc(n);
            KiLib::Rasters::SparseRaster<T> out( *this,r );
            return out;
         }  

      SparseRaster<T> op_erfc() const {
            const auto n = V;
            const auto r = n.apply([](T n) -> T { return std::erfc(n); });

            KiLib::Rasters::SparseRaster<T> out (*this, r);
            return out;
         }  


    bool operator==(const SparseRaster<T>& rhs) const {
        if ( ! can_perform_operation(rhs) ) return false;

         if ( ! std::equal(std::begin(this->V), std::end(this->V), std::begin(rhs.V) )) return false;
         if ( ! std::equal(std::begin(this->COL_INDEX), std::end(this->COL_INDEX), std::begin(rhs.COL_INDEX) )) return false;
         if ( ! std::equal(std::begin(this->ROW_INDEX), std::end(this->ROW_INDEX), std::begin(rhs.ROW_INDEX) )) return false;

        return true;

    }

template <typename C> SparseRaster<T> operator*(const C k ) const  
{

      // Get nodata values
      auto nd = V == this->get_nodata_value();
      KiLib::Rasters::SparseRaster<T> out(*this, V * k);
      out.V[nd] = this->get_nodata_value();
      return out;
}

template <typename C> SparseRaster<T> operator+(const C k ) const  
{
      auto nd = V == this->get_nodata_value();
    KiLib::Rasters::SparseRaster<T> out(*this, V + k);
      out.V[nd] = this->get_nodata_value();
   return out;
}


template <typename C> SparseRaster<T> operator-(const C k ) const  
{
     auto ad = (V == this->get_nodata_value());
    KiLib::Rasters::SparseRaster<T> out(*this, V - k);
    out.V[ad] = this->get_nodata_value();
   return out;
}


SparseRaster<T> operator*( const SparseRaster<T>& b) const 
    {
    if ( ! can_perform_operation(b) ) {
         return ApplyOperator(*this, b, OPERAND::MULTIPLY);
    }

     auto ad = (V == this->get_nodata_value());
    auto bd = (b.V == this->get_nodata_value());

      KiLib::Rasters::SparseRaster<T> out(b, V * b.V);
    out.set_name(this->get_name() + " * " + b.get_name());
    out.V[ad] = this->get_nodata_value();
    out.V[bd] = this->get_nodata_value();


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
    auto ad = (V == this->get_nodata_value());
    auto bd = (b.V == this->get_nodata_value());
 
   KiLib::Rasters::SparseRaster<T> out(*this, V / b.V);
    out.set_name(this->get_name() + " / " + b.get_name());
    out.V[ad] = this->get_nodata_value();
    out.V[bd] = this->get_nodata_value();

   return out;
}

SparseRaster<T> operator+(const SparseRaster<T>& b )
{
    if ( ! can_perform_operation(b) ) {
        throw std::invalid_argument("Rasters cannot be added because they aren't the same dimensions");
    }
    auto ad = (V == this->get_nodata_value());
    auto bd = (b.V == this->get_nodata_value());
   SparseRaster<T> out(*this, V + b.V);
    out.set_name(this->get_name() + " + " + b.get_name());
    out.V[ad] = this->get_nodata_value();
    out.V[bd] = this->get_nodata_value();
   return out;
}

SparseRaster<T> operator-(const SparseRaster<T>& b )
{
    if ( ! can_perform_operation(b) ) {
        throw std::invalid_argument("Rasters cannot be subtracted because they aren't the same dimensions");
    }
    auto ad = (V == this->get_nodata_value());
    auto bd = (b.V == this->get_nodata_value());

   SparseRaster<T> out(*this, V - b.V);
    out.set_name(this->get_name() + " - " + b.get_name());
    out.V[ad] = this->get_nodata_value();
    out.V[bd] = this->get_nodata_value();

   return out;
}
        friend KiLib::Rasters::SparseRaster<T> std::clamp( const KiLib::Rasters::SparseRaster<T>&  ,const T&, const T&);



   private:
      size_t           nnz;
      T                default_value;
      std::valarray<T> V;
      std::vector<size_t> COL_INDEX, ROW_INDEX, Z_INDEX;

      bool is_valid_cell( size_t i, size_t j, size_t k ) const override
      {
        return _get_data_index(i,j,k).has_value();
      }

         T get_data(size_t i, size_t j, size_t k) const override {
            return V[_get_data_index(i,j,k).value()];
         }

    std::optional<T> _get_data_index(size_t i, size_t j, size_t k) const  {

         // Bounds check
         if ( i >= this->rows || j >= this->cols || k >= this->zindex ) return {};

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
               size_t zindex_count = 0;
               for( ; (row_start < row_end) && (zindex_count < this->zindex); row_start++, zindex_count++ ) {
                  if ( Z_INDEX[row_start] == k ){
                     return row_start;
                  }
               }  
               return {};
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
         //if ( this->get_xllcorner() != other.get_xllcorner() ) return false;
         //if ( this->get_yllcorner() != other.get_yllcorner() ) return false;
         if ( this->get_height() != other.get_height() ) return false;
         if ( this->get_width() != other.get_width() ) return false;
         if ( this->get_cellsize() != other.get_cellsize() ) return false;
         if ( this->get_rows() != other.get_rows() ) return false;
         if ( this->get_cols() != other.get_cols() ) return false;
         if ( this->get_zindex() != other.get_zindex() ) return false;

        return true;

       }

      enum class OPERAND
      {
         MULTIPLY = 0,
         DIVIDE,
         PLUS,
         MINUS
      };

      SparseRaster<T> ApplyOperator( const SparseRaster<T>& a, const SparseRaster<T>& b, OPERAND op ) const
      {
         auto* op1     = &a;
         auto* op2     = &b;
         bool  swapped = false;

         // Find the bigger, more element matrix.
         if ( a.get_ndata() < b.get_ndata() )
         {
            op1     = &b;
            op2     = &a;
            swapped = true;
         }

         std::map<std::tuple<size_t,size_t, size_t>, double> values;

         // Need to loop through each cell in the larger raster.
         for ( size_t r = 0; r < op1->get_rows(); r++ )
         {
            for ( size_t c = 0; c < op1->get_cols(); c++ )
            {
               for ( size_t zindex = 0; zindex < op1->get_zindex(); zindex++) {

                  const auto& cell_a = op1->get( r, c, zindex);

                  if ( cell_a.is_nodata || std::isnan( *( cell_a.data ) ) || std::isinf( *( cell_a.data ) ) )
                  {
                     continue;
                  }

                  // Use the x,y,z coordinates to get the proper cell.
                  const auto& cell_b = op2->get( (double) cell_a.x(), (double) cell_a.y(), zindex );

                  if ( cell_b.is_nodata || std::isnan( *( cell_b.data ) ) || std::isinf( *( cell_b.data ) ) )
                  {
                     continue;
                  }

                  double val = 0;

                  switch ( op )
                  {
                  case OPERAND::MULTIPLY:
                     val = *( cell_a.data ) * *( cell_b.data );
                     break;
                  case OPERAND::DIVIDE:
                     if ( !swapped )
                     {
                        val = *( cell_a.data ) / *( cell_b.data );
                     }
                     else
                     {
                        val = *( cell_b.data ) / *( cell_a.data );
                     }
                     break;
                  case OPERAND::PLUS:
                     val = *( cell_a.data ) + *( cell_b.data );
                     break;
                  case OPERAND::MINUS:
                     if ( !swapped )
                     {
                        val = *( cell_a.data ) - *( cell_b.data );
                     }
                     else
                     {
                        val = *( cell_b.data ) - *( cell_a.data );
                     }
                     break;
                  default:
                     throw std::invalid_argument( "ApplyOperator: Unknown OPERAND" );
                  };

                  values.insert({{r,c,zindex}, val});
               }
            }
         }

         SparseRaster<T> out( std::make_tuple( op1->get_rows(), op1->get_cols(), op1->get_zindex() ), values);

         out.set_xllcorner( op1->get_xllcorner() );
         out.set_yllcorner( op1->get_yllcorner() );
         out.set_cellsize( op1->get_cellsize() );
         out.set_nodata_value( op1->get_nodata_value() );
         out.set_width( op1->get_width() );
         out.set_height( op1->get_height() );


      return out;
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
};
