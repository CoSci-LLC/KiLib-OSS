#pragma once

#include <stddef.h>
#include <algorithm>
#include <memory>
#include <stdexcept>
#include <iostream>
#include <KiLib/Exceptions/NotImplemented.hpp>
#include <KiLib/Rasters/Cell.hpp>
#include <functional>
#include <iterator>

namespace KiLib::Rasters
{

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
            this->set_xllcorner(other.get_xllcorner());
            this->set_yllcorner(other.get_yllcorner());
            this->set_cellsize(other.get_cellsize());
            this->set_nodata_value(other.get_nodata_value());
            this->set_width(other.get_width());
            this->set_height(other.get_height());
            this->rows = other.get_rows();
            this->cols = other.get_cols();
            this->zindex = other.get_zindex();
            

        }
        IRaster(size_t rows, size_t cols, size_t zindex): rows(rows), cols(cols), zindex(zindex) {}
        virtual ~IRaster() {}

        
        virtual Cell<T> get(double x, double y, size_t z = 0) const {

            double rF = (y - this->yllcorner) / this->cellsize;
            double cF = (x - this->xllcorner) / this->cellsize;

            //size_t r = std::clamp<size_t>((size_t)std::floor(rF), 0UL, this->get_rows() - 1);
            //size_t c = std::clamp<size_t>((size_t)std::floor(cF), 0UL, this->get_cols() - 1);

            return get((size_t)rF, (size_t)cF, z);
        }

        virtual Cell<T> get(size_t i, size_t j, size_t k) const = 0;
       
        virtual void set(double x, double y, const T& value) {
            set(x,y,0, value);
        }

        virtual void set(double x, double y, size_t z, const T& value) {
            double rF = (y - this->yllcorner) / this->cellsize;
            double cF = (x - this->xllcorner) / this->cellsize;

            if (rF > this->get_rows() || rF < 0 || cF > this->get_cols() || cF < 0) throw std::invalid_argument("Invalid x,y given to raster. Out of bounds");
            
            //size_t r = std::clamp<size_t>((size_t)std::floor(rF), 0UL, this->get_rows() - 1);
            //size_t c = std::clamp<size_t>((size_t)std::floor(cF), 0UL, this->get_cols() - 1);
            
            set((size_t)rF, (size_t)cF, z, value);
        }

        virtual void set(size_t i, size_t j, size_t k, const T& value) = 0;
        virtual void set(size_t i, size_t j, const T& value) {

            set(i,j,0, value);

            }

        virtual size_t getNRows() const
        {
            return rows;
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

        virtual size_t get_ndata() const = 0;

        virtual inline size_t flatten_index(size_t i, size_t j, size_t k) const
        {
            return (this->rows * this->cols * k) + (this->cols * i) + j;
        }

        virtual std::tuple<size_t, size_t, size_t> ind2sub(size_t idx) const 
        {
            size_t z = idx / (get_rows() * get_cols());
            idx -= (z * get_rows() * get_cols());
            size_t y = idx / get_rows();
            size_t x = idx % get_rows();

            return {x, y, z};
        }




   class RasterIterator
   {
   public:
      using iterator_category = std::input_iterator_tag;
      using value_type        = T; // Typically will be doubles
      using difference_type   = double;
      using pointer           = const T*;
      using reference         = T;

      RasterIterator( KiLib::Rasters::IRaster<T>* ptr, size_t idx = 0 ) : raster( ptr ), idx(idx)
      {
      }

      reference operator*() const
      {

            size_t i,j,k;
                auto r = raster->ind2sub(idx);
                i = std::get<0>(r);
                j = std::get<1>(r);
                k = std::get<2>(r);
         return *(raster->get( i, j, k).data);
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
                if (idx > raster->get_ndata() - 1) break;

                idx++;

                auto r = raster->ind2sub(idx);
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
      KiLib::Rasters::IRaster<T>* raster;
      size_t                      idx;
   };


        virtual RasterIterator begin() { return RasterIterator(this); }
        virtual RasterIterator end() { return RasterIterator(this, this->get_ndata()); }





    protected:
        size_t rows, cols, zindex;
        double xllcorner;    // Lower left corner x value in absolute coordinates
        double yllcorner;    // Lower left corner y value in absolute coordinates
        double cellsize;     // [m] Distance between values
        double width;        // [m] Width in X
        double height;       // [m] Height in Y
        double nodata_value; // Value associated with no data from DEM
        std::string name = "(No name)";

    private: 
        virtual bool is_valid_cell(size_t i, size_t j, size_t k) const = 0;
        virtual T get_data(size_t i, size_t j, size_t k) const = 0;
    };


    template<typename T>
    class IDirectAccessRaster : virtual public IRaster<T> {

    public:
        virtual const T* GetUnderlyingDataArray() const = 0;

    };


    template<typename T>
    class IValArrayRaster : virtual public IRaster<T> {

    public:
        virtual const std::valarray<T>& get_valarray() const = 0;

    };

}
