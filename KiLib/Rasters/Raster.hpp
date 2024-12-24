#pragma once

#include <stddef.h>
#include <algorithm>
#include <memory>
#include <stdexcept>
#include <iostream>
#include <functional>
#include <KiLib/Rasters/IRaster.hpp>
#include <map>


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
        Raster(size_t rows, size_t cols): rows(rows), cols(cols) {
            data.reserve( rows * cols );

            nnz = rows * cols;

        }

        void set_yllcorner(double val) { yllcorner = val; }
        void set_xllcorner(double val) { xllcorner = val; }
        void set_cellsize(double val) { cellsize = val; }
        void set_nodatavalue(double val) { nodata_value = val; }
        double get_nodatavalue() { return nodata_value; }
        void set_width(double val) { width = val; }
        void set_height( double val ) { height = val; }

        KiLib::Rasters::Cell<T> get(double x, double y) const override
        {
            double rF = (y - this->yllcorner) / this->cellsize;
            double cF = (x - this->xllcorner) / this->cellsize;

            size_t r = std::clamp<size_t>((size_t)std::floor(rF), 0UL, this->get_rows() - 1);
            size_t c = std::clamp<size_t>((size_t)std::floor(cF), 0UL, this->get_cols() - 1);

            return get(r, c);
        }

        KiLib::Rasters::Cell<T> get(size_t i, size_t j) const override
        {
            // Check out of bounds
            unsigned idx = i * cols + j;
            if (idx > cols * rows) {
                return KiLib::Rasters::Cell<T>(*this, i, j);
            }

           return KiLib::Rasters::Cell<T>(*this, i, j, data.at(idx));
        }

        void set(double x, double y, const T& value) override
        {
            double rF = (y - this->yllcorner) / this->cellsize;
            double cF = (x - this->xllcorner) / this->cellsize;

            size_t r = std::clamp<size_t>((size_t)std::floor(rF), 0UL, this->get_rows() - 1);
            size_t c = std::clamp<size_t>((size_t)std::floor(cF), 0UL, this->get_cols() - 1);
            
            set(r, c, value);
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

        size_t getNRows() const override
        {
            return rows;
        }

        size_t getNCols() const override
        {
            return cols;
        }

        KiLib::Rasters::Cell<T> at(size_t row, size_t col) const override
        {
            return get(row, col);
        }

        KiLib::Rasters::Cell<T> operator()(size_t row, size_t col) const override
        {
            return get(row, col);
        }

        double get_xllcorner() const override
        {
            return this->xllcorner;
        }
        double get_yllcorner() const override
        {
            return this->yllcorner;
        }
        double get_cellsize() const override
        {
            return this->cellsize;
        }
        double get_width() const override
        {
            return this->width;
        }
        double get_height() const override
        {
            return this->height;
        }
        double get_nodata_value() const override
        {
            return this->nodata_value;
        }

        size_t get_rows() const override
        {
            return this->rows;
        }

        size_t get_cols() const override
        {
            return this->cols;
        }

        size_t get_ndata() const override
        {
            return this->nnz;
        }

        size_t flatten_index(size_t r, size_t c) const override
        {
            return r * this->cols + c;
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

        T* GetUnderlyingDataArray() override {
            return data.data(); 
        }

    private:
        size_t rows, cols, nnz;
        std::vector<T> data;
        double xllcorner;    // Lower left corner x value in absolute coordinates
        double yllcorner;    // Lower left corner y value in absolute coordinates
        double cellsize;     // [m] Distance between values
        double width;        // [m] Width in X
        double height;       // [m] Height in Y
        double nodata_value;
    };

}
