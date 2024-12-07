#pragma once

#include <stddef.h>
#include <algorithm>
#include <memory>
#include <stdexcept>
#include <KiLib/Raster/Raster.hpp>
#include <bankformap/utils/iraster.hpp>
#include <bankformap/utils/cell.hpp>
#include <iostream>
#include <functional>

namespace BankforMAP::Rasters 
{
    template<typename T>
    class SparseRaster : public IRaster<T>
    {
    public:
        SparseRaster() {}
        SparseRaster(const KiLib::Raster& from_raster, double threshold, T default_value, std::function<void(T& obj, double value)> set_cell) : rows(from_raster.nRows), cols(from_raster.nCols)
        {
            // get NNZ
            SparseRaster::nnz = 0;
            for (size_t row = 0; row < from_raster.nRows; row++)
            {
                for (size_t col = 0; col < from_raster.nCols; col++)
                {
                    if (from_raster.at(row, col) > threshold)
                    {
                        nnz++; // get a total amount of cells we will be processing
                    }
                }
            }

            this->V = new T[nnz];
            std::fill_n(V, nnz, default_value);

            this->COL_INDEX = new size_t[nnz];
            this->ROW_INDEX = new size_t[rows + 1];
            this->default_value = default_value;

            // import nnz values into the new system
            size_t row_index = 0;
            size_t v_index = 0;

            for (size_t row = 0; row < from_raster.nRows; row++)
            {
                ROW_INDEX[row_index] = v_index;
                for (size_t col = 0; col < from_raster.nCols; col++)
                {
                    if (from_raster.at(row, col) > threshold)
                    {
                        // Put value into the value array
                        set_cell(V[v_index], from_raster.at(row,col));

                        //Link col_index to V
                        COL_INDEX[v_index] = col;
                        
                        // increase the next index
                        v_index++;
                    }
                }
                row_index++;
            }

            ROW_INDEX[row_index] = v_index;

            this->xllcorner = from_raster.xllcorner;
            this->yllcorner = from_raster.yllcorner;
            this->cellsize = from_raster.cellsize;
            this->width = from_raster.width;
            this->height = from_raster.height;
            this->nodata_value = from_raster.nodata_value;
        }

        SparseRaster(const BankforMAP::Rasters::IRaster<T>& from_raster) : rows(from_raster.get_rows()), cols(from_raster.get_cols())
        {
            // get NNZ
            SparseRaster::nnz = from_raster.get_ndata();

            this->V = new T[nnz];
            this->COL_INDEX = new size_t[nnz];
            this->ROW_INDEX = new size_t[rows + 1];

            // import nnz values into the new system
            size_t row_index = 0;
            size_t v_index = 0;

            for (size_t row = 0; row < from_raster.get_rows(); row++)
            {
                ROW_INDEX[row_index] = v_index;
                for (size_t col = 0; col < from_raster.get_cols(); col++)
                {
                    if (!from_raster.get(row, col).is_nodata)
                    {
                        // Put value into the value array
                        V[v_index] = *(from_raster.at(row, col).data);

                        //Link col_index to V
                        COL_INDEX[v_index] = col;
                        
                        // increase the next index
                        v_index++;
                    }
                }
                row_index++;
            }

            ROW_INDEX[row_index] = v_index;

            this->xllcorner = from_raster.get_xllcorner();
            this->yllcorner = from_raster.get_yllcorner();
            this->cellsize = from_raster.get_cellsize();
            this->width = from_raster.get_width();
            this->height = from_raster.get_height();
            this->nodata_value = from_raster.get_nodata_value();
        }


        ~SparseRaster() 
        {
            if (V != NULL) {
                delete V;
                delete COL_INDEX;
                delete ROW_INDEX;
                V = NULL;
            }
        }

        


        BankforMAP::Utils::Cell<T> get(double x, double y) const 
        {
            double rF = (y - this->yllcorner) / this->cellsize;
            double cF = (x - this->xllcorner) / this->cellsize;

            size_t r = std::clamp<size_t>((size_t)std::floor(rF), 0UL, this->get_rows() - 1);
            size_t c = std::clamp<size_t>((size_t)std::floor(cF), 0UL, this->get_cols() - 1);

            return get(r, c);
        }

        BankforMAP::Utils::Cell<T> get(double x, double y)
        {
            double rF = (y - this->yllcorner) / this->cellsize;
            double cF = (x - this->xllcorner) / this->cellsize;

            size_t r = std::clamp<size_t>((size_t)std::floor(rF), 0UL, this->get_rows() - 1);
            size_t c = std::clamp<size_t>((size_t)std::floor(cF), 0UL, this->get_cols() - 1);

            return get(r, c);
        }


        BankforMAP::Utils::Cell<T> get(int i, int j) const
        {
            return get((size_t)i, (size_t)j);
        }

        BankforMAP::Utils::Cell<T> get(int i, int j)
        {
            return get((size_t)i, (size_t)j);
        }

        BankforMAP::Utils::Cell<T> get(size_t i, size_t j) const
        {
            // Get row
            size_t row_start = ROW_INDEX[i];
            const size_t row_end = ROW_INDEX[i + 1];

            //No data? Return default data
            if (row_start == row_end)
                return BankforMAP::Utils::Cell<T>(*this, i, j);

            for (; row_start < row_end; row_start++ ) {
                if (COL_INDEX[row_start] == j) {
                    return BankforMAP::Utils::Cell<T>(*this, i, j, V[row_start]);
                }
                else if (COL_INDEX[row_start] > j) { //We have gone too far and won't find anything now.
                    return BankforMAP::Utils::Cell<T>(*this, i, j);
                }
            }
            return BankforMAP::Utils::Cell<T>(*this, i, j);;   
        }

        BankforMAP::Utils::Cell<T> get(size_t i, size_t j)
        {
            // Get row
            size_t row_start = ROW_INDEX[i];
            const size_t row_end = ROW_INDEX[i + 1];

            //No data? Return default data
            if (row_start == row_end)
                return BankforMAP::Utils::Cell<T>(*this, i, j);

            for (; row_start < row_end; row_start++ ) {
                if (COL_INDEX[row_start] == j) {
                    return BankforMAP::Utils::Cell<T>(*this, i, j, V[row_start]);
                }
                else if (COL_INDEX[row_start] > j) { //We have gone too are and won't find anything now.
                    return BankforMAP::Utils::Cell<T>(*this, i, j);
                }
            }
            return BankforMAP::Utils::Cell<T>(*this, i, j);
        }

        void set(size_t i, size_t j, const T& value) 
        {
            // Get row
            size_t row_start = ROW_INDEX[i];
            const size_t row_end = ROW_INDEX[i + 1];

            //No data? Return default data
            if (row_start == row_end)
                throw std::runtime_error("no data found because row start/end the same");

            for (; row_start < row_end; row_start++ ) {
                if (COL_INDEX[row_start] == j) {
                    V[row_start] = value;
                    return;
                }
            }
            throw std::runtime_error("no data found at all...");
        }

        void set(double x, double y, const T& value) 
        {
            // Get row
            const double rF = (y - this->yllcorner) / this->cellsize;
            const double cF = (x - this->xllcorner) / this->cellsize;

            size_t i = std::clamp<size_t>((size_t)std::floor(rF), 0UL, this->get_rows() - 1);
            size_t j = std::clamp<size_t>((size_t)std::floor(cF), 0UL, this->get_cols() - 1);
            
            size_t row_start = ROW_INDEX[i];
            const size_t row_end = ROW_INDEX[i + 1];

            //No data? Return default data
            if (row_start == row_end)
                throw std::runtime_error("no data found because row start/end the same");

            for (; row_start < row_end; row_start++ ) {
                if (COL_INDEX[row_start] == j) {
                    V[row_start] = value;
                    return;
                }
            }
            throw std::runtime_error("no data found at all...");
        }

        void to_string(std::function<std::string(const T&)> to_string)
        {
            std::cout << "V: [";
            for (size_t i = 0; i < nnz; i++) {
                std::cout << to_string(V[i]) << ',';
            }
            std::cout << "]" << std::endl;

            std::cout << "COL_INDEX: [";
            for (size_t i = 0; i < nnz; i++) {
                std::cout << COL_INDEX[i] << ',';
            }
            std::cout << "]" << std::endl;

            std::cout << "ROW_INDEX: [";
            for (size_t i = 0; i < rows + 1; i++) {
                std::cout << ROW_INDEX[i] << ',';
            }
            std::cout << "]" << std::endl;
        }

        size_t getNRows() const
        {
            return rows;
        }

        size_t getNCols() const
        {
            return cols;
        }

        double operator()(const KiLib::Vec3 &) const
        {
            //return this->getInterpBilinear(pos);
            throw std::runtime_error("Not implemented");
        }

        BankforMAP::Utils::Cell<T> at(size_t row, size_t col)
        {
            return get(row, col);
        }

        BankforMAP::Utils::Cell<T> at(size_t row, size_t col) const
        {
            return get(row, col);
        }

        BankforMAP::Utils::Cell<T> operator()(size_t row, size_t col)
        {
            return get(row, col);
        }

        BankforMAP::Utils::Cell<T> operator()(size_t row, size_t col) const
        {
            return get(row, col);
        }

        double get_xllcorner() const
        {
            return this->xllcorner;
        }
        double get_yllcorner() const
        {
            return this->yllcorner;
        }
        double get_cellsize() const
        {
            return this->cellsize;
        }
        double get_width() const
        {
            return this->width;
        }
        double get_height() const
        {
            return this->height;
        }
        double get_nodata_value() const
        {
            return this->nodata_value;
        }

        size_t get_rows() const
        {
            return this->rows;
        }

        size_t get_cols() const
        {
            return this->cols;
        }

        size_t get_ndata() const
        {
            return this->nnz;
        }

        size_t flatten_index(size_t r, size_t c) const
        {
            return r * this->cols + c;
        }

        struct Iterator{
            friend class SparseRaster<T>;

            // Always start from the beginning
            Iterator(const SparseRaster<T>& raster, size_t index) :  index(index), cur_row(0), raster(raster)  {
            }

            // incremeneting means going through the list
            Iterator& operator++() 
            {
                if (index < raster.nnz)
                {
                    index++;
                }
                return *this;
            }


            // post fixing is bad in general but is has it's usages
            Iterator operator++(int) 
            {
                Iterator tempIter = *this; // we make a copy of the iterator
                ++*this; // we increment
                return tempIter; // we return the copy before increment
            }


            // we need to be able to compare nodes
            bool operator!=(const Iterator& other) const  
            {
                return index != other.index;
            }


            // return the data from the node (dereference operator)
            BankforMAP::Utils::Cell<T> operator*() 
            {
                for (size_t i = 0; i < raster.rows + 1; i++) {
                    if (raster.ROW_INDEX[cur_row] <= index) {
                        cur_row++;
                    } else {
                        cur_row -= 1;
                        break;
                    }
                }
                return raster.get(cur_row, raster.COL_INDEX[index]);
            }

        private:
            size_t index;
            size_t cur_row;
            const SparseRaster<T>& raster;
        };

        // returning a const pointer to the front
        Iterator begin() const noexcept
        {
            return Iterator(*this, 0);
        };

        // returning a const pointer to the back - the back is always null because it marks the end of the list
        Iterator end() const noexcept
        {
            return Iterator(*this, nnz);
        };


    private:
        size_t rows, cols, nnz;
        T default_value;
        T *V;
        size_t *COL_INDEX, *ROW_INDEX;
        double xllcorner;    // Lower left corner x value in absolute coordinates
        double yllcorner;    // Lower left corner y value in absolute coordinates
        double cellsize;     // [m] Distance between values
        double width;        // [m] Width in X
        double height;       // [m] Height in Y
        double nodata_value; // Value associated with no data from DEM

    };
}