/**
 *  Copyright (c) 2020-2021 CoSci LLC, USA <software@cosci-llc.com>
 *
 *  This file is part of KiLib-OSS.
 *
 *  KiLib-OSS is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  KiLib-OSS is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with KiLib-OSS.  If not, see <https://www.gnu.org/licenses/>.
 */


#pragma once
#define _USE_MATH_DEFINES
#include <cmath>
#include <spdlog/spdlog.h>
#include <string>

namespace KiLib
{
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

      Vec3(double x, double y, double z) : x(x), y(y), z(z){};

      /**
       * @brief Return string representation of this vector
       *
       * @return std::string
       */
      std::string toString() const
      {
         return fmt::format("({}, {}, {})", this->x, this->y, this->z);
      }

      static Vec3   cross(const Vec3 a, const Vec3 b);
      static Vec3   normalise(const Vec3 a);
      static double norm(const Vec3 a);
      static double NormXY(const Vec3 a);
      static double angle2D(const Vec3 a);
      static double dot(const Vec3 a, const Vec3 b);

      ////////////////////////////////////////////////////////
      // Addition
      ////////////////////////////////////////////////////////
      friend Vec3  operator+(const Vec3 lhs, const Vec3 rhs);
      friend Vec3  operator+(const Vec3 lhs, const double rhs);
      friend Vec3  operator+(const double lhs, const Vec3 rhs);
      friend Vec3 &operator+=(Vec3 &lhs, const Vec3 rhs);
      friend Vec3 &operator+=(Vec3 &lhs, double rhs);

      ////////////////////////////////////////////////////////
      // Subtraction
      ////////////////////////////////////////////////////////
      friend Vec3  operator-(const Vec3 lhs, const Vec3 rhs);
      friend Vec3  operator-(const Vec3 lhs, const double rhs);
      friend Vec3  operator-(const double lhs, const Vec3 rhs);
      friend Vec3 &operator-=(Vec3 &lhs, const Vec3 rhs);
      friend Vec3 &operator-=(Vec3 &lhs, double rhs);

      ////////////////////////////////////////////////////////
      // Multiplication
      ////////////////////////////////////////////////////////
      friend Vec3  operator*(const Vec3 lhs, const Vec3 rhs);
      friend Vec3  operator*(const Vec3 lhs, const double rhs);
      friend Vec3  operator*(const double lhs, const Vec3 rhs);
      friend Vec3 &operator*=(Vec3 &lhs, const Vec3 rhs);
      friend Vec3 &operator*=(Vec3 &lhs, double rhs);

      ////////////////////////////////////////////////////////
      // Division
      ////////////////////////////////////////////////////////
      friend Vec3  operator/(const Vec3 lhs, const Vec3 rhs);
      friend Vec3  operator/(const Vec3 lhs, const double rhs);
      friend Vec3  operator/(const double lhs, const Vec3 rhs);
      friend Vec3 &operator/=(Vec3 &lhs, const Vec3 rhs);
      friend Vec3 &operator/=(Vec3 &lhs, double rhs);

      ////////////////////////////////////////////////////////
      // Equality
      ////////////////////////////////////////////////////////
      friend bool operator==(const Vec3 lhs, const Vec3 rhs);
      friend bool operator!=(const Vec3 lhs, const Vec3 rhs);
   };

   /**
    * @brief Cross product between two vectors
    *
    * @param a
    * @param b
    * @return Vec3
    */
   inline Vec3 Vec3::cross(const Vec3 a, const Vec3 b)
   {
      return {a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x};
   }

   /**
    * @brief Returns an L2 normalized version of vector
    *
    * @param a
    * @return Vec3
    */
   inline Vec3 Vec3::normalise(const Vec3 a)
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
   inline double Vec3::norm(const Vec3 a)
   {
      return std::sqrt(a.x * a.x + a.y * a.y + a.z * a.z);
   }

   /**
    * @brief Returns XY-only 2-norm of a
    *
    * @param a
    * @return double
    */
   inline double Vec3::NormXY(const Vec3 a)
   {
      return std::sqrt(a.x * a.x + a.y * a.y);
   }

   /**
    * @brief Returns dot product of a and b vectors
    *
    * @param a
    * @param b
    * @return double
    */
   inline double Vec3::dot(const Vec3 a, const Vec3 b)
   {
      return a.x * b.x + a.y * b.y + a.z * b.z;
   }

   /**
    * @brief Returns two-dimensional angle of this vector
    *
    * @param vec
    * @return double
    */
   inline double Vec3::angle2D(const Vec3 vec)
   {
      const double at = atan2(vec.x, vec.y);
      if (at < 0)
         return -at;
      else
         return (M_PI * 2.0) - at;
   }

   ////////////////////////////////////////////////////////
   // Addition
   ////////////////////////////////////////////////////////
   inline Vec3 operator+(const Vec3 lhs, const Vec3 rhs)
   {
      return {lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z};
   }

   inline Vec3 operator+(const Vec3 lhs, const double rhs)
   {
      return {lhs.x + rhs, lhs.y + rhs, lhs.z + rhs};
   }

   inline Vec3 operator+(const double lhs, const Vec3 rhs)
   {
      return rhs + lhs;
   }

   inline Vec3 &operator+=(Vec3 &lhs, const Vec3 rhs)
   {
      lhs.x += rhs.x;
      lhs.y += rhs.y;
      lhs.z += rhs.z;
      return lhs;
   }

   inline Vec3 &operator+=(Vec3 &lhs, double rhs)
   {
      lhs.x += rhs;
      lhs.y += rhs;
      lhs.z += rhs;
      return lhs;
   }

   ////////////////////////////////////////////////////////
   // Subtraction
   ////////////////////////////////////////////////////////
   inline Vec3 operator-(const Vec3 lhs, const Vec3 rhs)
   {
      return {lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z};
   }

   inline Vec3 operator-(const Vec3 lhs, const double rhs)
   {
      return {lhs.x - rhs, lhs.y - rhs, lhs.z - rhs};
   }

   inline Vec3 operator-(const double lhs, const Vec3 rhs)
   {
      return {lhs - rhs.x, lhs - rhs.y, lhs - rhs.z};
   }

   inline Vec3 &operator-=(Vec3 &lhs, const Vec3 rhs)
   {
      lhs.x -= rhs.x;
      lhs.y -= rhs.y;
      lhs.z -= rhs.z;
      return lhs;
   }

   inline Vec3 &operator-=(Vec3 &lhs, double rhs)
   {
      lhs.x -= rhs;
      lhs.y -= rhs;
      lhs.z -= rhs;
      return lhs;
   }

   ////////////////////////////////////////////////////////
   // Multiplication
   ////////////////////////////////////////////////////////
   inline Vec3 operator*(const Vec3 lhs, const Vec3 rhs)
   {
      return {lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z};
   }

   inline Vec3 operator*(const Vec3 lhs, const double rhs)
   {
      return {lhs.x * rhs, lhs.y * rhs, lhs.z * rhs};
   }

   inline Vec3 operator*(const double lhs, const Vec3 rhs)
   {
      return rhs * lhs;
   }

   inline Vec3 &operator*=(Vec3 &lhs, const Vec3 rhs)
   {
      lhs.x *= rhs.x;
      lhs.y *= rhs.y;
      lhs.z *= rhs.z;
      return lhs;
   }

   inline Vec3 &operator*=(Vec3 &lhs, double rhs)
   {
      lhs.x *= rhs;
      lhs.y *= rhs;
      lhs.z *= rhs;
      return lhs;
   }

   ////////////////////////////////////////////////////////
   // Division
   ////////////////////////////////////////////////////////
   inline Vec3 operator/(const Vec3 lhs, const Vec3 rhs)
   {
      return {lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z};
   }

   inline Vec3 operator/(const Vec3 lhs, const double rhs)
   {
      return {lhs.x / rhs, lhs.y / rhs, lhs.z / rhs};
   }

   inline Vec3 operator/(const double lhs, const Vec3 rhs)
   {
      return {lhs / rhs.x, lhs / rhs.y, lhs / rhs.z};
   }

   inline Vec3 &operator/=(Vec3 &lhs, const Vec3 rhs)
   {
      lhs.x /= rhs.x;
      lhs.y /= rhs.y;
      lhs.z /= rhs.z;
      return lhs;
   }

   inline Vec3 &operator/=(Vec3 &lhs, double rhs)
   {
      lhs.x /= rhs;
      lhs.y /= rhs;
      lhs.z /= rhs;
      return lhs;
   }

   ////////////////////////////////////////////////////////
   // Equality
   ////////////////////////////////////////////////////////
   inline bool  operator==(const Vec3 lhs, const Vec3 rhs)
   {
      return (lhs.x == rhs.x) && (lhs.y == rhs.y) && (lhs.z == rhs.z);
   }

   inline bool  operator!=(const Vec3 lhs, const Vec3 rhs)
   {
      return (lhs.x != rhs.x) || (lhs.y != rhs.y) || (lhs.z != rhs.z);
   }
} // namespace KiLib
