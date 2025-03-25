#pragma once

#include "IRaster.hpp"
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
#include <ranges>


namespace KiLib::Rasters
{

   struct Default
   {
      double value;
      bool   is_nodata;
   };


   template <typename T> class Raster : virtual public IDirectAccessRaster<T>, virtual public IValArrayRaster<T>
   {
   public:

      // Just call the other constructor with the zindex = 1
      Raster( size_t rows, size_t cols) : Raster( std::make_tuple(rows, cols, 1))
      {
      }


      Raster( size_t rows, size_t cols, double init_val) : Raster( std::make_tuple(rows, cols, 1) , init_val) {}

      
      
      Raster ( const std::tuple<size_t, size_t, size_t>& dims ) : data(std::get<0>(dims) * std::get<1>(dims) * std::get<2>(dims)), nodata_mask( std::get<0>(dims) * std::get<1>(dims) * std::get<2>(dims) , true)
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

      Raster (const std::tuple<size_t, size_t, size_t>& dims, T init_val ) : Raster(dims) 
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

      size_t get_valid_cell_count() const {
         return std::count(nodata_mask.begin(), nodata_mask.end(), false);
      }

      std::string to_string() const {
         std::stringstream ss; 
         
         ss << fmt::format( "Raster {}\n", this->name);
         ss << fmt::format( "  {} valid cells out of {} ({:.2f}% valid)\n", get_valid_cell_count(), this->nnz, 100.0 * get_valid_cell_count() / this->nnz);

         return ss.str();
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

      Raster( const Raster<T>& other, double d ) : nodata_mask(other.get_ndata(), false)
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
      }
      Raster( const Raster<T>& other, const std::valarray<T>& d )
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

      Raster<T> operator/( const T val ) const
      {
         return ApplyOperator( *this, val, Raster::OPERAND::DIVIDE );
      }

      bool operator==(const Raster<T>& rhs) const {
         
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
            if (*a != *b) return false;
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

      const T* GetUnderlyingDataArray() const override
      {
         return &( data[0] );
      }

      const std::valarray<T>& get_valarray() const override
   {
      return data;
   }


      T min() const {
         T m = std::numeric_limits<double>::max();
         for( size_t i = 0; i < nnz; i++ ) {
            if ( nodata_mask[i] == false ) 
               m = std::min(m, data[i]);
         }
         return m;
      }

      T max() const {
         T m = std::numeric_limits<double>::min();
         for( size_t i = 0; i < nnz; i++ ) {
            if ( nodata_mask[i] == false ) 
               m = std::max(m, data[i]);
         }
         return m;
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
         if ( a.get_rows() == b.get_rows() && a.get_cols() == b.get_cols() && a.get_zindex() == b.get_zindex() )
         {

         Raster<T> out( std::make_tuple( a.get_rows(), a.get_cols(), a.get_zindex() ));


            out.set_xllcorner( a.get_xllcorner() );
            out.set_yllcorner( a.get_yllcorner() );
            out.set_cellsize( a.get_cellsize() );
            out.set_nodata_value( a.get_nodata_value() );
            out.set_width( a.get_width() );
            out.set_height( a.get_height() );

            switch ( op )
            {
            case OPERAND::MULTIPLY:
               out.data = (std::valarray<T>)a * (std::valarray<T>)b;
               break;
            case OPERAND::DIVIDE:
               out.data = (std::valarray<T>)a / (std::valarray<T>)b;
               break;
            case OPERAND::PLUS:
               out.data = (std::valarray<T>)a + (std::valarray<T>)b;
               break;
            case OPERAND::MINUS:
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

         Raster<T> out( std::make_tuple( op1->get_rows(), op1->get_cols(), op1->get_zindex() ));

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

                     out.set( r, c, zindex, val );
                  }
               }
            }

            return out;
         }
      }

      Raster<T> ApplyOperator( const Raster<T>& a, const T b, OPERAND op ) const
      {
         // Either use the index to multiply each element, or if we don't have the same kind of rasters
         // we need to multiply by the location, which is slower

         Raster<T> out( std::make_tuple( a.get_rows(), a.get_cols(), a.get_zindex() ));

         out.set_xllcorner( a.get_xllcorner() );
         out.set_yllcorner( a.get_yllcorner() );
         out.set_cellsize( a.get_cellsize() );
         out.set_nodata_value( a.get_nodata_value() );
         out.set_width( a.get_width() );
         out.set_height( a.get_height() );

         switch ( op )
         {
         case OPERAND::MULTIPLY:
            out.data = (std::valarray<T>)a * b;
            break;
         case OPERAND::DIVIDE:
            out.data = (std::valarray<T>)a / b;
            break;
         case OPERAND::PLUS:
            out.data = (std::valarray<T>)a + b;
            break;
         case OPERAND::MINUS:
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


namespace std
{
// template <class T> T max( const KiLib::Rasters::Raster<T>& a )
// {
//    //std::valarray<T>          result = std::max( (std::valarray<T>)a );
//    //KiLib::Rasters::Raster<T> out( a, result );
//    auto in = (std::valarray<T>)a;
//    return *std::max_element(std::begin(a), std::end(a));
// }

//   template <class T> KiLib::Rasters::Raster<T> min( const KiLib::Rasters::Raster<T>& a )
//   {
//      std::valarray<T>          result = std::min( (std::valarray<T>)a );
//      KiLib::Rasters::Raster<T> out( a, result );
//      return out;
//   }

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
   template <class T> KiLib::Rasters::Raster<T> exp( const KiLib::Rasters::Raster<T>& a )
   {
      std::valarray<T>          result = std::exp( (std::valarray<T>)a );
      KiLib::Rasters::Raster<T> out( a, result );
      return out;
   }
   template <class T> T min( const KiLib::Rasters::Raster<T>& a )
   {
      return a.min();
   }

   template <class T> T max( const KiLib::Rasters::Raster<T>& a )
   {
      return a.max();
   }

   template <class T> KiLib::Rasters::Raster<T> clamp( const KiLib::Rasters::Raster<T>& a, const T& lo, const T& hi)
   {
/*      const auto va = (std::valarray<T>)a;
      std::vector<double> v;
      v.assign(std::begin(va), std::end(va));
      std::for_each(std::begin(v), std::end(v), [&lo, &hi](T& v) { return std::clamp(v, lo, hi); });
      KiLib::Rasters::Raster<T> out( a, std::valarray<double>(v.data(), v.size()) );*/


      auto in = (std::valarray<T>)a;
      std::transform(std::begin(in), std::end(in), std::begin(in), [&lo, &hi](T& v) { return std::clamp(v, lo, hi); });
      KiLib::Rasters::Raster<T> out( a, in );
      return out;
   }

   template <class T> std::valarray<T> erfc( const std::valarray<T>& a) {
      return a.apply([](T n) -> T { return std::erfc(n); });
   }

   template <class T> KiLib::Rasters::Raster<T> ierfc ( const KiLib::Rasters::Raster<T>& a) {
      const auto n = (std::valarray<T>)a;
      const auto r = 1 / std::sqrt(M_PI) * std::exp( -1 * std::pow(n, 2)) - n * std::erfc(n);
      KiLib::Rasters::Raster<T> out( a, r );
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

template <class T, typename C> KiLib::Rasters::Raster<T> operator/( const C k, const KiLib::Rasters::Raster<T>& a )
{
   std::valarray<T>          result = k / (std::valarray<T>)a;
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
