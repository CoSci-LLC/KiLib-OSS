#pragma once
#include <algorithm>
#include <cmath>
#include <string>

// Required for math constants on Windows
#ifdef _WINDOWS
#include <corecrt_math_defines.h>
#endif


namespace KiLib
{

   // NOTE: A lot of stuff is not implemented (*=, /=, etc). Feel free to implement
   // NOTE: A lot of stuff is not implemented (*=, /=, etc). Feel free to implement
   // NOTE: A lot of stuff is not implemented (*=, /=, etc). Feel free to implement
   // NOTE: A lot of stuff is not implemented (*=, /=, etc). Feel free to implement
   // NOTE: A lot of stuff is not implemented (*=, /=, etc). Feel free to implement
   // NOTE: A lot of stuff is not implemented (*=, /=, etc). Feel free to implement
   // NOTE: A lot of stuff is not implemented (*=, /=, etc). Feel free to implement
   // NOTE: A lot of stuff is not implemented (*=, /=, etc). Feel free to implement
   // NOTE: A lot of stuff is not implemented (*=, /=, etc). Feel free to implement
   // NOTE: A lot of stuff is not implemented (*=, /=, etc). Feel free to implement

   /**
    * @brief Simple vector class with x, y, and z components. Implements various arithmetic and
    * linear algebra (dot/cross products, L2 norm, etc) operations.
    *
    */
   class Vec3
   {
   public:
      double x;
      double y;
      double z;

      Vec3(){};

      Vec3(double x, double y, double z)
      {
         this->x = x;
         this->y = y;
         this->z = z;
      }

      /**
       * @brief Return string representation of this vector
       *
       * @return std::string
       */
      std::string toString() const
      {
         return "(" + std::to_string(this->x) + "," + std::to_string(this->y) + "," + std::to_string(this->z) + ")";
      };

      static Vec3 cross(const Vec3 &a, const Vec3 &b);
      static Vec3 normalise(const Vec3 &a);
      static double norm(const Vec3 &a);
      static double angle2D(const Vec3 &a);
      static double dot(const Vec3 &a, const Vec3 &b);

      friend Vec3 operator+(const Vec3 &lhs, const Vec3 &rhs);
      friend Vec3 operator-(const Vec3 &lhs, const Vec3 &rhs);
      friend Vec3 operator*(const Vec3 &lhs, const Vec3 &rhs);
      friend Vec3 operator*(const Vec3 &lhs, const double rhs);
      friend Vec3 operator*(const double lhs, const Vec3 &rhs);
      friend Vec3 operator/(const Vec3 &lhs, const double rhs);
      friend Vec3 &operator+=(Vec3 &lhs, const Vec3 &rhs);
   };

   /**
    * @brief Cross product between two vectors
    *
    * @param a
    * @param b
    * @return Vec3
    */
   inline Vec3 Vec3::cross(const Vec3 &a, const Vec3 &b)
   {
      return {a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x};
   }

   /**
    * @brief Returns an L2 normalized version of vector
    *
    * @param a
    * @return Vec3
    */
   inline Vec3 Vec3::normalise(const Vec3 &a)
   {
      double norm = Vec3::norm(a);
      if (norm == 0)
         return a;

      return a / norm;
   }

   /**
    * @brief Returns 2-norm of a
    *
    * @param a
    * @return double
    */
   inline double Vec3::norm(const Vec3 &a)
   {
      return std::sqrt(a.x * a.x + a.y * a.y + a.z * a.z);
   }

   /**
    * @brief Returns dot product of a and b vectors
    *
    * @param a
    * @param b
    * @return double
    */
   inline double Vec3::dot(const Vec3 &a, const Vec3 &b)
   {
      return a.x * b.x + a.y * b.y + a.z * b.z;
   }

   inline Vec3 operator+(const Vec3 &lhs, const Vec3 &rhs)
   {
      return {lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z};
   }

   inline Vec3 operator-(const Vec3 &lhs, const Vec3 &rhs)
   {
      return {lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z};
   }

   inline Vec3 operator*(const Vec3 &lhs, const Vec3 &rhs)
   {
      return {lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z};
   }

   inline Vec3 operator*(const Vec3 &lhs, const double rhs)
   {
      return {lhs.x * rhs, lhs.y * rhs, lhs.z * rhs};
   }

   inline Vec3 operator*(const double lhs, const Vec3 &rhs)
   {
      return {lhs * rhs.x, lhs * rhs.y, lhs * rhs.z};
   }

   inline Vec3 operator/(const Vec3 &lhs, const double rhs)
   {
      return {lhs.x / rhs, lhs.y / rhs, lhs.z / rhs};
   }

   inline Vec3 &operator+=(Vec3 &lhs, const Vec3 &rhs)
   {
      lhs.x += rhs.x;
      lhs.y += rhs.y;
      lhs.z += rhs.z;

      return lhs;
   }

   inline Vec3 &operator-=(Vec3 &lhs, const Vec3 &rhs)
   {
      lhs.x -= rhs.x;
      lhs.y -= rhs.y;
      lhs.z -= rhs.z;

      return lhs;
   }


   /**
    * @brief Returns two-dimensional angle of this vector
    *
    * @param vec
    * @return double
    */
   inline double Vec3::angle2D(const Vec3 &vec)
   {
      const double at = atan2(vec.x, vec.y);
      if (at < 0)
         return -at;
      else
         return (M_PI * 2.0) - at;
   }

} // namespace KiLib