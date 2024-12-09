
#pragma once

#include <algorithm>
#include <cmath>
#include <string>
#include <KiLib/Utils/Attribute.hpp>
#include <KiLib/Utils/SafeDouble.hpp>
#include <KiLib/Utils/Vec2.hpp>

// Forward declare the class template
namespace KiLib::Rasters {
   template<typename T> class IRaster;
};

namespace KiLib::Rasters
{
   // Forward declare the class template
   template<typename T> class Cell;

   // Declare the friend in the class defintion
   template<typename T>
   class Cell
   {
   public:

      static double norm_xy(const Cell &a);
      static double norm_ij(const Cell &a);

      SafeDouble _x, _y;       // The real position on the map cooresponding to the i,j
      Attribute<size_t> _i, _j;       // the row/col position
      const KiLib::Rasters::IRaster<T>& parent_raster;
      const T* data;         // The data at the cell
      bool is_nodata = false;
      
      
      Cell(){};
      Cell(const Cell<T>& in) : parent_raster(in.parent_raster)
      {
         this->i(in.i());
         this->j(in.j());
         data = in.data;
         is_nodata = in.is_nodata;
      }

      Cell(const KiLib::Rasters::IRaster<T>& parent_raster, size_t i, size_t j) : parent_raster(parent_raster)
      {
         this->i(i);
         this->j(j);
         this->is_nodata = true;
         this->data = NULL;
      }

      Cell(const KiLib::Rasters::IRaster<T>& parent_raster, size_t i, size_t j, const T& data) : parent_raster(parent_raster)
      {
         this->i(i);
         this->j(j);
         this->data = &data;
      }

      Cell(const KiLib::Rasters::IRaster<T>& parent_raster, size_t i, size_t j, T& data) : parent_raster(parent_raster)
      {
         this->i(i);
         this->j(j);
         this->data = &data;
      }

      Cell(const KiLib::Rasters::IRaster<T>& parent_raster, double x, double y, T& data) : parent_raster(parent_raster)
      {
         this->_x = x;
         this->_y = y;
         this->data = &data;
      }

      KiLib::Vec2<double> get_cell_position(size_t ind) const
      {
         size_t r = ind / this->_nCols(); //TODO: Should this be rows???
         size_t c = ind % this->_nCols();
         return get_cell_position(r, c);
      }

      KiLib::Vec2<double> get_cell_position(size_t row,  size_t col) const
      {
         KiLib::Vec2<double> pos = KiLib::Vec2<double>(this->parent_raster._xllcorner() + col * this->parent_raster._cellsize()  + this->parent_raster._cellsize() / 2.0, this->parent_raster._yllcorner() + row * this->parent_raster._cellsize()  + this->parent_raster._cellsize() / 2.0);
         return pos;
      }

      double get_cell_position_x(size_t col) const
      {
         return this->parent_raster.get_xllcorner() + (double)col * this->parent_raster.get_cellsize()  + this->parent_raster.get_cellsize() / 2.0;
      }

      double get_cell_position_y(size_t row) const
      {
         return this->parent_raster.get_yllcorner() + (double)row * this->parent_raster.get_cellsize()  + this->parent_raster.get_cellsize() / 2.0;
      }

      KiLib::Vec2<size_t> get_nearest_cell(const Cell &cell) const
      {
          return {get_nearest_row(cell), get_nearest_col(cell)};
      }

      size_t get_nearest_row(const Cell& cell) const
      {
         double rF = (cell.y() - cell.parent_raster.get_yllcorner()) / cell.parent_raster.get_cellsize();
         return std::clamp<size_t>((size_t)std::floor(rF), 0UL, cell.parent_raster._nRows() - 1);
      }

      size_t get_nearest_col(const Cell& cell) const
      {
         double cF = (cell.x() - cell.parent_raster.get_xllcorner()) / cell.parent_raster.get_cellsize();
         return std::clamp<size_t>((size_t)std::floor(cF), 0UL, cell.parent_raster._nCols() - 1);
      }

      size_t i() const
      {
         return _i();
      }

      

      size_t j() const
      {
         return _j();
      }

      

      double x() const
      {
         return _x();
      }


      double y() const
      {
         return _y();
      }

      

      /**
       * @brief Return string representation of this vector
       *
       * @return std::string
       */
      std::string toString() const
      {
         return fmt::format("Real: ({}, {})\tGrid: ({}, {})", this->x(), this->y(), this->i(), this->j());
      }

      void operator()(const Cell& in)
      {
         this->_x = in.x();
         this->_y = in.y();
         this->_i = in.i();
         this->_j = in.j();
         this->data = in.data;
      }

      double getInterpBilinear(std::function<double(const T&)> selector) const
      {
         return getInterpBilinear(*this, selector);
      }

      // Returns (bilinear) interpolated data value at specified position
      // Takes in a vec3 for convenience, ignores Z
      static double getInterpBilinear(const Cell &pos, std::function<double(const T&)> selector)
      {
         const double x = (pos.x() - pos.parent_raster.get_xllcorner()) / pos.parent_raster.get_cellsize();
         const double y = (pos.y() - pos.parent_raster.get_yllcorner()) / pos.parent_raster.get_cellsize();

         const size_t r = std::min(static_cast<size_t> (std::floor(y)), pos.parent_raster.get_rows() - 1ul);
         const size_t c = std::min(static_cast<size_t> (std::floor(x)), pos.parent_raster.get_cols() - 1ul);
         
         const size_t ru = std::min(r + 1ul, pos.parent_raster.get_rows() - 1ul);
         const size_t cr = std::min(c + 1ul, pos.parent_raster.get_cols() - 1ul);

         const double f00 = selector(*(pos.parent_raster.get(r, c).data));
         const double f10 = selector(*(pos.parent_raster.get(r, cr).data));
         const double f01 = selector(*(pos.parent_raster.get(ru, c).data));
         const double f11 = selector(*(pos.parent_raster.get(ru, cr).data));

         const double sx = x - std::floor(x);
         const double sy = y - std::floor(y);

         const double val = f00 * (1.0 - sx) * (1.0 - sy) + f10 * sx * (1.0 - sy) + f01 * (1.0 - sx) * sy + f11 * sx * sy;

         return val;
      }

      

      KiLib::Vec3 getCellCenter(size_t ind) const
      {
         size_t r = ind / this->nCols;
         size_t c = ind % this->nCols;

         KiLib::Vec3 pos = KiLib::Vec3(
            this->xllcorner + c * this->cellsize + this->cellsize / 2.0,
            this->yllcorner + r * this->cellsize + this->cellsize / 2.0, 0);
         pos.z = this->getInterpBilinear(pos);

         return pos;
      }

      operator KiLib::Vec2<double>() const 
      {
         return KiLib::Vec2<double>(x(), y());
      }

      operator KiLib::Vec2<size_t>() const 
      {
         return KiLib::Vec2<size_t>(i(), j());
      }
   
   #define TOLERANCE 1.0E-09

bool IsClose(double A, double B)
{
    if (A == B)
        return true;
    if (std::fabs((A - B) / B) <= TOLERANCE)
        return true;
    return false;
}

      bool IsPointWithinRadius(const Vec2<double> &p, double radius) const
      {

         const double distance = std::sqrt(std::pow(p.x() - x(), 2) + std::pow(p.y() - y(), 2));
         if (distance < radius || IsClose(distance, radius))
           return true;

         return false;

      }


      private:

      void i(size_t value)
      {
         _i(value);

         // Update x
         _y = get_cell_position_y(value);
      }

      void j(size_t value)
      {
         _j(value);

         // Update y
         _x = get_cell_position_x(value);
      }

      void x(double value)
      {
         _x(value);

         // Set i
         _i = get_nearest_row(*this); 

      }

      void y(double value)
      {
         _y(value);

          // Set j
         _j = get_nearest_col(*this); 
      }     



   };

   /**
    * @brief Returns 2-norm of a in real coordinates
    *
    * @param a
    * @return double
    */
   template<typename T>
   inline double Cell<T>::norm_xy(const Cell<T> &a)
   {
      return std::sqrt(a.x() * a.x() + a.y() * a.y());
   }

    /**
    * @brief Returns 2-norm of a in grid coordinates
    *
    * @param a
    * @return double
    */
   template<typename T>
   inline double Cell<T>::norm_ij(const Cell<T> &a)
   {
      return std::sqrt(a.x() * a.x() + a.y() * a.y());
   }


}; // namespace KiLib::Rasters
