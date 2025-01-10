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

        void set_yllcorner(double val) { this->yllcorner = val; }
        void set_xllcorner(double val) { this->xllcorner = val; }
        void set_cellsize(double val) { this->cellsize = val; }
        void set_nodatavalue(double val) { this->nodata_value = val; }
        double get_nodatavalue() const { return this->nodata_value; }
        void set_width(double val) { this->width = val; }
        void set_height( double val ) { this->height = val; }

        KiLib::Rasters::Cell<T> get(size_t i, size_t j) const override
        {
            // Check out of bounds
            unsigned idx = i * this->cols + j;
            if (idx > this->cols * this->rows) {
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
        Raster<T> operator*(const Raster<T>& r) const {

            const auto result = (std::valarray<T>)r * data;
            Raster<T> new_raster(r, result);
            return new_raster;
        
        }   


           
        Raster(const Raster<T>& other, std::valarray<T> d) 
        {
            this->nnz = other.get_ndata();
            this->rows = other.get_rows();
            this->cols = other.get_cols();

            this->set_xllcorner(other.get_xllcorner());
            this->set_yllcorner(other.get_yllcorner());
            this->set_cellsize(other.get_cellsize());
            this->set_nodatavalue(other.get_nodatavalue());
            this->set_width(other.get_width());
            this->set_height(other.get_height());

            this->data = d;
        }


        const T* GetUnderlyingDataArray() const override {
            return &(data[0]); 
        }

    private:
        size_t nnz;
        std::valarray<T> data;
    };

}
