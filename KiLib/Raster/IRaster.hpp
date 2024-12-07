#pragma once

#include <stddef.h>
#include <algorithm>
#include <memory>
#include <stdexcept>
#include <iostream>
#include <KiLib/Exceptions/NotImplemented.hpp>
#include <KiLib/Raster/Cell.hpp>
#include <functional>
#include <iterator>

namespace KiLib::Rasters
{
    template<typename T>
    class IRaster
    {
    public:

        using iterator_category = std::input_iterator_tag;
        using value_type = T;
        using difference_type = size_t;
        using pointer = const T*;
        using reference = T;

        IRaster() {}
        virtual ~IRaster() {}
        
        virtual Cell<T> get(double x, double y) const = 0;

        virtual Cell<T> get(size_t i, size_t j) const = 0;
       
        virtual void set(double i, double j, const T& value) = 0;
        virtual void set(size_t i, size_t j, const T& value) = 0;

        virtual size_t getNRows() const
        {
            return rows;
        }

        virtual size_t getNCols() const
        {
            return cols;
        }

        virtual KiLib::Rasters::Cell<T> at(size_t row, size_t col)
        {
            return get(row, col);
        }

        virtual KiLib::Rasters::Cell<T> at(double x, double y)
        {
            return get(x, y);
        }

        virtual KiLib::Rasters::Cell<T> at(double x, double y) const
        {
            return get(x, y);
        }


        virtual KiLib::Rasters::Cell<T> operator()(size_t row, size_t col)
        {
            return get(row, col);
        }

        virtual KiLib::Rasters::Cell<T> operator()(size_t row, size_t col) const
        {
            return get(row, col);
        }

        virtual double get_xllcorner() const
        {
            return this->xllcorner;
        }
        virtual double get_yllcorner() const
        {
            return this->yllcorner;
        }
        virtual double get_cellsize() const
        {
            return this->cellsize;
        }
        virtual double get_width() const
        {
            return this->width;
        }
        virtual double get_height() const
        {
            return this->height;
        }
        virtual double get_nodata_value() const
        {
            return this->nodata_value;
        }

        virtual size_t get_rows() const
        {
            return this->rows;
        }

        virtual size_t get_cols() const
        {
            return this->cols;
        }

        virtual size_t get_ndata() const = 0;

        virtual size_t flatten_index(size_t r, size_t c) const
        {
            return r * this->cols + c;
        }

    private:
        size_t rows, cols;
        double xllcorner;    // Lower left corner x value in absolute coordinates
        double yllcorner;    // Lower left corner y value in absolute coordinates
        double cellsize;     // [m] Distance between values
        double width;        // [m] Width in X
        double height;       // [m] Height in Y
        double nodata_value; // Value associated with no data from DEM

    };
}
