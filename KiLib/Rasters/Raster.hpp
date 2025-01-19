#pragma once

#include <stddef.h>
#include <algorithm>
#include <memory>
#include <stdexcept>
#include <iostream>
#include <functional>
#include <KiLib/Rasters/IRaster.hpp>
#include <map>
#include <valarray>


namespace KiLib::Rasters
{

    struct Default {
        double value;
        bool is_nodata;
    };



    template<typename T>
    class Raster : public IDirectAccessRaster<T>
    {
    public:
        Raster(size_t rows, size_t cols) : data(rows * cols) {

            // Initialize the base class variables
            this->rows = rows;
            this->cols = cols;

            // Reserve the data sizes
            data.resize( rows * cols );

            nnz = rows * cols;
        }

        KiLib::Rasters::Cell<T> get(size_t i, size_t j) const override
        {
            // Check out of bounds
            unsigned idx = i * this->cols + j;
            if ( (idx > this->cols * this->rows) || data[idx] == this->get_nodata_value() )  {
                return KiLib::Rasters::Cell<T>(*this, i, j);
            }

           return KiLib::Rasters::Cell<T>(*this, i, j, data[idx]);
        }

        void set(size_t i, size_t j, const T& value) override
        {
            data[i * this->cols + j] = value;
        }

        bool is_valid_cell(double x, double y) const
        {
            double rF = (y - this->yllcorner) / this->cellsize;
            double cF = (x - this->xllcorner) / this->cellsize;

            size_t r = std::clamp<size_t>((size_t)std::floor(rF), 0UL, this->get_rows() - 1);
            size_t c = std::clamp<size_t>((size_t)std::floor(cF), 0UL, this->get_cols() - 1);

            return is_valid_cell(r, c);
        }

        size_t get_ndata() const override
        {
            return this->nnz;
        }

        struct Iterator {
            // Always start from the beginning
            Iterator(const Raster<T>& raster, typename std::map<std::pair<int, int>, T*>::const_iterator it) : raster(raster), it(it) {
            
            };
            

            // incremeneting means going through the list
            Iterator& operator++() 
            {
                it++;
                return *this;
            }

            // post fixing is bad in general but is has it's usages
            Iterator operator++(int) 
            {
                return ++it;
            }          


            // we need to be able to compare nodes
            bool operator!=(const Iterator& other) const  
            {
                return it != other.it;
            }



            // return the data from the node (dereference operator)
            KiLib::Rasters::Cell<T> operator*() const
            {
                // first = the pair
                // first/second = the coordinates inside the pair
                return raster.get((size_t)(*it).first.first, (size_t)(*it).first.second);
            }

        private:
            const Raster<T>& raster;
            typename std::map<std::pair<int, int>, T*>::const_iterator it;
        };

        // returning a const pointer to the front
        Iterator begin() const noexcept
        {
            return Iterator(*this, data.begin());
        };

        // returning a const pointer to the back - the back is always null because it marks the end of the list
        Iterator end() const noexcept
        {
            return Iterator(*this, data.end());
        };

        operator std::valarray<T>() const { return data; }
     /*   Raster<T> operator*(const Raster<T>& r) const {

            const auto result = (std::valarray<T>)r * data;
            Raster<T> new_raster(r, result);
            return new_raster;
        
        }*/   


        Raster(const Raster<T>& other) :  Raster(other, (std::valarray<T>)other) {}

        Raster(const Raster<T>& other, const std::valarray<T>&& d) 
        {
            this->nnz = other.get_ndata();
            this->rows = other.get_rows();
            this->cols = other.get_cols();

            this->set_xllcorner(other.get_xllcorner());
            this->set_yllcorner(other.get_yllcorner());
            this->set_cellsize(other.get_cellsize());
            this->set_nodata_value(other.get_nodata_value());
            this->set_width(other.get_width());
            this->set_height(other.get_height());

            this->data = std::valarray<double>(&d[0], d.size());
        }

        /**
        * Element by element multiplication. Returns a Raster class (may be different that what was provided)
        */ 
        Raster<T>& operator*(const Raster<T>& other) const 
        {
            return ApplyOperator(*this, other, std::multiplies<T>()); 
        }

        Raster<T>& operator+(const Raster<T>& other) const 
        {
            return ApplyOperator(*this, other, std::plus<T>()); 
        }
        
        Raster<T>& operator-(const Raster<T>& other) const
        {
            return ApplyOperator(*this, other, std::minus<T>()); 
        }

        Raster<T>& operator/(const Raster<T>& other) const
        {
            return ApplyOperator(*this, other, std::divides<T>()); 

        }

        static Raster<T> Copy(const Raster<T>& other)
        {
            Raster<T> out(other.get_rows(), other.get_cols());

            out.set_xllcorner(other.get_xllcorner());
            out.set_yllcorner(other.get_yllcorner());
            out.set_cellsize(other.get_cellsize());
            out.set_nodata_value(other.get_nodata_value());
            out.set_width(other.get_width());
            out.set_height(other.get_height());

            return out;
        }

        const T* GetUnderlyingDataArray() const override {
            return &(data[0]); 
        }

    private:
        size_t nnz;
        std::valarray<T> data;

         Raster<T>& ApplyOperator(const Raster<T>& a, const Raster<T>& b, std::function<T(const T&, const T&)> op) const 
        {
            // Either use the index to multiply each element, or if we don't have the same kind of rasters
            // we need to multiply by the location, which is slower
            Raster<T> out(a);
            if ( a.get_rows() == b.get_rows() && a.get_cols() == b.get_cols() )
            {
                for ( size_t i = 0; i < a.get_rows(); i ++ ) {
                    for (size_t j = 0; j < a.get_cols(); j++ ) {
                        auto val = op( *(a.get(i,j).data), *(b.get(i,j).data) ); 
                        out.set( i, j, val);
                    }

                }

            return out;

            }
            else //Multiply by rasters
            {


      //Need to loop through each cell in the larger raster.
      for ( size_t r = 0; r < a.get_rows(); r++) {
         for ( size_t c = 0; c < a.get_cols(); c++ ) { 
            const auto& cell_a = a.get(r, c);

            if ( cell_a.is_nodata || std::isnan( *(cell_a.data) ) || std::isinf( *(cell_a.data) ) ) {
               out.set(r, c, out.get_nodata_value());
               continue;
            }

            // Use the x,y coordinates to get the proper cell.
            const auto& cell_b = b.get(cell_a.x(), cell_a.y());

            double val = 0;

            if ( cell_b.is_nodata || std::isnan( *(cell_b.data) ) || std::isinf( *(cell_b.data) ) ) {
               out.set(r, c, out.get_nodata_value());
               continue;
            }

            val = *(cell_b.data);
            out.set(r, c, *(cell_a.data) * val);
            
         }
      }

      return out;
            }
        }



    };

}
