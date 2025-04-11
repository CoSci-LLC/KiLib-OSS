#pragma once

#include "IRaster.hpp"
#include <KiLib/Rasters/IRaster.hpp>
#include <algorithm>
#include <functional>
#include <iostream>
#include <map>
#include <memory>

#include <stddef.h>
#include <stdexcept>
#include <tuple>
#include <valarray>




namespace KiLib::Rasters
{

   struct Default
   {
      double value;
      bool   is_nodata;
   };


   template <typename T> class DenseRaster : public IRaster<T>
   {
   public:

      // Just call the other constructor with the zindex = 1
      DenseRaster( size_t rows, size_t cols) : DenseRaster( std::make_tuple(rows, cols, 1))
      {
      }


      DenseRaster( size_t rows, size_t cols, double init_val) : DenseRaster( std::make_tuple(rows, cols, 1) , init_val) {}

      
      
      DenseRaster ( const std::tuple<size_t, size_t, size_t>& dims ) : data(std::get<0>(dims) * std::get<1>(dims) * std::get<2>(dims)), nodata_mask( std::get<0>(dims) * std::get<1>(dims) * std::get<2>(dims) , true)
      {
          // Initialize the base class variables
         
         const auto rows = std::get<0>(dims);
         const auto cols = std::get<1>(dims);
         const auto zindex = std::get<2>(dims);

         this->rows = rows;
        this->cols = cols;
         this->zindex = zindex; 

         // Reserve the data sizes
         data.resize( rows * cols * zindex);
         nodata_mask.resize( rows * cols * zindex);
         nodata_mask.reserve( rows * cols * zindex);

         nnz = rows * cols * zindex;  
      }

      DenseRaster (const std::tuple<size_t, size_t, size_t>& dims, T init_val ) : DenseRaster(dims) 
      {
         for ( size_t i = 0; i < nnz; i++) {
            nodata_mask[i] = false;
            data[i] = init_val;
         }
      }

      using IRaster<T>::get;
      KiLib::Rasters::Cell<T> get( size_t i, size_t j, size_t k = 0 ) const override
      {
   
         // Check out of bounds
         unsigned idx = (this->rows * this->cols * k) + (this->cols * i) + j;
         if (  (i >= this->rows) || (j >= this->cols) || (k >= this->zindex)  || ( idx > this->cols * this->rows * this->zindex ) || data[idx] == this->get_nodata_value() || nodata_mask[idx] == true )
         {
            return KiLib::Rasters::Cell<T>( *this, i, j);
         }

         return KiLib::Rasters::Cell<T>( *this, i, j, data[idx] );
      }

      size_t get_valid_cell_count() const override {
         return std::count(nodata_mask.begin(), nodata_mask.end(), false);
      }

      using IRaster<T>::set;
      void set( size_t i, size_t j, size_t k, const T& value ) override
      {
         unsigned idx = (this->rows * this->cols * k) + (this->cols * i) + j;
         
         if ( idx >= this->nnz ) throw std::invalid_argument("Cannot set field due to attempt to set value of index.");

         data[idx] = value;
         nodata_mask[idx] = value == this->get_nodata_value();
      }

      size_t get_ndata() const override
      {
         return this->nnz;
      }


      operator std::valarray<T>() const
      {
         return data;
      }
      /*   Raster<T> operator*(const Raster<T>& r) const {

             const auto result = (std::valarray<T>)r * data;
             Raster<T> new_raster(r, result);
             return new_raster;

        }*/

      DenseRaster( const DenseRaster<T>& other ) : DenseRaster( other, (std::valarray<T>)other )
      {
      }

      DenseRaster( const DenseRaster<T>& other, double d ) 
      {
         this->nnz  = other.get_ndata();
         this->rows = other.get_rows();
         this->cols = other.get_cols();
         this->zindex = other.get_zindex();

         this->set_xllcorner( other.get_xllcorner() );
         this->set_yllcorner( other.get_yllcorner() );
         this->set_cellsize( other.get_cellsize() );
         this->set_nodata_value( other.get_nodata_value() );
         this->set_width( other.get_width() );
         this->set_height( other.get_height() );
         this->set_name( other.get_name() );

         this->data = std::valarray<double>( d, this->nnz );
         this->nodata_mask.resize( this->nnz );
         std::copy( other.nodata_mask.begin(), other.nodata_mask.end(), this->nodata_mask.begin() );
      }
      DenseRaster( const DenseRaster<T>& other, const std::valarray<T>& d )
      {
         this->nnz  = other.get_ndata();
         this->rows = other.get_rows();
         this->cols = other.get_cols();
         this->zindex = other.get_zindex();

         this->set_xllcorner( other.get_xllcorner() );
         this->set_yllcorner( other.get_yllcorner() );
         this->set_cellsize( other.get_cellsize() );
         this->set_nodata_value( other.get_nodata_value() );
         this->set_width( other.get_width() );
         this->set_height( other.get_height() );
         this->set_name( other.get_name() );

         this->data = std::valarray<double>( &d[0], d.size() );
         this->nodata_mask.resize( this->nnz );
         std::copy( other.nodata_mask.begin(), other.nodata_mask.end(), this->nodata_mask.begin() );
      }

      DenseRaster<T>& operator=( const DenseRaster<T>& other )
      {
         this->CopyFrom( other );
         return *this;
      }

      void CopyFrom( const DenseRaster<T>& other )
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

         this->data.resize( this->nnz );
         this->nodata_mask.resize( this->nnz );
         std::copy( other.nodata_mask.begin(), other.nodata_mask.end(), this->nodata_mask.begin() );

         this->data = std::valarray<double>( &other.data[0], this->nnz );
      }

      TYPE get_type() const override { return TYPE::DENSE; }

      /**
       * Element by element multiplication. Returns a Raster class (may be different that what was provided)
       */
      DenseRaster<T> operator*( const DenseRaster<T>& other ) const 
      {
         return ApplyOperator( *this, other, IRaster<T>::OPERAND::MULTIPLY );
      }

      DenseRaster<T> operator*( const T val ) const 
      {
         return ApplyOperator( *this, val, IRaster<T>::OPERAND::MULTIPLY );
      }

      DenseRaster<T> operator+( const DenseRaster<T>& other ) const
      {
         return ApplyOperator( *this, other, IRaster<T>::OPERAND::PLUS );
      }
      DenseRaster<T> operator+( const T val ) const
      {
         return ApplyOperator( *this, val, IRaster<T>::OPERAND::PLUS );
      }
      DenseRaster<T> operator-( const DenseRaster<T>& other ) const
      {
         return ApplyOperator( *this, other, IRaster<T>::OPERAND::MINUS );
      }
      DenseRaster<T> operator-( const T val ) const
      {
         return ApplyOperator( *this, val, IRaster<T>::OPERAND::MINUS );
      }
      DenseRaster<T> operator/( const DenseRaster<T>& other ) const
      {
         return ApplyOperator( *this, other, IRaster<T>::OPERAND::DIVIDE );
      }

      DenseRaster<T> operator/( const T val ) const
      {
         return ApplyOperator( *this, val, IRaster<T>::OPERAND::DIVIDE );
      }

      bool operator==(const DenseRaster<T>& rhs) const {
         
         // Shortcut true
         if ( this == &rhs ) return true;

         // Check each property
         if ( this->get_xllcorner() != rhs.get_xllcorner() ) return false;
         if ( this->get_yllcorner() != rhs.get_yllcorner() ) return false;
         if ( this->get_height() != rhs.get_height() ) return false;
         if ( this->get_width() != rhs.get_width() ) return false;
         if ( this->get_nodata_value() != rhs.get_nodata_value() ) return false;
         if ( this->get_cellsize() != rhs.get_cellsize() ) return false;
         if ( this->get_rows() != rhs.get_rows() ) return false;
         if ( this->get_cols() != rhs.get_cols() ) return false;
         if ( this->get_zindex() != rhs.get_zindex() ) return false;

         // Check data now, but needs to respect the nodata attributes.
         if ( ! std::equal(std::begin(this->nodata_mask), std::end(this->nodata_mask), std::begin(rhs.nodata_mask) )) return false;

         // Check each value in the data. We don't need to be careful about bounds because
         // of all the checks above. If we aren't good, then a new feature was added and
         // someone (probably me) didn't check here.
         for (auto a = this->begin(), b = rhs.begin(); a != this->end(); ++a, ++b) {
            if ( *(&a).data != *(&b).data) return false;
         } 


         return true;


      }

      void ChangeInftoNoData()
      {
         auto nodata_value = this->get_nodata_value();
         std::for_each(
            std::begin( data ), std::end( data ),
            [&nodata_value]( T val )
            {
               if ( std::isnan( val ) || std::isinf( val ) )
               {
                  return nodata_value;
               }
               return val;
            } );
      }

      T min() const override {
         T m = std::numeric_limits<double>::max();
         for( size_t i = 0; i < nnz; i++ ) {
            if ( nodata_mask[i] == false ) 
               m = std::min(m, data[i]);
         }
         return m;
      }

      T max() const override {
         T m = std::numeric_limits<double>::min();
         for( size_t i = 0; i < nnz; i++ ) {
            if ( nodata_mask[i] == false ) 
               m = std::max(m, data[i]);
         }
         return m;
      }

      DenseRaster<T> op_divide(const T val) const {
         return ApplyOperator( val, *this, IRaster<T>::OPERAND::DIVIDE);
         }  


      DenseRaster<T> op_minus(const T val) const {
         return ApplyOperator( val, *this, IRaster<T>::OPERAND::MINUS);
         }  

      DenseRaster<T> op_ierfc() const {
            const auto n = data;
            const auto r = 1 / std::sqrt(M_PI) * std::exp( -1 * std::pow(n, 2)) - n * std::erfc(n);
            KiLib::Rasters::DenseRaster<T> out( *this,r );
            return out;
         }  

      DenseRaster<T> op_erfc() const {
            const auto n = data;

            return n.apply([](T n) -> T { return std::erfc(n); });
         }  


   private:
      size_t            nnz;
      std::valarray<T>  data;
      std::vector<bool> nodata_mask;

         bool is_valid_cell(size_t i, size_t j, size_t k) const override  {
            
               return !nodata_mask[this->flatten_index(i, j, k)];
         }
         
         T get_data(size_t i, size_t j, size_t k) const override {
return data[this->flatten_index(i,j,k)];
         }

      DenseRaster<T> ApplyOperator( const DenseRaster<T>& a, const DenseRaster<T>& b, typename IRaster<T>::OPERAND op ) const
      {
         // Either use the index to multiply each element, or if we don't have the same kind of rasters
         // we need to multiply by the location, which is slower
         if ( a.get_rows() == b.get_rows() && a.get_cols() == b.get_cols() && a.get_zindex() == b.get_zindex() )
         {

            DenseRaster<T> out( std::make_tuple( a.get_rows(), a.get_cols(), a.get_zindex() ));
   
            out.copy_metadata_from(a);

            switch ( op )
            {
            case IRaster<T>::OPERAND::MULTIPLY:
               out.data = (std::valarray<T>)a * (std::valarray<T>)b;
               break;
            case IRaster<T>::OPERAND::DIVIDE:
               out.data = (std::valarray<T>)a / (std::valarray<T>)b;
               break;
            case IRaster<T>::OPERAND::PLUS:
               out.data = (std::valarray<T>)a + (std::valarray<T>)b;
               break;
            case IRaster<T>::OPERAND::MINUS:
               out.data = (std::valarray<T>)a - (std::valarray<T>)b;
               break;
            default:
               throw std::invalid_argument( "ApplyOperator: Unknown OPERAND" );
            };

            for ( size_t i = 0; i < a.get_ndata(); i++ )
            {
               out.nodata_mask[i] = a.nodata_mask[i] || b.nodata_mask[i];
            }
            return out;
         }
         else // Multiply by rasters
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

         DenseRaster<T> out( std::make_tuple( op1->get_rows(), op1->get_cols(), op1->get_zindex() ));

            out.set_xllcorner( op1->get_xllcorner() );
            out.set_yllcorner( op1->get_yllcorner() );
            out.set_cellsize( op1->get_cellsize() );
            out.set_nodata_value( op1->get_nodata_value() );
            out.set_width( op1->get_width() );
            out.set_height( op1->get_height() );

            // Need to loop through each cell in the larger raster.
            for ( size_t r = 0; r < op1->get_rows(); r++ )
            {
               for ( size_t c = 0; c < op1->get_cols(); c++ )
               {
                  for ( size_t zindex = 0; zindex < op1->get_zindex(); zindex++) {

                     const auto& cell_a = op1->get( r, c, zindex);

                     if ( cell_a.is_nodata || std::isnan( *( cell_a.data ) ) || std::isinf( *( cell_a.data ) ) )
                     {
                        out.set( r, c, zindex, out.get_nodata_value() );
                        continue;
                     }

                     // Use the x,y,z coordinates to get the proper cell.
                     const auto& cell_b = op2->get( (double) cell_a.x(), (double) cell_a.y(), zindex );

                     if ( cell_b.is_nodata || std::isnan( *( cell_b.data ) ) || std::isinf( *( cell_b.data ) ) )
                     {
                        out.set( r, c, zindex, out.get_nodata_value() );
                        continue;
                     }

                     double val = 0;

                     switch ( op )
                     {
                     case IRaster<T>::OPERAND::MULTIPLY:
                        val = *( cell_a.data ) * *( cell_b.data );
                        break;
                     case IRaster<T>::OPERAND::DIVIDE:
                        if ( !swapped )
                        {
                           val = *( cell_a.data ) / *( cell_b.data );
                        }
                        else
                        {
                           val = *( cell_b.data ) / *( cell_a.data );
                        }
                        break;
                     case IRaster<T>::OPERAND::PLUS:
                        val = *( cell_a.data ) + *( cell_b.data );
                        break;
                     case IRaster<T>::OPERAND::MINUS:
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

                     out.set( r, c, zindex, val );
                  }
               }
            }

            return out;
         }
      }
      DenseRaster<T> ApplyOperator( const T b, const DenseRaster<T>& a, typename IRaster<T>::OPERAND op ) const
      {
         // Either use the index to multiply each element, or if we don't have the same kind of rasters
         // we need to multiply by the location, which is slower

         DenseRaster<T> out( std::make_tuple( a.get_rows(), a.get_cols(), a.get_zindex() ));

         out.set_xllcorner( a.get_xllcorner() );
         out.set_yllcorner( a.get_yllcorner() );
         out.set_cellsize( a.get_cellsize() );
         out.set_nodata_value( a.get_nodata_value() );
         out.set_width( a.get_width() );
         out.set_height( a.get_height() );

         switch ( op )
         {
         case IRaster<T>::OPERAND::MULTIPLY:
            out.data = a.data * b;
            break;
         case IRaster<T>::OPERAND::DIVIDE:
            out.data = b / a.data;
            break;
         case IRaster<T>::OPERAND::PLUS:
            out.data = a.data + b;
            break;
         case IRaster<T>::OPERAND::MINUS:
            out.data = b - a.data;
            break;
         default:
            throw std::invalid_argument( "ApplyOperator: Unknown OPERAND" );
         };

         out.nodata_mask = a.nodata_mask;

         return out;
      }

      DenseRaster<T> ApplyOperator( const DenseRaster<T>& a, const T b, typename IRaster<T>::OPERAND op ) const
      {
         // Either use the index to multiply each element, or if we don't have the same kind of rasters
         // we need to multiply by the location, which is slower

         DenseRaster<T> out( std::make_tuple( a.get_rows(), a.get_cols(), a.get_zindex() ));

         out.set_xllcorner( a.get_xllcorner() );
         out.set_yllcorner( a.get_yllcorner() );
         out.set_cellsize( a.get_cellsize() );
         out.set_nodata_value( a.get_nodata_value() );
         out.set_width( a.get_width() );
         out.set_height( a.get_height() );

         switch ( op )
         {
         case IRaster<T>::OPERAND::MULTIPLY:
            out.data = (std::valarray<T>)a * b;
            break;
         case IRaster<T>::OPERAND::DIVIDE:
            out.data = (std::valarray<T>)a / b;
            break;
         case IRaster<T>::OPERAND::PLUS:
            out.data = (std::valarray<T>)a + b;
            break;
         case IRaster<T>::OPERAND::MINUS:
            out.data = (std::valarray<T>)a - b;
            break;
         default:
            throw std::invalid_argument( "ApplyOperator: Unknown OPERAND" );
         };

         out.nodata_mask = a.nodata_mask;

         return out;
      }
   };

} // namespace KiLib::Rasters


