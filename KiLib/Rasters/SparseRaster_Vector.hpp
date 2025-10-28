#pragma once

#include <KiLib/Rasters/IRaster.hpp>
#include <algorithm>
#include <functional>
#include <map>
#include <stddef.h>
#include <optional>
#include <stdexcept>
#include <vector>

#ifndef EXEC_POLICY
#define EXEC_POLICY std::execution::par_unseq
#endif

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


   friend SparseRaster<T> operator*(SparseRaster<T>&& a, const SparseRaster<T>& b) ;
   friend SparseRaster<T> operator*(const SparseRaster<T>& a, const SparseRaster<T>& b) ;
   friend SparseRaster<T> operator*(const SparseRaster<T>& a, SparseRaster<T>&& b) ;
   friend SparseRaster<T> operator*(SparseRaster<T>&& a, SparseRaster<T>&& b) ;
   friend SparseRaster<T> operator*(const double k, const SparseRaster<T>& a);
   friend SparseRaster<T> operator*(const double k, SparseRaster<T>&& a);
   friend SparseRaster<T> operator*(const SparseRaster<T>& a, const double k);
   friend SparseRaster<T> operator*(SparseRaster<T>&& a, const double k);




   friend SparseRaster<T> operator+(SparseRaster<T>&& a, const SparseRaster<T>& b) ;
   friend SparseRaster<T> operator+(const SparseRaster<T>& a, const SparseRaster<T>& b) ;
   friend SparseRaster<T> operator+(const SparseRaster<T>& a, SparseRaster<T>&& b) ;
   friend SparseRaster<T> operator+(SparseRaster<T>&& a, SparseRaster<T>&& b) ;
   friend SparseRaster<T> operator+(const double k, const SparseRaster<T>& a);
   friend SparseRaster<T> operator+(const double k, SparseRaster<T>&& a);
   friend SparseRaster<T> operator+(const SparseRaster<T>& a, const double k);
   friend SparseRaster<T> operator+(SparseRaster<T>&& a, const double k);



   friend SparseRaster<T> operator-(SparseRaster<T>&& a, const SparseRaster<T>& b) ;
   friend SparseRaster<T> operator-(const SparseRaster<T>& a, const SparseRaster<T>& b) ;
   friend SparseRaster<T> operator-(const SparseRaster<T>& a, SparseRaster<T>&& b) ;
   friend SparseRaster<T> operator-(SparseRaster<T>&& a, SparseRaster<T>&& b) ;
   friend SparseRaster<T> operator-(const double k, const SparseRaster<T>& a);
   friend SparseRaster<T> operator-(const double k, SparseRaster<T>&& a);
   friend SparseRaster<T> operator-(const SparseRaster<T>& a, const double k);
   friend SparseRaster<T> operator-(SparseRaster<T>&& a, const double k);



   friend SparseRaster<T> operator/(SparseRaster<T>&& a, const SparseRaster<T>& b) ;
   friend SparseRaster<T> operator/(const SparseRaster<T>& a, const SparseRaster<T>& b) ;
   friend SparseRaster<T> operator/(const SparseRaster<T>& a, SparseRaster<T>&& b) ;
   friend SparseRaster<T> operator/(SparseRaster<T>&& a, SparseRaster<T>&& b) ;
   friend SparseRaster<T> operator/(const double k, const SparseRaster<T>& a);
   friend SparseRaster<T> operator/(const double k, SparseRaster<T>&& a);
   friend SparseRaster<T> operator/(const SparseRaster<T>& a, const double k);
   friend SparseRaster<T> operator/(SparseRaster<T>&& a, const double k);


   friend SparseRaster<T> operator*=(SparseRaster<T>& a, const SparseRaster<T>& k);
   friend SparseRaster<T> operator+=(SparseRaster<T>& a, const SparseRaster<T>& k);
   friend SparseRaster<T> operator-=(SparseRaster<T>& a, const SparseRaster<T>& k);
   friend SparseRaster<T> operator/=(SparseRaster<T>& a, const SparseRaster<T>& k);
   friend SparseRaster<T> operator/=(const SparseRaster<T>& a, SparseRaster<T>& k);
   friend SparseRaster<T> operator-=(const SparseRaster<T>& a, SparseRaster<T>& k);


   friend SparseRaster<double> operator*= ( SparseRaster<double>& a, const double k );
   friend SparseRaster<double> operator+= ( SparseRaster<double>& a, const double k );
   friend SparseRaster<double> operator/= ( SparseRaster<double>& a, const double k );
   friend SparseRaster<double> operator-= ( SparseRaster<double>& a, const double k );

   friend SparseRaster<T> std::max( SparseRaster<T>&& a, const SparseRaster<T>& b) ;
   friend SparseRaster<T> std::max( const SparseRaster<T>& a, SparseRaster<T>&& b) ;
   friend SparseRaster<T> std::max( const SparseRaster<T>& a, const SparseRaster<T>& b) ;
   friend SparseRaster<T> std::min( SparseRaster<T>&& a, const SparseRaster<T>& b) ;
   friend SparseRaster<T> std::min( const SparseRaster<T>& a, SparseRaster<T>&& b) ;
   friend SparseRaster<T> std::min( const SparseRaster<T>& a, const SparseRaster<T>& b) ;






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



      SparseRaster( const std::tuple<size_t, size_t, size_t>& dims, const std::map<std::tuple<size_t, size_t, size_t>, double>& values ) : nnz( std::get<0>(dims) * std::get<1>(dims) * std::get<2>(dims) ), V(values.size()), COL_INDEX(values.size()), ROW_INDEX(std::get<0>(dims) + 1), Z_INDEX(values.size())
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


        SparseRaster(const SparseRaster<T>& other, const std::vector<T>& new_data) : nnz( other.nnz ), V( new_data ), COL_INDEX(other.COL_INDEX), ROW_INDEX(other.ROW_INDEX), Z_INDEX(other.Z_INDEX)
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

         using IRaster<T>::get_next_valid_cell_index_after;
        size_t get_next_valid_cell_index_after(size_t idx) const override {
              if (idx > this->V.size()) return idx;
               idx++;
            return idx;
        }


      using IRaster<T>::begin;
      IRaster<T>::RasterIterator begin() override { return typename IRaster<T>::RasterIterator(this, 0); }
      IRaster<T>::RasterIterator begin() const override { return typename IRaster<T>::RasterIterator(this, 0); }

      using IRaster<T>::end;
      IRaster<T>::RasterIterator end() override { return typename IRaster<T>::RasterIterator(this, this->V.size()); }
      IRaster<T>::RasterIterator end() const override { return typename IRaster<T>::RasterIterator(this, this->V.size()); }


      using IRaster<T>::ind2sub;
      std::tuple<size_t, size_t, size_t> ind2sub(size_t idx) const  override {
         // take index into array and convert to coordinates

         // Find in the ROW_INDEX what index where the value is greater than idx
         size_t c;
         for (c = 0; c < this->ROW_INDEX.size()  -1; c++) {
            if (this->ROW_INDEX[c + 1] > idx) {
               break;
            }
         }

         if ( c == this->ROW_INDEX.size() - 1) {
            c--;
         }

         return { c , this->COL_INDEX[idx] , this->Z_INDEX[idx] };
      }


      using IRaster<T>::get;
      KiLib::Rasters::Cell<T> get( size_t i, size_t j, size_t k = 0 ) const override
      {
        auto d = _get_data_index(i, j, k);

        if ( d.has_value() && V[d.value()] != this->get_nodata_value()) {
               auto c = KiLib::Rasters::Cell<T>( *this, i, j, k, V[d.value()] );
               c.index = (unsigned)d.value();
               return c;
        }
        else {

        return KiLib::Rasters::Cell<T>( *this, i, j ,k );
        }
      }

      Cell<T> get(const Cell<T>& c) const  override 
      {
         if ( ! c.is_nodata ) {
            auto r = KiLib::Rasters::Cell<T>( *this, c.i(), c.j(), c.k(), V[c.index()] );
            r.index = c.index;
            return r;
         }
         else {
            return KiLib::Rasters::Cell<T>( *this, c.i(), c.j() ,c.k() );
         } 
      }

      size_t get_valid_cell_count() const override  
      {
      const auto nodata = this->get_nodata_value();
         return std::count_if(V.begin(), V.end(), [&nodata](auto v) { if (v != nodata) return true; else return false; });
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

      void set(const Cell<T>& t, const T& value) override {
         V[t.index()] = value;
      }

      size_t get_ndata() const override
      {
         return this->V.size();
      }

      T min() const override {
         T m = std::numeric_limits<double>::max();
         for( size_t i = 0; i < V.size(); i++ ) {
               if ( V[i] != this->get_nodata_value()) 
                  m = std::min(m, V[i]);
         }
         return m;
      }

      T max() const override {
         T m = std::numeric_limits<double>::min();
         for( size_t i = 0; i < V.size(); i++ ) {
               if ( V[i] != this->get_nodata_value()) 
                  m = std::max(m, V[i]);
         }
         return m;
      }

      TYPE get_type() const override { return TYPE::SPARSE; }
      
      void op_divide(const T val)  {

            const auto nodata = this->get_nodata_value();
            std::transform(EXEC_POLICY, V.begin(), V.end(), V.begin(), [&nodata, &val](T v) { if (v == nodata) {return nodata; } else { return val / v;  } } );
         }  

         void op_minus(const T val) {

            const auto nodata = this->get_nodata_value();
            std::transform(EXEC_POLICY, V.begin(), V.end(), V.begin(), [&nodata, &val](T v) { if (v == nodata) {return nodata; } else { return val - v;  } } );
         }  

      SparseRaster<T> op_ierfc() const {

            KiLib::Rasters::SparseRaster<T> out(*this);

            const auto nodata = this->get_nodata_value();
            std::transform(EXEC_POLICY, V.begin(), V.end(), out.V.begin(), [&nodata](T v) { if (v == nodata) {return nodata; } else {

               return 1 / std::sqrt(M_PI) * std::exp( -1 * std::pow(v, 2)) - v * std::erfc(v);
          } } );



            return out;
         }  

      SparseRaster<T> op_erfc() const {
            KiLib::Rasters::SparseRaster<T> out(*this);

            const auto nodata = this->get_nodata_value();
            std::transform( EXEC_POLICY, V.begin(), V.end(), out.V.begin(), [&nodata](T v) { if (v == nodata) {return nodata; } else { return std::erfc(v);  } } );

            return out;
         }  


    bool operator==(const SparseRaster<T>& rhs) const {
        if ( ! can_perform_operation(rhs) ) return false;

         if ( ! std::equal(std::begin(this->V), std::end(this->V), std::begin(rhs.V) )) return false;
         if ( ! std::equal(std::begin(this->COL_INDEX), std::end(this->COL_INDEX), std::begin(rhs.COL_INDEX) )) return false;
         if ( ! std::equal(std::begin(this->ROW_INDEX), std::end(this->ROW_INDEX), std::begin(rhs.ROW_INDEX) )) return false;

        return true;

    }


   private:
      size_t           nnz;
      T                default_value;
      std::vector<T> V;
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

      using IRaster<T>::apply;
      void apply( std::function<T(T)> f) override {

            auto nodata_value = this->get_nodata_value();
            std::transform( EXEC_POLICY, V.begin(), V.end(), V.begin(), [&f, &nodata_value ](T v) { if (v == nodata_value) {return nodata_value; } else { return f(v);  } } );

        }






   }; //SparseRaster
} // namespace KiLib::Rasters


template <class T> KiLib::Rasters::SparseRaster<T> operator-( const KiLib::Rasters::SparseRaster<T>& a )
{
   return -1 * a;
}


namespace std {
   template <class T> KiLib::Rasters::SparseRaster<T> ierfc ( const KiLib::Rasters::SparseRaster<T>& a) {
      const auto n = a.V;
      const auto r = 1 / std::sqrt(M_PI) * std::exp( -1 * std::pow(n, 2)) - n * std::erfc(n);
      KiLib::Rasters::SparseRaster<T> out( a, r );
      return out;
   }


   template <class T> KiLib::Rasters::SparseRaster<T> max( const KiLib::Rasters::SparseRaster<T>& a, KiLib::Rasters::SparseRaster<T>&& b ) 
   {
      return std::max(std::forward(b), a);
   }


   template <class T> KiLib::Rasters::SparseRaster<T> max( KiLib::Rasters::SparseRaster<T>&& a, const KiLib::Rasters::SparseRaster<T>& b )
   {
      //Let's take and transform the raster
      std::transform(EXEC_POLICY, a.V.begin(), a.V.end(), b.V.begin(), a.V.begin(),
                     [](const T& a, const T& b) { 
                        return std::max(a, b); 
                     }
                     );
      return a;
   }

   template <class T> KiLib::Rasters::SparseRaster<T> max(const  KiLib::Rasters::SparseRaster<T>& a, const KiLib::Rasters::SparseRaster<T>& b )
   {
      KiLib::Rasters::SparseRaster<T> out(a);
      //Let's take and transform the raster
      std::transform(EXEC_POLICY, out.V.begin(), out.V.end(), b.V.begin(), out.V.begin(),
                     [](const T& a, const T& b) { 
                        return std::max(a, b); 
                     }
                     );
      return out;
   }


   template <class T> KiLib::Rasters::SparseRaster<T> min( const KiLib::Rasters::SparseRaster<T>& a, KiLib::Rasters::SparseRaster<T>&& b ) 
   {
      return std::min(std::forward(b), a);
   }


   template <class T> KiLib::Rasters::SparseRaster<T> min( KiLib::Rasters::SparseRaster<T>&& a, const KiLib::Rasters::SparseRaster<T>& b )
   {
      //Let's take and transform the raster
      std::transform(EXEC_POLICY, a.V.begin(), a.V.end(), b.V.begin(), a.V.begin(),
                     [](const T& a, const T& b) { 
                        return std::min(a, b); 
                     }
                     );
      return a;
   }

   template <class T> KiLib::Rasters::SparseRaster<T> min(const  KiLib::Rasters::SparseRaster<T>& a, const KiLib::Rasters::SparseRaster<T>& b )
   {
      KiLib::Rasters::SparseRaster<T> out(a);
      //Let's take and transform the raster
      std::transform(EXEC_POLICY, out.V.begin(), out.V.end(), b.V.begin(), out.V.begin(),
                     [](const T& a, const T& b) { 
                        return std::min(a, b); 
                     }
                     );
      return out;
   }

};
