#pragma once

#include <stddef.h>
#include <algorithm>
#include <memory>
#include <stdexcept>
#include <KiLib/Raster/Raster.hpp>
#include <bankformap/utils/cell.hpp>
#include <iostream>
#include <functional>
#include <bankformap/utils/iraster.hpp>
#include <map>

namespace BankforMAP::Rasters
{
    template<typename T>
    class DOKSparseRaster : public IRaster<T>
    {
    public:
        DOKSparseRaster() {
            DOKSparseRaster::nnz = 0;
        }
        DOKSparseRaster(const KiLib::Raster& from_raster, double threshold, T default_value, std::function<void(T& obj, double value)> set_cell) : rows(from_raster.nRows), cols(from_raster.nCols)
        {
            // get NNZ
            DOKSparseRaster::nnz = 0;
            for (size_t row = 0; row < from_raster.nRows; row++)
            {
                for (size_t col = 0; col < from_raster.nCols; col++)
                {
                    if (from_raster.at(row, col) != from_raster.nodata_value && from_raster.at(row, col) > threshold)
                    {
                        T d = default_value;
                        set_cell(d, from_raster.at(row,col));
                        set(row, col, d);
                    }
                }
            }

            this->xllcorner = from_raster.xllcorner;
            this->yllcorner = from_raster.yllcorner;
            this->cellsize = from_raster.cellsize;
            this->width = from_raster.width;
            this->height = from_raster.height;
            this->nodata_value = from_raster.nodata_value;
        }

        ~DOKSparseRaster()
        {
            auto it = data.begin();
 
            // Iterate through the map and print the elements
            while (it != data.end()) {
                delete it->second;
                ++it;
            }
        }

        void LoadRaster(const KiLib::Raster& from_raster, double threshold, std::function<void(T& obj, double value)> set_cell)
        {
            for (size_t row = 0; row < from_raster.nRows; row++)
            {
                for (size_t col = 0; col < from_raster.nCols; col++)
                {
                    if (from_raster.at(row, col) > threshold)
                    {
                        auto cell = get(row, col);

                        if (cell.is_nodata) {
                            T d;
                            set_cell(d, from_raster.at(row,col));
                            set(row, col, d);
                        } else {
                            set_cell(cell.data, from_raster.at(row,col));
                        }
                    }
                }
            }
        }

        BankforMAP::Utils::Cell<T> get(double x, double y) const override
        {
            double rF = (y - this->yllcorner) / this->cellsize;
            double cF = (x - this->xllcorner) / this->cellsize;

            size_t r = std::clamp<size_t>((size_t)std::floor(rF), 0UL, this->get_rows() - 1);
            size_t c = std::clamp<size_t>((size_t)std::floor(cF), 0UL, this->get_cols() - 1);

            return get(r, c);
        }

        BankforMAP::Utils::Cell<T> get(size_t i, size_t j) const override
        {
            try {
                T* d = data.at({i, j});
               return BankforMAP::Utils::Cell<T>(*this, i, j, *d);
            } catch (std::out_of_range ex) {
                return BankforMAP::Utils::Cell<T>(*this, i, j);
            }
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
            try {
                *(data.at({i, j})) = value;
            } catch (std::out_of_range ex) {
                nnz++;

                // Create new object...
                T* d = new T(value);
                data.insert_or_assign({i, j}, d);
            }
        }

        bool is_valid_cell(double x, double y) const
        {
            double rF = (y - this->yllcorner) / this->cellsize;
            double cF = (x - this->xllcorner) / this->cellsize;

            size_t r = std::clamp<size_t>((size_t)std::floor(rF), 0UL, this->get_rows() - 1);
            size_t c = std::clamp<size_t>((size_t)std::floor(cF), 0UL, this->get_cols() - 1);

            return is_valid_cell(r, c);
        }

        bool is_valid_cell(size_t i, size_t j) const
        {
            try {
                data.at({i, j});
                return true;
            } catch (std::out_of_range ex) {
                return false;
            }
        }

        size_t getNRows() const override
        {
            return rows;
        }

        size_t getNCols() const override
        {
            return cols;
        }

        BankforMAP::Utils::Cell<T> at(size_t row, size_t col) const override
        {
            return get(row, col);
        }

        BankforMAP::Utils::Cell<T> operator()(size_t row, size_t col) const override
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

        /**
         * @brief Return a random point within the raster.
         * 
         * @param gen The generator to use for random numbers
         * @return KiLib::Vec3 The point. Dependent on implementation
         */
        KiLib::Vec3 randPoint(std::mt19937_64 &gen, std::function<double(const T&)> selector) const override
        {
            KiLib::Vec3                            point;
            std::uniform_real_distribution<double> xDist(this->xllcorner, this->xllcorner + this->width);
            std::uniform_real_distribution<double> yDist(this->yllcorner, this->yllcorner + this->height);

            point.x = xDist(gen);
            point.y = yDist(gen);
            point.z = get(point.x, point.y).getInterpBilinear(selector ); // for terrain?

            return point;
        }

        struct Iterator {
            // Always start from the beginning
            Iterator(const DOKSparseRaster<T>& raster, typename std::map<std::pair<int, int>, T*>::const_iterator it) : raster(raster), it(it) {
            
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
            BankforMAP::Utils::Cell<T> operator*() const
            {
                // first = the pair
                // first/second = the coordinates inside the pair
                return raster.get((size_t)(*it).first.first, (size_t)(*it).first.second);
            }

        private:
            const DOKSparseRaster<T>& raster;
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

    private:
        size_t rows, cols, nnz;
        T default_value;
        std::map<std::pair<int, int>, T*> data;
        double xllcorner;    // Lower left corner x value in absolute coordinates
        double yllcorner;    // Lower left corner y value in absolute coordinates
        double cellsize;     // [m] Distance between values
        double width;        // [m] Width in X
        double height;       // [m] Height in Y
        double nodata_value; // Value associated with no data from DEM

    };
}