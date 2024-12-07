
#pragma once

#include <algorithm>
#include <cmath>
#include <string>
#include <KiLib/Utils/Attribute.hpp>
#include <KiLib/Utils/SafeDouble.hpp>
#include <KiLib/Utils/Vec3.hpp>

namespace KiLib
{

   // Forward declare the class template
   template<typename T> class Vec2;

   // Declare the functions 
   template<typename T> Vec2<T> operator+(const Vec2<T>& lhs, const Vec2<T>& rhs);
   template<typename T> Vec2<T> operator-(const Vec2<T>& lhs, const Vec2<T>& rhs);
   template<typename T> bool operator==(const Vec2<T>& lhs, const Vec2<T>& rhs);


   // Declare the friend in the class defintion
   template <typename T>
   class Vec2
   {
   public:

      static double norm(const Vec2 &a);
      friend Vec2 operator+<T>(const Vec2 &lhs, const Vec2 &rhs);
      friend Vec2 operator-<T>(const Vec2 &lhs, const Vec2 &rhs);
      

      Attribute<T> _x, _y;
      
      Vec2(){};
      Vec2(const Vec2& in)
      {
         this->_x = in.x();
         this->_y = in.y();
      }
      Vec2(const KiLib::Vec3& in)
      {
         this->_x = in.x;
         this->_y = in.y;
      }

      inline Vec2 & operator=(const Vec2& other)
      {
         this->_x = other.x();
         this->_y = other.y();
         return *this;
      }

      Vec2(T x, T y)
      {
         this->_x = x;
         this->_y = y;
      }

      T i() const
      {
         return _x();
      }

      void i(T value)
      {
         _x(value);
      }

      T j() const
      {
         return _y();
      }

      void j(T value)
      {
         _y(value);
      }

      T x() const
      {
         return _x();
      }

      void x(T value)
      {
         _x(value);
      }

      T y() const
      {
         return _y();
      }

      void y(T value)
      {
         _y(value);
      }

      /**
       * @brief Return string representation of this vector
       *
       * @return std::string
       */
      std::string toString() const
      {
         return fmt::format("({}, {})", this->x(), this->y());
      }

      void operator()(const Vec2& in)
      {
         this->_x = in.x();
         this->_y = in.y();
      }

      double distanceFrom(const Vec2 &a) const 
      {
         return std::sqrt( std::pow(a.x() - x(), 2) + std::pow(a.y() - y(), 2) );
      }

      /**
       * @brief Returns XY-only 2-norm of a
       *
       * @param a
       * @return double
       */
      static double NormXY(const Vec2<T> &a)
      {
         return std::sqrt(a.x() * a.x() + a.y() * a.y());
      }

      double NormXY() const
      {
         return Vec2::NormXY(*this);
      }         

   };

   /**
    * @brief Returns 2-norm of a
    *
    * @param a
    * @return double
    */
   template<typename T>
   inline double Vec2<T>::norm(const Vec2 &a)
   {
      return std::sqrt(a.x() * a.x() + a.y() * a.y());
   }

   template<typename T>
   Vec2<T> operator+(const Vec2<T>& lhs, const Vec2<T> &rhs)
   {
      return {lhs.x() + rhs.x(), lhs.y() + rhs.y()};
   }

   template<typename T>
   Vec2<T> operator-(const Vec2<T>& lhs, const Vec2<T> &rhs)
   {
      return {lhs.x() - rhs.x(), lhs.y() - rhs.y()};
   }

   template<typename T>
   inline Vec2<T> operator/(const Vec2<T> &lhs, const double rhs)
   {
      return {lhs.x() / rhs, lhs.y() / rhs};
   }

   template<typename T>
   inline Vec2<T> operator-(const KiLib::Vec3 &lhs, const Vec2<T> &rhs)
   {
      return {lhs.x - rhs.x(), lhs.y - rhs.y(), lhs.z};
   }

   template<typename T>
   inline bool operator==(const Vec2<T> &lhs, const Vec2<T> &rhs)
   {
      return lhs.x() == rhs.x() && lhs.y() == rhs.y();
   }

   
}; // namespace 
