#pragma once

#include "DenseRaster.hpp"
#include "IRaster.hpp"
#include <KiLib/Rasters/IRaster.hpp>
#include <KiLib/Rasters/SparseRaster.hpp>
#include <KiLib/Rasters/DenseRaster.hpp>
#include <algorithm>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <KiLib/Exceptions/NotImplemented.hpp>
#include <sstream>
#include <stddef.h>
#include <stdexcept>
#include <tuple>
#include <valarray>
#include <ranges>

namespace KiLib::Rasters
{
   /**
    * A proxy class for KiLib Rasters. Enables the use of different kinds of rasters 
    * without having to adjust dependent code
    */ 
   template <typename T> class Raster  : public IRaster<T>
   {
   public:

      // Just call the other constructor with the zindex = 1
      Raster( size_t rows, size_t cols) : Raster( std::make_tuple(rows, cols, 1))
      {
      }

      Raster ( const std::tuple<size_t, size_t, size_t>& dims )
      {
         // This has to initialize a dense raster because we aren't given any other values. 
         raster = new KiLib::Rasters::DenseRaster<T>(dims);

         // This is just a preference of CoSci and has been seen around.
         raster->set_nodata_value(-9999);
      }

      ~Raster() {
         delete raster;
      }

      Raster (const std::tuple<size_t, size_t, size_t>& dims, T init_val ) 
      {
          // This has to initialize a dense raster because we aren't given any other values. 
         raster = new KiLib::Rasters::DenseRaster<T>(dims, init_val);

         // This is just a preference of CoSci and has been seen around.
         raster->set_nodata_value(-9999);
      }

      Raster (const Raster<T>& other ) : Raster(*(other.raster)) {}

      Raster (const IRaster<T>& other ) {
         switch (other.get_type()) {
            case Rasters::TYPE::DENSE:
               raster = new KiLib::Rasters::DenseRaster<T>(dynamic_cast<const Rasters::DenseRaster<T>&>(other) );
               break;
            case Rasters::TYPE::SPARSE:
               raster = new KiLib::Rasters::SparseRaster<T>(dynamic_cast<const KiLib::Rasters::SparseRaster<T>&>(other));
               break;
            default:
               throw NotImplementedException("Other raster types not implemented for this constructor");
         }
      }

      using IRaster<T>::get;
      KiLib::Rasters::Cell<T> get( size_t i, size_t j, size_t k = 0 ) const override
      {
         return raster->get(i, j, k); 
      }


      size_t get_valid_cell_count() const override {
         return raster->get_valid_cell_count(); 
      }
      std::string to_string() const override {
         return raster->to_string();
      }    

      using IRaster<T>::set;
      void set( size_t i, size_t j, size_t k, const T& value ) override
      {
         raster->set(i,j,k,value);
      }

      size_t get_ndata() const override
      {
         return raster->get_ndata();
      }
      
      size_t flatten_index(size_t i, size_t j, size_t k) const override { return raster->flatten_index(i,j,k); }

      /**
       * Copy raster properties, but assign new init value to current cells
       */ 
      Raster( const Raster<T>& other, double init_val )
      {
         switch (other.get_type()) {
            
            case Rasters::TYPE::DENSE:
               raster = new KiLib::Rasters::DenseRaster<T>( { other.get_rows(), other.get_cols(), other.get_zindex() }, init_val );
               break;
            case Rasters::TYPE::SPARSE:
               raster = new KiLib::Rasters::SparseRaster<T>((KiLib::Rasters::SparseRaster<T>)other.raster, init_val);
               break;
            default:
               throw NotImplementedException("Other raster types not implemented for this constructor");
         }
      }

      Raster<T>& operator=( const Raster<T>& other )
      {
         *raster = *(other.raster);
      }

      size_t get_rows() const override { return raster->get_rows();}
      size_t get_cols() const override { return raster->get_cols();}
      size_t get_zindex() const override { return raster->get_zindex();}
      double get_height() const override { return raster->get_height();}
      double get_width() const override { return raster->get_width();}
      double get_xllcorner() const override { return raster->get_xllcorner();}
      double get_yllcorner() const override { return raster->get_yllcorner();}
      double get_nodata_value() const override { return raster->get_nodata_value(); }
      double get_cellsize() const override { return raster->get_cellsize();}

      KiLib::Rasters::Cell<T> at(size_t row, size_t col, size_t zindex = 0) override { return raster->get(row, col, zindex);}
      KiLib::Rasters::Cell<T> at(size_t row, size_t col, size_t zindex = 0) const override { return raster->get(row,col, zindex);}
      KiLib::Rasters::Cell<T> at(double x, double y, size_t zindex = 0) override { return raster->get( x, y, zindex);}
      KiLib::Rasters::Cell<T> at(double x, double y, size_t zindex = 0) const override { return raster->get(x, y, zindex); }
      KiLib::Rasters::Cell<T> operator()(size_t row, size_t col, size_t zindex = 0) override { return raster->get(row, col, zindex); }
      KiLib::Rasters::Cell<T> operator()(size_t row, size_t col, size_t zindex = 0) const override { return raster->get(row, col, zindex);  }


      void set_xllcorner(double val) override { raster->set_xllcorner(val); }
      void set_yllcorner(double val) override { raster->set_yllcorner(val); }
      void set_cellsize(double val) override { raster->set_cellsize(val); }
      void set_width(double val) override { raster->set_width(val); }
      void set_height(double val) override { raster->set_height(val); }
      void set_nodata_value(double val) override { raster->set_nodata_value(val); }


      void clamp(const T& lo, const T& hi) override { raster->clamp(lo, hi);}
/*      void CopyFrom( const Raster<T>& other )
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
      }*/

      TYPE get_type() const override { return raster->get_type(); }

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
      Raster<T> operator-( const T& val ) const
      {
         return ApplyOperator( *this, val, Raster::OPERAND::MINUS );
      }

      Raster<T> op_minus(const T& val) const {
         return ApplyOperator( val, *this, Raster::OPERAND::MINUS);
      }

      Raster<T> op_divide(const T& val) const {
         return ApplyOperator( val, *this, Raster::OPERAND::DIVIDE);
      }

      Raster<T> op_erfc() const
      {
         if (this->aget_type() == TYPE::DENSE ) {

               // Cast to the dense rasters so we can utilize the special methods there
               KiLib::Rasters::DenseRaster<T>* ad = (KiLib::Rasters::DenseRaster<T>*)raster;
               return (*ad).op_erfc();
               
            } else if ( this->get_type() == TYPE::SPARSE ) {
               KiLib::Rasters::SparseRaster<T>* ad = (KiLib::Rasters::SparseRaster<T>*)raster;
               return (*ad).op_erfc();
            }
            throw NotImplementedException("Other types for operands have not been created");
      }

      Raster<T> op_ierfc() const
      {
         if (this->get_type() == TYPE::DENSE ) {

               // Cast to the dense rasters so we can utilize the special methods there
               KiLib::Rasters::DenseRaster<T>* ad = (KiLib::Rasters::DenseRaster<T>*)raster;
               return (*ad).op_ierfc();
            } else if ( this->get_type() == TYPE::SPARSE ) {
               KiLib::Rasters::SparseRaster<T>* ad = (KiLib::Rasters::SparseRaster<T>*)raster;
               return (*ad).op_erfc();
            }
            throw NotImplementedException("Other types for operands have not been created");
      }







      Raster<T> operator/( const Raster<T>& other ) const
      {
         return ApplyOperator( *this, other, Raster::OPERAND::DIVIDE );
      }

      Raster<T> operator/( const T& val ) const
      {
         return ApplyOperator( *this, val, Raster::OPERAND::DIVIDE );
      }

      bool operator==(const Raster<T>& rhs) const {
         
         // Shortcut true
         if ( this == &rhs ) return true;

         if ( raster->get_type() != rhs.get_type() ) return false;

         // Check each property
         if ( raster->get_xllcorner() != rhs.get_xllcorner() ) return false;
         if ( raster->get_yllcorner() != rhs.get_yllcorner() ) return false;
         if ( raster->get_height() != rhs.get_height() ) return false;
         if ( raster->get_width() != rhs.get_width() ) return false;
         if ( raster->get_nodata_value() != rhs.get_nodata_value() ) return false;
         if ( raster->get_cellsize() != rhs.get_cellsize() ) return false;
         if ( raster->get_rows() != rhs.get_rows() ) return false;
         if ( raster->get_cols() != rhs.get_cols() ) return false;
         if ( raster->get_zindex() != rhs.get_zindex() ) return false;

         switch (get_type()) {
            case TYPE::DENSE:
               return static_cast<KiLib::Rasters::DenseRaster<T>*>(raster)->operator==( *(static_cast<KiLib::Rasters::DenseRaster<T>*>(rhs.raster)));
         case TYPE::SPARSE:
               return static_cast<KiLib::Rasters::SparseRaster<T>*>(raster)->operator==( *(static_cast<KiLib::Rasters::SparseRaster<T>*>(rhs.raster)));
         default:
            throw NotImplementedException("Cannot compare rasters");
         }
         
      }

      T min() const override {
         return raster->min();
      }


      T max() const override {
         return raster->max();
      }

   private:
      IRaster<T>* raster;
      
      bool is_valid_cell(size_t i, size_t j, size_t k) const override { return raster->is_valid_cell(i,j,k); }
      T get_data(size_t i, size_t j, size_t k) const override { return raster->get_data(i,j,k); }


      Raster<T> ApplyOperator( const Raster<T>& a, const Raster<T>& b, typename IRaster<T>::OPERAND op ) const
      {
         // Either use the index to multiply each element, or if we don't have the same kind of rasters
         // we need to multiply by the location, which is slower
         if ( a.get_type() == b.get_type() &&  a.get_rows() == b.get_rows() && a.get_cols() == b.get_cols() && a.get_zindex() == b.get_zindex() )
         {

            if (a.get_type() == TYPE::DENSE ) {

               // Cast to the dense rasters so we can utilize the special methods there
               KiLib::Rasters::DenseRaster<T>* ad = (KiLib::Rasters::DenseRaster<T>*)a.raster;
               KiLib::Rasters::DenseRaster<T>* bd = (KiLib::Rasters::DenseRaster<T>*)b.raster;

               switch ( op )
               {
               case IRaster<T>::OPERAND::MULTIPLY:
                  return *ad * *bd;
               case IRaster<T>::OPERAND::DIVIDE:
                  return *ad / *bd;
                  break;
               case IRaster<T>::OPERAND::PLUS:
                  return *ad + *bd;
                  break;
               case IRaster<T>::OPERAND::MINUS:
                  return *ad - *bd;
               default:
                  throw std::invalid_argument( "ApplyOperator: Unknown OPERAND" );
               };
            } else if ( a.get_type() == TYPE::SPARSE ) {
                // Cast to the sparse rasters so we can utilize the special methods there
               KiLib::Rasters::SparseRaster<T>* ad = (KiLib::Rasters::SparseRaster<T>*)a.raster;
               KiLib::Rasters::SparseRaster<T>* bd = (KiLib::Rasters::SparseRaster<T>*)b.raster;

               switch ( op )
               {
               case IRaster<T>::OPERAND::MULTIPLY:
                  return *ad * *bd;
               case IRaster<T>::OPERAND::DIVIDE:
                  return *ad / *bd;
                  break;
               case IRaster<T>::OPERAND::PLUS:
                  return *ad + *bd;
                  break;
               case IRaster<T>::OPERAND::MINUS:
                  return *ad - *bd;
               default:
                  throw std::invalid_argument( "ApplyOperator: Unknown OPERAND" );
               };
            }
            throw NotImplementedException("Other types for operands have not been created");
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

            out.copy_metadata_from(*op1);

            // Need to loop through each cell in the larger raster->
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

      Raster<T> ApplyOperator( const Raster<T>& a, const T b, typename IRaster<T>::OPERAND op ) const
      {
         if (a.get_type() == TYPE::DENSE ) {

               // Cast to the dense rasters so we can utilize the special methods there
               KiLib::Rasters::DenseRaster<T>* ad = (KiLib::Rasters::DenseRaster<T>*)a.raster;

               switch ( op )
               {
               case IRaster<T>::OPERAND::MULTIPLY:
                  return *ad * b;
               case IRaster<T>::OPERAND::DIVIDE:
                  return *ad / b;
                  break;
               case IRaster<T>::OPERAND::PLUS:
                  return *ad + b;
                  break;
               case IRaster<T>::OPERAND::MINUS:
                  return *ad - b;
               default:
                  throw std::invalid_argument( "ApplyOperator: Unknown OPERAND" );
               };
            } else if ( a.get_type() == TYPE::SPARSE ) {
               // Cast to the sparse rasters so we can utilize the special methods there
               KiLib::Rasters::SparseRaster<T>* ad = (KiLib::Rasters::SparseRaster<T>*)a.raster;

               switch ( op )
               {
               case IRaster<T>::OPERAND::MULTIPLY:
                  return *ad * b;
               case IRaster<T>::OPERAND::DIVIDE:
                  return *ad / b;
                  break;
               case IRaster<T>::OPERAND::PLUS:
                  return *ad + b;
                  break;
               case IRaster<T>::OPERAND::MINUS:
                  return *ad - b;
               default:
                  throw std::invalid_argument( "ApplyOperator: Unknown OPERAND" );
               };
            }
            throw NotImplementedException("Other types for operands have not been created");
      }

      Raster<T> ApplyOperator(  const T& b, const Raster<T>& a, typename IRaster<T>::OPERAND op ) const
      {
         if (a.get_type() == TYPE::DENSE ) {

               // Cast to the dense rasters so we can utilize the special methods there
               KiLib::Rasters::DenseRaster<T>* ad = (KiLib::Rasters::DenseRaster<T>*)a.raster;

               switch ( op )
               {
               case IRaster<T>::OPERAND::MULTIPLY:
                  return *ad * b;
               case IRaster<T>::OPERAND::DIVIDE:
                  return (*ad).op_divide(b) ;
                  break;
               case IRaster<T>::OPERAND::PLUS:
                  return *ad + b;
                  break;
               case IRaster<T>::OPERAND::MINUS:
                  return (*ad).op_minus(b);
               default:
                  throw std::invalid_argument( "ApplyOperator: Unknown OPERAND" );
               };
            } else if ( a.get_type() == TYPE::SPARSE ) {
               // Cast to the sparse rasters so we can utilize the special methods there
               KiLib::Rasters::SparseRaster<T>* ad = (KiLib::Rasters::SparseRaster<T>*)a.raster;

               switch ( op )
               {
               case IRaster<T>::OPERAND::MULTIPLY:
                  return *ad * b;
               case IRaster<T>::OPERAND::DIVIDE:
                  return (*ad).op_divide(b) ;
                  break;
               case IRaster<T>::OPERAND::PLUS:
                  return *ad + b;
                  break;
               case IRaster<T>::OPERAND::MINUS:
                  return (*ad).op_minus(b);
               default:
                  throw std::invalid_argument( "ApplyOperator: Unknown OPERAND" );
               };
            }
            throw NotImplementedException("Other types for operands have not been created");
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


      KiLib::Rasters::Raster<T> out(a);
      out.clamp(lo, hi);
      return out;

   /*   auto in = (std::valarray<T>)a;
      std::transform(std::begin(in), std::end(in), std::begin(in), [&lo, &hi](T& v) { return std::clamp(v, lo, hi); });
      KiLib::Rasters::Raster<T> out( a, in );
      return out;*/
   }



   template <class T> std::valarray<T> erfc( const KiLib::Rasters::Raster<T>& a) {
      return a.op_erfc();
   }

   template <class T> KiLib::Rasters::Raster<T> ierfc ( const KiLib::Rasters::Raster<T>& a) {
      return a.op_ierfc();
   }



} // namespace std

// This is the (scalar * Raster) operator
template <class T, typename C> KiLib::Rasters::Raster<T> operator*( const C k, const KiLib::Rasters::Raster<T>& a )
{
   return a * k;
}

template <class T, typename C> KiLib::Rasters::Raster<T> operator/( const C& k, const KiLib::Rasters::Raster<T>& a )
{
   return a.op_divide(static_cast<const double>(k));
}

template <class T, typename C> KiLib::Rasters::Raster<T> operator-( const C& k, const KiLib::Rasters::Raster<T>& a )
{
   return a.op_minus(k);
}

template <class T> KiLib::Rasters::Raster<T> operator-( const KiLib::Rasters::Raster<T>& a )
{
   return a * -1;
}
