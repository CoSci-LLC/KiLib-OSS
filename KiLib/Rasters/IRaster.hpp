#pragma once

#include <execution>
#include <stddef.h>
#include <algorithm>
#include <mutex>
#include <memory>
#include <stdexcept>
#include <sstream>

#include <iostream>
#include <KiLib/Exceptions/NotImplemented.hpp>
#include <KiLib/Rasters/Cell.hpp>
#include <functional>
#include <iterator>
#include <valarray>



namespace std { 
   template <class T> std::valarray<T> erfc( const std::valarray<T>& a) {
      return a.apply([](T n) -> T { return std::erfc(n); });
   }
}


namespace KiLib::Rasters
{
    
    enum class TYPE {
        INVALID,
        DENSE,
        SPARSE
    };
    
    template<typename T>
    class IRaster
    {
    public:

        using iterator_category = std::input_iterator_tag;
        using value_type = T; //Typically will be doubles
        using difference_type = size_t;
        using pointer = const T*;
        using reference = T;

        IRaster() {}
        IRaster(const IRaster<T>& other) {
            this->rows = other.get_rows();
            this->cols = other.get_cols();
            this->zindex = other.get_zindex();
            this->copy_metadata_from(other);
        }
        IRaster(size_t rows, size_t cols, size_t zindex): rows(rows), cols(cols), zindex(zindex) {}
        virtual ~IRaster() {}

        
        virtual Cell<T> get(double x, double y, size_t z = 0) const {

            double rF = (y - this->get_yllcorner()) / this->get_cellsize();
            double cF = (x - this->get_xllcorner()) / this->get_cellsize();

            //size_t r = std::clamp<size_t>((size_t)std::floor(rF), 0UL, this->get_rows() - 1);
            //size_t c = std::clamp<size_t>((size_t)std::floor(cF), 0UL, this->get_cols() - 1);

            return get((size_t)rF, (size_t)cF, z);
        }

        virtual TYPE get_type() const { return TYPE::INVALID; }
            
        void copy_metadata_from(const IRaster<T>& other) {

            this->set_xllcorner(other.get_xllcorner());
            this->set_yllcorner(other.get_yllcorner());
            this->set_cellsize(other.get_cellsize());
            this->set_nodata_value(other.get_nodata_value());
            this->set_width(other.get_width());
            this->set_height(other.get_height());
            this->set_name(other.get_name());
        }

        virtual Cell<T> get(size_t,  size_t , size_t ) const  = 0;
       
        virtual void set(double x, double y, const T& value) {
            set(x,y,0, value);
        }

        virtual void set(double x, double y, size_t z, const T& value) {
            double rF = (y - this->get_yllcorner()) / this->get_cellsize();
            double cF = (x - this->get_xllcorner()) / this->get_cellsize();

            if (rF > this->get_rows() || rF < 0 || cF > this->get_cols() || cF < 0) throw std::invalid_argument("Invalid x,y given to raster. Out of bounds");
            
            //size_t r = std::clamp<size_t>((size_t)std::floor(rF), 0UL, this->get_rows() - 1);
            //size_t c = std::clamp<size_t>((size_t)std::floor(cF), 0UL, this->get_cols() - 1);
            
            set((size_t)rF, (size_t)cF, z, value);
        }

        virtual void set(size_t , size_t, size_t, const T&)  = 0;
        virtual void set(size_t i, size_t j, const T& value) {

            set(i,j,0, value);

            }

        virtual size_t getNRows() const
        {
            return rows;
        }

        virtual size_t get_valid_cell_count() const 
        {
            return std::distance( (*this).begin(), (*this).end()); 
        }
        

        virtual void set_name(std::string val) { name = val; }
        virtual std::string get_name() const { return name; }

        virtual size_t getNCols() const
        {
            return cols;
        }

        virtual KiLib::Rasters::Cell<T> at(size_t row, size_t col, size_t zindex = 0)
        {
            return get(row, col, zindex);
        }

        virtual KiLib::Rasters::Cell<T> at(size_t row, size_t col, size_t zindex = 0) const
        {
            return get(row, col, zindex);
        }

        virtual KiLib::Rasters::Cell<T> at(double x, double y, size_t zindex = 0)
        {
            return get(x, y, zindex );
        }

        virtual KiLib::Rasters::Cell<T> at(double x, double y, size_t zindex = 0) const
        {
            return get(x, y, zindex);
        }


        virtual KiLib::Rasters::Cell<T> operator()(size_t row, size_t col, size_t zindex = 0)
        {
            return get(row, col, zindex);
        }

        virtual KiLib::Rasters::Cell<T> operator()(size_t row, size_t col, size_t zindex = 0) const
        {
            return get(row, col, zindex);
        }

        virtual double get_xllcorner() const
        {
            return this->xllcorner;
        }
        virtual void set_xllcorner(double val) { this->xllcorner = val; }
        virtual double get_yllcorner() const
        {
            return this->yllcorner;
        }
    virtual void set_yllcorner(double val) { this->yllcorner = val; }
        virtual double get_cellsize() const
        {
            return this->cellsize;
        }
    virtual void set_cellsize(double val) { this->cellsize = val; }
        virtual double get_width() const
        {
            return this->width;
        }
    virtual void set_width(double val) { this->width = val; }
        virtual double get_height() const
        {
            return this->height;
        }
    virtual void set_height(double val) { this->height = val; }
        virtual double get_nodata_value() const
        {
            return this->nodata_value;
        }

        virtual void set_nodata_value(double val) { this->nodata_value = val; }
        virtual size_t get_rows() const
        {
            return this->rows;
        }

        virtual size_t get_cols() const
        {
            return this->cols;
        }

        virtual size_t get_zindex() const
        {
            return this->zindex;
        }

        virtual size_t get_ndata() const = 0 ;

        virtual inline size_t flatten_index(size_t i, size_t j, size_t k) const
        {
            return (this->get_rows() * this->get_cols() * k) + (this->get_cols() * i) + j;
        }

        virtual std::string to_string() const {
         std::stringstream ss; 
         
         ss << fmt::format( "Raster {} [{} x {}]\n", this->name, get_rows(), get_cols());
         ss << fmt::format( "  {} valid cells out of {} ({:.2f}% valid)\n", get_valid_cell_count(), this->get_ndata(), 100.0 * get_valid_cell_count() / this->get_ndata());
         ss << fmt::format( "  x: {}  y: {} nodata_val: {}\n", get_xllcorner(), get_yllcorner(), get_nodata_value()); 

         return ss.str();
        }

        virtual std::tuple<size_t, size_t, size_t> ind2sub(size_t idx) const 
        {
            size_t z = idx / (get_rows() * get_cols());
            idx -= (z * get_rows() * get_cols());
            size_t y = idx / get_rows();
            size_t x = idx % get_rows();

            return {x, y, z};
        }


        virtual T min() const {
            T m = std::numeric_limits<T>::max();
            for ( auto cell : *this ) {
                    m = std::min(m, *(cell.data));
            } 
            return m;
        }

        

        virtual T max() const {
            T m = std::numeric_limits<T>::min();
            for ( auto cell : *this ) {
                    m = std::max(m, *(cell.data));
            } 
            return m;
        }

        virtual void apply( std::function<T(T)> f) {
            size_t total = this->get_rows() * this->get_cols() * this->get_zindex();
            for ( size_t idx = 0; idx < total; idx++ ) {
                size_t i,j,k;
                auto r = this->ind2sub(idx);
                i = std::get<0>(r);
                j = std::get<1>(r);
                k = std::get<2>(r);
                if ( this->is_valid_cell(i, j, k) ) {
                    auto v = f(get_data(i,j,k));
                    this->set(i,j,k, v);
                }
            } 

        }



        virtual void clamp(const T& lo, const T& hi) {
            size_t total = this->get_rows() * this->get_cols();
            for ( size_t idx = 0; idx < total; idx++ ) {
                size_t i,j,k;
                auto r = this->ind2sub(idx);
                i = std::get<0>(r);
                j = std::get<1>(r);
                k = std::get<2>(r);
                if ( this->is_valid_cell(i, j, k) ) {
                    this->set(i,j,k, std::clamp(get_data(i,j,k), lo, hi));
                }
            } 

        }

   class RasterIterator
   {
   public:
      using iterator_category = std::input_iterator_tag;
      using value_type        = T; // Typically will be doubles
      using difference_type   = double;
      using pointer           = const T*;
      using reference         = Rasters::Cell<T>;

      RasterIterator( KiLib::Rasters::IRaster<T>* ptr, size_t idx = 0 ) : raster( ptr ), idx(idx)
      {
            auto r = raster->ind2sub(this->idx);
            size_t i = std::get<0>(r);
            size_t j = std::get<1>(r);
            size_t k = std::get<2>(r);
            
            while ( ! raster->is_valid_cell( i, j, k) && (this->idx < (raster->get_ndata()))) {
                this->idx++;
                r = raster->ind2sub(this->idx);
                i = std::get<0>(r);
                j = std::get<1>(r);
                k = std::get<2>(r);
            }
      }

      RasterIterator( const KiLib::Rasters::IRaster<T>* ptr, size_t idx = 0 ) : raster( ptr ), idx(idx)
      {
            auto r = raster->ind2sub(this->idx);
            size_t i = std::get<0>(r);
            size_t j = std::get<1>(r);
            size_t k = std::get<2>(r);
            
            while ( ! raster->is_valid_cell( i, j, k) && (this->idx < (raster->get_ndata()))) {
                this->idx++;
                r = raster->ind2sub(this->idx);
                i = std::get<0>(r);
                j = std::get<1>(r);
                k = std::get<2>(r);
            }
      }

      reference operator*() const
      {

            size_t i,j,k;
                auto r = raster->ind2sub(idx);
                i = std::get<0>(r);
                j = std::get<1>(r);
                k = std::get<2>(r);
         return raster->get( i, j, k);
      }

      Cell<T> operator&() const {
            
            size_t i,j,k;
                auto r = raster->ind2sub(idx);
                i = std::get<0>(r);
                j = std::get<1>(r);
                k = std::get<2>(r);
         return raster->get( i, j, k);
    
      }

      RasterIterator& operator++()
      {
            size_t i,j,k;
         do
         {
                if (this->idx > (raster->get_rows() * raster->get_cols() * raster->get_zindex()) - 1) break;

                this->idx++;

                auto r = raster->ind2sub(this->idx);
                i = std::get<0>(r);
                j = std::get<1>(r);
                k = std::get<2>(r);
         } while ( ! raster->is_valid_cell( i, j, k) );

         return *this;
      }


      RasterIterator operator++(int) {
         RasterIterator tmp = *this;
         ++(*this);
         return tmp;
      }

      friend bool operator==(const RasterIterator& a, const RasterIterator& b) {
            return a.idx == b.idx;
      }

      friend bool operator!=(const RasterIterator& a, const RasterIterator& b) {
         return ! ( a == b);
      }

   private:
      const KiLib::Rasters::IRaster<T>* raster;
      size_t                      idx;
   };


        virtual RasterIterator begin()  { return RasterIterator(this); }
        virtual RasterIterator begin() const  { return RasterIterator(this); }
        virtual RasterIterator end() { return RasterIterator(this, this->get_rows() * this->get_cols() * this->get_zindex()); }
        virtual RasterIterator end() const { return RasterIterator(this, this->get_rows() * this->get_cols() * this->get_zindex()); }

        virtual bool is_valid_cell(size_t , size_t , size_t ) const = 0;
        virtual T get_data(size_t , size_t , size_t ) const  =0;


    protected:
        size_t rows, cols, zindex;
        double xllcorner;    // Lower left corner x value in absolute coordinates
        double yllcorner;    // Lower left corner y value in absolute coordinates
        double cellsize;     // [m] Distance between values
        double width;        // [m] Width in X
        double height;       // [m] Height in Y
        double nodata_value; // Value associated with no data from DEM
        std::string name = "(No name)";
      enum class OPERAND
      {
         MULTIPLY = 0,
         DIVIDE,
         PLUS,
         MINUS,
         MAX,
         MIN
      };

    };
}

