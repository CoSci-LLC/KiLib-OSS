#pragma once

#include <KiLib/Rasters/IRaster.hpp>
#include <algorithm>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <stddef.h>
#include <stdexcept>
#include <valarray>


namespace KiLib::Rasters
{

   struct Default
   {
      double value;
      bool   is_nodata;
   };


   template <typename T> class Raster : public IDirectAccessRaster<T>
   {
   public:
      Raster( size_t rows, size_t cols ) : data( rows * cols ), nodata_mask( rows * cols, true )
      {

         // Initialize the base class variables
         this->rows = rows;
         this->cols = cols;

         // Reserve the data sizes
         data.resize( rows * cols );
         nodata_mask.resize( rows * cols );
         nodata_mask.reserve( rows * cols );

         nnz = rows * cols;
      }

      using IRaster<T>::get;
      KiLib::Rasters::Cell<T> get( size_t i, size_t j ) const override
      {
         // Check out of bounds
         unsigned idx = i * this->cols + j;
         if ( ( idx > this->cols * this->rows ) || data[idx] == this->get_nodata_value() || nodata_mask[idx] == true )
         {
            return KiLib::Rasters::Cell<T>( *this, i, j );
         }

         return KiLib::Rasters::Cell<T>( *this, i, j, data[idx] );
      }

      using IRaster<T>::set;
      void set( size_t i, size_t j, const T& value ) override
      {
         data[i * this->cols + j] = value;

         if ( value == this->get_nodata_value() )
         {
            nodata_mask[i * this->cols + j] = true;
         }
         else
         {
            nodata_mask[i * this->cols + j] = false;
         }
      }

      bool is_valid_cell( double x, double y ) const
      {
         double rF = ( y - this->yllcorner ) / this->cellsize;
         double cF = ( x - this->xllcorner ) / this->cellsize;

         size_t r = std::clamp<size_t>( (size_t)std::floor( rF ), 0UL, this->get_rows() - 1 );
         size_t c = std::clamp<size_t>( (size_t)std::floor( cF ), 0UL, this->get_cols() - 1 );

         return is_valid_cell( r, c );
      }

      size_t get_ndata() const override
      {
         return this->nnz;
      }

      struct Iterator
      {
         // Always start from the beginning
         Iterator( const Raster<T>& raster, typename std::map<std::pair<int, int>, T*>::const_iterator it )
            : raster( raster ), it( it ){

                                };


         // incremeneting means going through the list
         Iterator& operator++()
         {
            it++;
            return *this;
         }

         // post fixing is bad in general but is has it's usages
         Iterator operator++( int )
         {
            return ++it;
         }


         // we need to be able to compare nodes
         bool operator!=( const Iterator& other ) const
         {
            return it != other.it;
         }


         // return the data from the node (dereference operator)
         KiLib::Rasters::Cell<T> operator*() const
         {
            // first = the pair
            // first/second = the coordinates inside the pair
            return raster.get( (size_t)( *it ).first.first, (size_t)( *it ).first.second );
         }

      private:
         const Raster<T>&                                           raster;
         typename std::map<std::pair<int, int>, T*>::const_iterator it;
      };

      // returning a const pointer to the front
      Iterator begin() const noexcept
      {
         return Iterator( *this, data.begin() );
      };

      // returning a const pointer to the back - the back is always null because it marks the end of the list
      Iterator end() const noexcept
      {
         return Iterator( *this, data.end() );
      };

      operator std::valarray<T>() const
      {
         return data;
      }
      /*   Raster<T> operator*(const Raster<T>& r) const {

             const auto result = (std::valarray<T>)r * data;
             Raster<T> new_raster(r, result);
             return new_raster;

         }*/


      Raster( const Raster<T>& other ) : Raster( other, (std::valarray<T>)other )
      {
      }

      Raster( const Raster<T>& other, const std::valarray<T>& d )
      {
         this->nnz  = other.get_ndata();
         this->rows = other.get_rows();
         this->cols = other.get_cols();

         this->set_xllcorner( other.get_xllcorner() );
         this->set_yllcorner( other.get_yllcorner() );
         this->set_cellsize( other.get_cellsize() );
         this->set_nodata_value( other.get_nodata_value() );
         this->set_width( other.get_width() );
         this->set_height( other.get_height() );

         this->data = std::valarray<double>( &d[0], d.size() );
         this->nodata_mask.resize( this->nnz );
         std::copy( other.nodata_mask.begin(), other.nodata_mask.end(), this->nodata_mask.begin() );
      }

      Raster<T>& operator=( const Raster<T>& other )
      {
         this->CopyFrom( other );
         return *this;
      }

      void CopyFrom( const Raster<T>& other )
      {
         this->nnz  = other.get_ndata();
         this->rows = other.get_rows();
         this->cols = other.get_cols();

         this->set_xllcorner( other.get_xllcorner() );
         this->set_yllcorner( other.get_yllcorner() );
         this->set_cellsize( other.get_cellsize() );
         this->set_nodata_value( other.get_nodata_value() );
         this->set_width( other.get_width() );
         this->set_height( other.get_height() );

         this->data.resize( this->nnz );
         this->nodata_mask.resize( this->nnz );
         std::copy( other.nodata_mask.begin(), other.nodata_mask.end(), this->nodata_mask.begin() );

         this->data = std::valarray<double>( &other.data[0], this->nnz );
      }


      /**
       * Element by element multiplication. Returns a Raster class (may be different that what was provided)
       */
      Raster<T> operator*( const Raster<T>& other ) const
      {
         return ApplyOperator( *this, other, Raster::OPERAND::MULTIPLY );
      }

      Raster<T> operator*( const T val ) const
      {
         return ApplyOperator( *this, val, Raster::OPERAND::MULTIPLY );
      }

      Raster<T> operator+( const Raster<T>& other ) const
      {
         return ApplyOperator( *this, other, Raster::OPERAND::PLUS );
      }

      Raster<T> operator-( const Raster<T>& other ) const
      {
         return ApplyOperator( *this, other, Raster::OPERAND::MINUS );
      }

      Raster<T> operator/( const Raster<T>& other ) const
      {
         return ApplyOperator( *this, other, Raster::OPERAND::DIVIDE );
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

      const T* GetUnderlyingDataArray() const override
      {
         return &( data[0] );
      }


   private:
      size_t            nnz;
      std::valarray<T>  data;
      std::vector<bool> nodata_mask;

      enum class OPERAND
      {
         MULTIPLY = 0,
         DIVIDE,
         PLUS,
         MINUS
      };

      Raster<T> ApplyOperator( const Raster<T>& a, const Raster<T>& b, OPERAND op ) const
      {
         // Either use the index to multiply each element, or if we don't have the same kind of rasters
         // we need to multiply by the location, which is slower
         if ( a.get_rows() == b.get_rows() && a.get_cols() == b.get_cols() )
         {

            std::valarray<T> result;
            switch ( op )
            {
            case OPERAND::MULTIPLY:
               result = (std::valarray<T>)a * (std::valarray<T>)b;
               break;
            case OPERAND::DIVIDE:
               result = (std::valarray<T>)a / (std::valarray<T>)b;
               break;
            case OPERAND::PLUS:
               result = (std::valarray<T>)a + (std::valarray<T>)b;
               break;
            case OPERAND::MINUS:
               result = (std::valarray<T>)a - (std::valarray<T>)b;
               break;
            default:
               throw std::invalid_argument( "ApplyOperator: Unknown OPERAND" );
            };

            std::for_each(
               std::begin( result ), std::end( result ),
               [&a]( T val )
               {
                  if ( std::isnan( val ) || std::isinf( val ) )
                  {
                     return a.get_nodata_value();
                  }
                  return val;
               } );


            Raster<T> out( a, result );
            for ( size_t i; i < a.get_ndata(); i++ )
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
            Raster<T> out( *op1 );

            // Need to loop through each cell in the larger raster.
            for ( size_t r = 0; r < op1->get_rows(); r++ )
            {
               for ( size_t c = 0; c < op1->get_cols(); c++ )
               {
                  const auto& cell_a = op1->get( r, c );

                  if ( cell_a.is_nodata || std::isnan( *( cell_a.data ) ) || std::isinf( *( cell_a.data ) ) )
                  {
                     out.set( r, c, out.get_nodata_value() );
                     continue;
                  }

                  // Use the x,y coordinates to get the proper cell.
                  const auto& cell_b = op2->get( cell_a.x(), cell_a.y() );

                  double val = 0;

                  if ( cell_b.is_nodata || std::isnan( *( cell_b.data ) ) || std::isinf( *( cell_b.data ) ) )
                  {
                     out.set( r, c, out.get_nodata_value() );
                     continue;
                  }

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

                  out.set( r, c, val );
               }
            }

            return out;
         }
      }

      Raster<T> ApplyOperator( const Raster<T>& a, const T b, OPERAND op ) const
      {
         // Either use the index to multiply each element, or if we don't have the same kind of rasters
         // we need to multiply by the location, which is slower


         std::valarray<T> result;
         switch ( op )
         {
         case OPERAND::MULTIPLY:
            result = (std::valarray<T>)a * b;
            break;
         case OPERAND::DIVIDE:
            result = (std::valarray<T>)a / b;
            break;
         case OPERAND::PLUS:
            result = (std::valarray<T>)a + b;
            break;
         case OPERAND::MINUS:
            result = (std::valarray<T>)a - b;
            break;
         default:
            throw std::invalid_argument( "ApplyOperator: Unknown OPERAND" );
         };

         Raster<T> out( a, result );


         return out;
      }
   };

} // namespace KiLib::Rasters


namespace std
{
   template <class T> KiLib::Rasters::Raster<T> atan( const KiLib::Rasters::Raster<T>& a )
   {
      std::valarray<T>          result = std::atan( (std::valarray<T>)a );
      KiLib::Rasters::Raster<T> out( a, result );
      return out;
   }

   template <class T> KiLib::Rasters::Raster<T> sin( const KiLib::Rasters::Raster<T>& a )
   {
      std::valarray<T>          result = std::sin( (std::valarray<T>)a );
      KiLib::Rasters::Raster<T> out( a, result );
      return out;
   }

   template <class T> KiLib::Rasters::Raster<T> cos( const KiLib::Rasters::Raster<T>& a )
   {
      std::valarray<T>          result = std::cos( (std::valarray<T>)a );
      KiLib::Rasters::Raster<T> out( a, result );
      return out;
   }

   template <class T> KiLib::Rasters::Raster<T> tan( const KiLib::Rasters::Raster<T>& a )
   {
      std::valarray<T>          result = std::tan( (std::valarray<T>)a );
      KiLib::Rasters::Raster<T> out( a, result );
      return out;
   }


} // namespace std

// This is the (scalar * Raster) operator
template <class T, typename C> KiLib::Rasters::Raster<T> operator*( const C k, const KiLib::Rasters::Raster<T>& a )
{
   std::valarray<T>          result = (std::valarray<T>)a * k;
   KiLib::Rasters::Raster<T> out( a, result );
   return out;
}


template <class T, typename C> KiLib::Rasters::Raster<T> operator-( const C k, const KiLib::Rasters::Raster<T>& a )
{
   std::valarray<T>          result = k - (std::valarray<T>)a;
   KiLib::Rasters::Raster<T> out( a, result );
   return out;
}

template <class T> KiLib::Rasters::Raster<T> operator-( const KiLib::Rasters::Raster<T>& a )
{
   std::valarray<T>          result = -1 * (std::valarray<T>)a;
   KiLib::Rasters::Raster<T> out( a, result );
   return out;
}
