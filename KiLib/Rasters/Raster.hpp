#pragma once

#include "DenseRaster.hpp"
#include "IRaster.hpp"
//#include "SparseRaster.hpp"
#include <KiLib/Rasters/IRaster.hpp>
#include <KiLib/Rasters/SparseRaster_Vector.hpp>
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




namespace std {
   template <class T> KiLib::Rasters::Raster<T> max( const KiLib::Rasters::Raster<T>& a, KiLib::Rasters::Raster<T>&& b) {
      if ( a.get_type() != b.get_type() ||  a.get_rows() != b.get_rows() || a.get_cols() != b.get_cols() || a.get_zindex() != b.get_zindex() ) {
            throw NotImplementedException("Cannot take max of disimilar rasters at the moment. Feature not implemented");
      }

      if (a.get_type() == KiLib::Rasters::TYPE::DENSE ) {

         // Cast to the dense rasters so we can utilize the special methods there
         const KiLib::Rasters::DenseRaster<T>* ad = (KiLib::Rasters::DenseRaster<T>*)a.raster;
         KiLib::Rasters::DenseRaster<T>* bd = (KiLib::Rasters::DenseRaster<T>*)b.raster;
         *bd = std::max(std::move(*bd), *ad);
         return b;

      } else if ( a.get_type() == KiLib::Rasters::TYPE::SPARSE ) {
         // Cast to the sparse rasters so we can utilize the special methods there
         const KiLib::Rasters::SparseRaster<T>* ad = (KiLib::Rasters::SparseRaster<T>*)a.raster;
         KiLib::Rasters::SparseRaster<T>* bd = (KiLib::Rasters::SparseRaster<T>*)b.raster;
         *bd = std::max(std::move(*bd), *ad);
         return b;
      }
      throw NotImplementedException("Other types for operands have not been created");



   }

   template <class T> KiLib::Rasters::Raster<T> max( KiLib::Rasters::Raster<T>&& a, const KiLib::Rasters::Raster<T>& b) {
      return std::max(b, std::move(a));
   }

   template <class T> KiLib::Rasters::Raster<T> min( const KiLib::Rasters::Raster<T>& a, KiLib::Rasters::Raster<T>&& b) {
      if ( a.get_type() != b.get_type() ||  a.get_rows() != b.get_rows() || a.get_cols() != b.get_cols() || a.get_zindex() != b.get_zindex() ) {
            throw NotImplementedException("Cannot take max of disimilar rasters at the moment. Feature not implemented");
      }

      if (a.get_type() == KiLib::Rasters::TYPE::DENSE ) {

         // Cast to the dense rasters so we can utilize the special methods there
         const KiLib::Rasters::DenseRaster<T>* ad = (KiLib::Rasters::DenseRaster<T>*)a.raster;
         KiLib::Rasters::DenseRaster<T>* bd = (KiLib::Rasters::DenseRaster<T>*)b.raster;
         *bd = std::min(std::move(*bd), *ad);
         return b;

      } else if ( a.get_type() == KiLib::Rasters::TYPE::SPARSE ) {
         // Cast to the sparse rasters so we can utilize the special methods there
         const KiLib::Rasters::SparseRaster<T>* ad = (KiLib::Rasters::SparseRaster<T>*)a.raster;
         KiLib::Rasters::SparseRaster<T>* bd = (KiLib::Rasters::SparseRaster<T>*)b.raster;
         *bd = std::min(std::move(*bd), *ad);
         return b;
      }
      throw NotImplementedException("Other types for operands have not been created");



   }

   template <class T> KiLib::Rasters::Raster<T> min( KiLib::Rasters::Raster<T>&& a, const KiLib::Rasters::Raster<T>& b) {
      return std::min(b, std::move(a));
   }


}





namespace KiLib::Rasters
{

   /**
    * A proxy class for KiLib Rasters. Enables the use of different kinds of rasters 
    * without having to adjust dependent code
    */ 
   template <typename T> class Raster  : public IRaster<T>
   {
   public:

   friend Raster<T> operator*(KiLib::Rasters::Raster<T>&& a, const KiLib::Rasters::Raster<T>& b) ;
   friend Raster<T> operator*(const Raster<T>& a, const Raster<T>& b) ;
   friend Raster<T> operator*(const KiLib::Rasters::Raster<T>& a, KiLib::Rasters::Raster<T>&& b) ;
   friend Raster<T> operator*(KiLib::Rasters::Raster<T>&& a, KiLib::Rasters::Raster<T>&& b) ;
   friend Raster<T> operator*(const double k, const Raster<T>& a);
   friend Raster<T> operator*(const double k, Raster<T>&& a);
   friend Raster<T> operator*(const Raster<T>& a, const double k);
   friend Raster<T> operator*(Raster<T>&& a, const double k);

   friend Raster<T> operator-(KiLib::Rasters::Raster<T>&& a, const KiLib::Rasters::Raster<T>& b) ;
   friend Raster<T> operator-(const Raster<T>& a, const Raster<T>& b) ;
   friend Raster<T> operator-(const KiLib::Rasters::Raster<T>& a, KiLib::Rasters::Raster<T>&& b) ;
   friend Raster<T> operator-(KiLib::Rasters::Raster<T>&& a, KiLib::Rasters::Raster<T>&& b) ;
   friend Raster<T> operator-(const double k, const Raster<T>& a);
   friend Raster<T> operator-(const double k, Raster<T>&& a);
   friend Raster<T> operator-(const Raster<T>& a, const double k);
   friend Raster<T> operator-(Raster<T>&& a, const double k);

   friend Raster<T> operator+(KiLib::Rasters::Raster<T>&& a, const KiLib::Rasters::Raster<T>& b) ;
   friend Raster<T> operator+(const Raster<T>& a, const Raster<T>& b) ;
   friend Raster<T> operator+(const KiLib::Rasters::Raster<T>& a, KiLib::Rasters::Raster<T>&& b) ;
   friend Raster<T> operator+(KiLib::Rasters::Raster<T>&& a, KiLib::Rasters::Raster<T>&& b) ;
   friend Raster<T> operator+(const double k, const Raster<T>& a);
   friend Raster<T> operator+(const double k, Raster<T>&& a);
   friend Raster<T> operator+(const Raster<T>& a, const double k);
   friend Raster<T> operator+(Raster<T>&& a, const double k);

   friend Raster<T> operator/(Raster<T>&& a, const Raster<T>& b) ;
   friend Raster<T> operator/(const Raster<T>& a, const Raster<T>& b) ;
   friend Raster<T> operator/(const Raster<T>& a, Raster<T>&& b) ;
   friend Raster<T> operator/(Raster<T>&& a, Raster<T>&& b) ;
   friend Raster<T> operator/(const double k, const Raster<T>& a);
   friend Raster<T> operator/(const double k, Raster<T>&& a);
   friend Raster<T> operator/(const Raster<T>& a, const double k);
   friend Raster<T> operator/(Raster<T>&& a, const double k);

   friend Raster<T> std::max( Raster<T>&& a, const Raster<T>& b) ;
   friend Raster<T> std::max( const Raster<T>& a, Raster<T>&& b) ;
   friend Raster<T> std::max( const Raster<T>& a, const Raster<T>& b) ;
   friend Raster<T> std::min( const Raster<T>& a, const Raster<T>& b) ;
   friend Raster<T> std::min( Raster<T>&& a, const Raster<T>& b) ;
   friend Raster<T> std::min( const Raster<T>& a, Raster<T>&& b) ;

      Raster () {
         raster = nullptr;
      }

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
   
      Raster( const std::tuple<size_t, size_t, size_t>& dims, const std::map<std::tuple<size_t, size_t, size_t>, double>& values, Rasters::TYPE type = Rasters::TYPE::DENSE) 
      {
         switch(type) {
            case Rasters::TYPE::DENSE:
               raster = new KiLib::Rasters::DenseRaster<T>(dims);
               for (auto const& [location, val] : values) {
                  const auto row   = std::get<0>( location );
                  const auto col   = std::get<1>( location );
                  const auto z = std::get<2>( location );

                  raster->set(row, col, z, val);
               }
               break;
            case Rasters::TYPE::SPARSE:
               raster = new KiLib::Rasters::SparseRaster<T>(dims, values);
               break;
            default:
               throw NotImplementedException("Other raster types not implemented for this constructor");
         }
         raster->set_nodata_value(-9999);
      }

      ~Raster() override {
         if ( raster != nullptr) {
            delete raster;
            raster = nullptr;
         }
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


      Cell<T> get(const Cell<T>& c) const  override 
      {
         return raster->get(c);
      }

      void set(const Cell<T>& t, const T& value) override {

         raster->set(t, value);
      }



      using IRaster<T>::apply;
      void apply( std::function<T(T)> f) override {
         raster->apply(f);
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
               raster = new KiLib::Rasters::DenseRaster<T>( std::make_tuple( other.get_rows(), other.get_cols(), other.get_zindex() ), init_val );
               break;
            case Rasters::TYPE::SPARSE:
               raster = new KiLib::Rasters::SparseRaster<T>( *((KiLib::Rasters::SparseRaster<T>*)other.raster), [init_val](const auto&) { return init_val; });
               break;
            default:
               throw NotImplementedException("Other raster types not implemented for this constructor");
         }
      }

     Raster( const Raster<T>& other, const std::tuple<size_t, size_t, size_t>& dims, double init_val ) : Raster( { other.get_rows(), other.get_cols(), std::get<2>(dims) }, extract_values(other, std::get<2>(dims), init_val), other.get_type()) {
         const auto row   = std::get<0>( dims );
         const auto col   = std::get<1>( dims );

         if (row != 0 || col != 0) {
            throw std::invalid_argument("Please set row and col to 0 acknowledging you are just changing the zindex for initilization");
         }

         raster->copy_metadata_from(other);
      }


     Raster( const Raster<T>& other, const std::tuple<size_t, size_t, size_t>& dims, double init_val, Rasters::TYPE type ): Raster( { other.get_rows(), other.get_cols(), std::get<2>(dims) }, extract_values(other, std::get<2>(dims), init_val), type) {
         const auto row   = std::get<0>( dims );
         const auto col   = std::get<1>( dims );

         if (row != 0 || col != 0) {
            throw std::invalid_argument("Please set row and col to 0 acknowledging you are just changing the zindex for initilization");
         }
         raster->copy_metadata_from(other);
      }

      
      static std::map<std::tuple<size_t, size_t, size_t>, double> extract_values(const Raster<T>& other, size_t z) {
         // Create a list of values and then pass those into the constructor.
         std::map<std::tuple<size_t, size_t, size_t>, double> values;
         for ( auto it = other.begin(); it != other.end(); ++it ) {
            const auto cell= (&it);
            for (size_t z_loop = 0; z_loop < z; z_loop++) {
              values.insert({ { cell.i(), cell.j(), z_loop}, *(cell.data) });
            }
         }
         return values;
      }

      static std::map<std::tuple<size_t, size_t, size_t>, double> extract_values(const Raster<T>& other, size_t z, double init_val) {
         // Create a list of values and then pass those into the constructor.
         std::map<std::tuple<size_t, size_t, size_t>, double> values;
         for ( auto it = other.begin(); it != other.end(); ++it ) {
            const auto cell= (&it);
            for (size_t z_loop = 0; z_loop < z; z_loop++) {
              values.insert({ { cell.i(), cell.j(), z_loop}, init_val });
            }
         }
         return values;
      }

      Raster( const Raster<T>& other, const std::tuple<size_t, size_t, size_t>& dims ) : Raster( { other.get_rows(), other.get_cols(), std::get<2>(dims) }, extract_values(other, std::get<2>(dims)), other.get_type()) {
   
         const auto row   = std::get<0>( dims );
         const auto col   = std::get<1>( dims );

         if (row != 0 || col != 0) {
            throw std::invalid_argument("Please set row and col to 0 acknowledging you are just changing the zindex for initilization");
         }
      }

      void convert_type_to(Rasters::TYPE type) {
         
         if ( type == this->get_type()) return; // Nothing to due, it is already that type!
      

         else if (type == Rasters::TYPE::SPARSE && this->get_type() == Rasters::TYPE::DENSE ) {

           // auto* old = raster;
            raster = new KiLib::Rasters::SparseRaster<T>( *raster, [](const auto& c) { return *(c.data); });
            //delete old;
            return; 

         }
         throw NotImplementedException("Other raster types not implemented for this constructor");

      }

      // Move Operator
      Raster<T>& operator=(Raster<T>&& other) 
      {
         // Just move the pointer
         if ( raster != nullptr ) {
            delete raster;
         }
         raster = other.raster;
         other.raster = nullptr;
         return *this;
      }

      // Copy Operator
      Raster<T>& operator=( const Raster<T>& other )
      {
         delete this->raster;

         switch (other.get_type()) {
            
            case Rasters::TYPE::DENSE:
               raster = new KiLib::Rasters::DenseRaster<T>( *((KiLib::Rasters::DenseRaster<T>*)other.raster));
               break;
            case Rasters::TYPE::SPARSE:
               raster = new KiLib::Rasters::SparseRaster<T>( *((KiLib::Rasters::SparseRaster<T>*)other.raster));
               break;
            default:
               throw NotImplementedException("Other raster types not implemented for this constructor");
         }

         return *this;

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
      std::string get_name() const override {return raster->get_name();}

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
      void set_name(std::string val) override { raster->set_name(val); }


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

      Raster<T> op_minus(const T& val) const {
         return ApplyOperator( val, *this, OPERAND::MINUS);
      }

      Raster<T> op_divide(const T& val) const {
         return ApplyOperator( val, *this, OPERAND::DIVIDE);
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
               return (*ad).op_ierfc();
            }
            throw NotImplementedException("Other types for operands have not been created");
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

      void atan() const {
         raster->apply( [](T t) { return std::atan(t);} );
      }

      void tan() const {
         raster->apply( [](T t) { return std::tan(t); } );
      }

      void cos() const {
         raster->apply( [](T t) { return std::cos(t); } );
      }

      void sin() const {
         raster->apply( [](T t) { return std::sin(t); } );
      }

      void exp() const {
         raster->apply( [](T t) { return std::exp(t); } );
      }

      T max() const override {
         return raster->max();
      }


     using IRaster<T>::begin;
     typename IRaster<T>::RasterIterator begin() override { return raster->begin(); }
     typename IRaster<T>::RasterIterator begin() const override { return raster->begin(); }

     using IRaster<T>::end;
     typename IRaster<T>::RasterIterator end() override { return raster->end(); }
     typename IRaster<T>::RasterIterator end() const override { return raster->end(); }




   private:
      IRaster<T>* raster;
      using IRaster<T>::name;
      
      bool is_valid_cell(size_t i, size_t j, size_t k) const override { return raster->is_valid_cell(i,j,k); }
      T get_data(size_t i, size_t j, size_t k) const override { return raster->get_data(i,j,k); }

      static Raster<T> ApplyOperator( const Raster<T>& a, const Raster<T>& b, OPERAND op );
      static Raster<T> ApplyOperator_LR( const Raster<T>& a, Raster<T>&& b, OPERAND op );
      static Raster<T> ApplyOperator_RL( Raster<T>&& a, const Raster<T>& b, OPERAND op );
      static Raster<T> ApplyOperator_RR( Raster<T>&& a, Raster<T>&& b, OPERAND op );
    
      static Raster<T> ApplyOperator( const Raster<T>& a, const T b, OPERAND op )
      {
         if (a.get_type() == TYPE::DENSE ) {

               // Cast to the dense rasters so we can utilize the special methods there
               KiLib::Rasters::DenseRaster<T>* ad = (KiLib::Rasters::DenseRaster<T>*)a.raster;

               switch ( op )
               {
               case OPERAND::MULTIPLY:
                  return *ad * b;
               case OPERAND::DIVIDE:
                  return *ad / b;
                  break;
               case OPERAND::PLUS:
                  return *ad + b;
                  break;
               case OPERAND::MINUS:
                  return *ad - b;
               default:
                  throw std::invalid_argument( "ApplyOperator: Unknown OPERAND" );
               };
            } else if ( a.get_type() == TYPE::SPARSE ) {
               // Cast to the sparse rasters so we can utilize the special methods there
               KiLib::Rasters::SparseRaster<T>* ad = (KiLib::Rasters::SparseRaster<T>*)a.raster;

               switch ( op )
               {
               case OPERAND::MULTIPLY:
                  return *ad * b;
               case OPERAND::DIVIDE:
                  return *ad / b;
                  break;
               case OPERAND::PLUS:
                  return *ad + b;
                  break;
               case OPERAND::MINUS:
                  return *ad - b;
               default:
                  throw std::invalid_argument( "ApplyOperator: Unknown OPERAND" );
               };
            }
            throw NotImplementedException("Other types for operands have not been created");
      }

      static Raster<T> ApplyOperator( Raster<T>&& a, const T b, OPERAND op )
      {
         if (a.get_type() == TYPE::DENSE ) {

               // Cast to the dense rasters so we can utilize the special methods there
               KiLib::Rasters::DenseRaster<T>* ad = (KiLib::Rasters::DenseRaster<T>*)a.raster;

               switch ( op )
               {
               case OPERAND::MULTIPLY:
                  *ad = std::move(*ad) * b;
                  break;
               case OPERAND::DIVIDE:
                  *ad = std::move(*ad) / b;
                  break;
               case OPERAND::PLUS:
                  *ad = std::move(*ad) + b;
                  break;
               case OPERAND::MINUS:
                  *ad = std::move(*ad) - b;
                  break;
               default:
                  throw std::invalid_argument( "ApplyOperator: Unknown OPERAND" );
               };
               return a;
            } else if ( a.get_type() == TYPE::SPARSE ) {
               // Cast to the sparse rasters so we can utilize the special methods there
               KiLib::Rasters::SparseRaster<T>* ad = (KiLib::Rasters::SparseRaster<T>*)a.raster;

               switch ( op )
               {
               case OPERAND::MULTIPLY:
                  *ad = std::move(*ad) * b;
                  break;
               case OPERAND::DIVIDE:
                  *ad = std::move(*ad) / b;
                  break;
               case OPERAND::PLUS:
                  *ad = std::move(*ad) + b;
                  break;
               case OPERAND::MINUS:
                  *ad = std::move(*ad) - b;
                  break;
               default:
                  throw std::invalid_argument( "ApplyOperator: Unknown OPERAND" );
               };
               return a;
            }
            throw NotImplementedException("Other types for operands have not been created");
      }



      static Raster<T> ApplyOperator(  const T& b, const Raster<T>& a, OPERAND op ) 
      {
         if (a.get_type() == TYPE::DENSE ) {

               // Cast to the dense rasters so we can utilize the special methods there
               const KiLib::Rasters::DenseRaster<T>* ad = (KiLib::Rasters::DenseRaster<T>*)a.raster;

               switch ( op )
               {
               case OPERAND::MULTIPLY:
                  return *ad * b;
               case OPERAND::DIVIDE:
                  return b / *ad;
               case OPERAND::PLUS:
                  return *ad + b;
               case OPERAND::MINUS:
                  return (*ad).op_minus(b);
               default:
                  throw std::invalid_argument( "ApplyOperator: Unknown OPERAND" );
               };
            } else if ( a.get_type() == TYPE::SPARSE ) {
               // Cast to the sparse rasters so we can utilize the special methods there
               const KiLib::Rasters::SparseRaster<T>* ad = (KiLib::Rasters::SparseRaster<T>*)a.raster;

               switch ( op )
               {
               case OPERAND::MULTIPLY:
                  return *ad * b;
               case OPERAND::DIVIDE:
                  return b / *ad;
               case OPERAND::PLUS:
                  return *ad + b;
               case OPERAND::MINUS:
                  return b - *ad;
               default:
                  throw std::invalid_argument( "ApplyOperator: Unknown OPERAND" );
               };
            }
            throw NotImplementedException("Other types for operands have not been created");
      }

      static Raster<T> ApplyOperator(  const T& b, Raster<T>&& a, OPERAND op ) 
      {
         if (a.get_type() == TYPE::DENSE ) {

               // Cast to the dense rasters so we can utilize the special methods there
               KiLib::Rasters::DenseRaster<T>* ad = (KiLib::Rasters::DenseRaster<T>*)a.raster;

               switch ( op )
               {
               case OPERAND::MULTIPLY:
                  *ad *= b;
                  return std::move(a);
               case OPERAND::DIVIDE:
                  (*ad).op_divide(b); 
                  return std::move(a);
               case OPERAND::PLUS:
                  *ad += b;
                  return std::move(a);
               case OPERAND::MINUS:
                  b - *ad;
                  return std::move(a);
               default:
                  throw std::invalid_argument( "ApplyOperator: Unknown OPERAND" );
               };
            } else if ( a.get_type() == TYPE::SPARSE ) {
               // Cast to the sparse rasters so we can utilize the special methods there
               KiLib::Rasters::SparseRaster<T>* ad = (KiLib::Rasters::SparseRaster<T>*)a.raster;

               switch ( op )
               {
               case OPERAND::MULTIPLY:
                  *ad = std::move(*ad) * b;
                  break;
               case OPERAND::DIVIDE:
                  (*ad).op_divide(b);
                  break;
               case OPERAND::PLUS:
                  *ad = std::move(*ad) + b;
                  break;
               case OPERAND::MINUS:
                  (*ad).op_minus(b);
                  break;
               default:
                  throw std::invalid_argument( "ApplyOperator: Unknown OPERAND" );
               };
               return a;
            }
            throw NotImplementedException("Other types for operands have not been created");
      }



   };
} // namespace KiLib::Rasters


namespace std
{

   template <class T> KiLib::Rasters::Raster<T> atan( const KiLib::Rasters::Raster<T>& a )
   {
      KiLib::Rasters::Raster<T> out(a);
      out.atan();
      out.set_name( "atan(" + out.get_name() + ")");
      return out;
   }
   template <class T> KiLib::Rasters::Raster<T> atan( KiLib::Rasters::Raster<T>&& a )
   {
      a.atan();
      a.set_name( "atan(" + a.get_name() + ")");
      return a;
   }

   template <class T> KiLib::Rasters::Raster<T> sin( const KiLib::Rasters::Raster<T>& a )
   {
      KiLib::Rasters::Raster<T> out(a);
      out.sin();
      out.set_name( "sin(" + out.get_name() + ")");
      return out;
   }
   template <class T> KiLib::Rasters::Raster<T> sin( KiLib::Rasters::Raster<T>&& a )
   {
      a.sin();
      a.set_name( "sin(" + a.get_name() + ")");
      return a;
   }

   template <class T> KiLib::Rasters::Raster<T> cos( const KiLib::Rasters::Raster<T>& a )
   {
      KiLib::Rasters::Raster<T> out(a);
      out.cos();
      out.set_name( "cos(" + out.get_name() + ")");
      return out;
   }

   template <class T> KiLib::Rasters::Raster<T> cos( KiLib::Rasters::Raster<T>&& a )
   {
      a.cos();
      a.set_name( "cos(" + a.get_name() + ")");
      return a;
   }



   template <class T> KiLib::Rasters::Raster<T> tan( const KiLib::Rasters::Raster<T>& a )
   {
      KiLib::Rasters::Raster<T> out(a);
      out.tan();
      out.set_name( "tan(" + out.get_name() + ")");
      return out;
   }

   template <class T> KiLib::Rasters::Raster<T> tan( KiLib::Rasters::Raster<T>&& a )
   {
      a.tan();
      a.set_name( "tan(" + a.get_name() + ")");
      return a;
   }



   template <class T> KiLib::Rasters::Raster<T> exp( KiLib::Rasters::Raster<T>&& a )
   {
      a.exp();
      a.set_name( "exp(" + a.get_name() + ")");
      return a;
   }

   template <class T> KiLib::Rasters::Raster<T> exp( const KiLib::Rasters::Raster<T>& a )
   {
      KiLib::Rasters::Raster<T> out(a);
      out.exp();
      out.set_name( "exp(" + out.get_name() + ")");
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


   template <class T> KiLib::Rasters::Raster<T> max( const KiLib::Rasters::Raster<T>& b, double a) {
      return max(b, a);
   }

   template <class T> KiLib::Rasters::Raster<T> max( double b, const KiLib::Rasters::Raster<T>& a)
   {
      KiLib::Rasters::Raster<T> out(a);
      out.apply( [&b](T t) { return std::max(t, b);} );
      return out;
   }

   template <class T> KiLib::Rasters::Raster<T> max( KiLib::Rasters::Raster<T>&& b, double a) {
      return max(a, std::forward(b));
   }

   template <class T> KiLib::Rasters::Raster<T> max( double b, KiLib::Rasters::Raster<T>&& a)
   {
      a.apply( [&b](T t) { return std::min(t, b);} );
      return a;
   }





   template <class T> KiLib::Rasters::Raster<T> min( const KiLib::Rasters::Raster<T>& b, double a) {
      return min(b, a);
   }

   template <class T> KiLib::Rasters::Raster<T> min( double b, const KiLib::Rasters::Raster<T>& a)
   {
      KiLib::Rasters::Raster<T> out(a);
      out.apply( [&b](T t) { return std::min(t, b);} );
      return out;
   }


   template <class T> KiLib::Rasters::Raster<T> min( KiLib::Rasters::Raster<T>&& b, double a) {
      return min(a, std::forward(b));
   }

   template <class T> KiLib::Rasters::Raster<T> min( double b, KiLib::Rasters::Raster<T>&& a)
   {
      a.apply( [&b](T t) { return std::min(t, b);} );
      return a;
   }



   template <class T> KiLib::Rasters::Raster<T> max( const KiLib::Rasters::Raster<T>& a, const KiLib::Rasters::Raster<T>& b)
   {
      if ( a.get_type() == b.get_type() &&  a.get_rows() == b.get_rows() && a.get_cols() == b.get_cols() && a.get_zindex() == b.get_zindex() )
      {
         if (a.get_type() == KiLib::Rasters::TYPE::DENSE ) {

            // Cast to the dense rasters so we can utilize the special methods there
            const KiLib::Rasters::DenseRaster<T>* ad = (KiLib::Rasters::DenseRaster<T>*)a.raster;
            const KiLib::Rasters::DenseRaster<T>* bd = (KiLib::Rasters::DenseRaster<T>*)b.raster;
            return std::max(*bd, *ad);

         } else if ( a.get_type() == KiLib::Rasters::TYPE::SPARSE ) {
            // Cast to the sparse rasters so we can utilize the special methods there
            const KiLib::Rasters::SparseRaster<T>* ad = (KiLib::Rasters::SparseRaster<T>*)a.raster;
            const KiLib::Rasters::SparseRaster<T>* bd = (KiLib::Rasters::SparseRaster<T>*)b.raster;
            return std::max(*bd, *ad);
         }
      }


         auto* op1     = &a;
         auto* op2     = &b;

            if ( b.get_cellsize() < a.get_cellsize() ) {
               op1     = &b;
               op2     = &a;
            } else if ( a.get_ndata() < b.get_ndata() )
            {
               op1     = &b;
               op2     = &a;
            }

         KiLib::Rasters::Raster<T> out(*op1);
         out.copy_metadata_from(*op1);

         std::for_each(EXEC_POLICY, out.begin(), out.end(), [&](auto it) {
            size_t r = it.i();
            size_t c = it.j();
            size_t z = it.k();
            size_t x = it.x();
            size_t y = it.y();

            auto cell_b = (*op2).get((double)x,(double)y, z);

            if (cell_b.is_nodata) return;

            out.set(r,c, z, std::max( *(it.data), *(cell_b.data) ));
         });
         return out;

   }



   template <class T> KiLib::Rasters::Raster<T> min( const KiLib::Rasters::Raster<T>& a, const KiLib::Rasters::Raster<T>& b)
   {
      if ( a.get_type() == b.get_type() &&  a.get_rows() == b.get_rows() && a.get_cols() == b.get_cols() && a.get_zindex() == b.get_zindex() )
      {
         if (a.get_type() == KiLib::Rasters::TYPE::DENSE ) {

            // Cast to the dense rasters so we can utilize the special methods there
            const KiLib::Rasters::DenseRaster<T>* ad = (KiLib::Rasters::DenseRaster<T>*)a.raster;
            const KiLib::Rasters::DenseRaster<T>* bd = (KiLib::Rasters::DenseRaster<T>*)b.raster;
            return std::max(*bd, *ad);

         } else if ( a.get_type() == KiLib::Rasters::TYPE::SPARSE ) {
            // Cast to the sparse rasters so we can utilize the special methods there
            const KiLib::Rasters::SparseRaster<T>* ad = (KiLib::Rasters::SparseRaster<T>*)a.raster;
            const KiLib::Rasters::SparseRaster<T>* bd = (KiLib::Rasters::SparseRaster<T>*)b.raster;
            return std::max(*bd, *ad);
         }
         else {
            throw NotImplementedException("Raster type not implemented for std::min()");
         }
      }
      else {


         auto* op1     = &a;
         auto* op2     = &b;

         // Find the bigger, more element matrix.
            if ( b.get_cellsize() < a.get_cellsize() ) {
               op1     = &b;
               op2     = &a;
            } else if ( a.get_ndata() < b.get_ndata() )
            {
               op1     = &b;
               op2     = &a;
            }



         KiLib::Rasters::Raster<T> out(*op1);
         out.copy_metadata_from(*op1);

         for ( auto it = out.begin(); it != out.end(); ++it) 
         {
            size_t r = (&it).i();
            size_t c = (&it).j();
            size_t z = (&it).k();
            size_t x = (&it).x();
            size_t y = (&it).y();

            // Check if b is a no data cell
               auto cell_b = (*op2).get((double)x,(double)y, z);
               
               if (cell_b.is_nodata) continue;

               out.set(r,c, z, std::min( *((&it).data), *(cell_b.data) ));
         }
         return out;
      }
   }



   template <class T> KiLib::Rasters::Raster<T> clamp( const KiLib::Rasters::Raster<T>& a, const T& lo, const T& hi)
   {
      KiLib::Rasters::Raster<T> out(a);
      out.clamp(lo, hi);
      return out;
   }
   template <class T> KiLib::Rasters::Raster<T> clamp( KiLib::Rasters::Raster<T>&& a, const T& lo, const T& hi)
   {
      a.clamp(lo, hi);
      return a;
   }




   template <class T> std::valarray<T> erfc( const KiLib::Rasters::Raster<T>& a) {
      return a.op_erfc();
   }

   template <class T> KiLib::Rasters::Raster<T> ierfc ( const KiLib::Rasters::Raster<T>& a) {
      return a.op_ierfc();
   }



} // namespace std

template <class T> KiLib::Rasters::Raster<T> operator-( const KiLib::Rasters::Raster<T>& a )
{
   return a * -1;
}


template <class T> KiLib::Rasters::Raster<T> operator-( const KiLib::Rasters::Raster<T>&& a )
{
   return std::move(a) * -1;
}
