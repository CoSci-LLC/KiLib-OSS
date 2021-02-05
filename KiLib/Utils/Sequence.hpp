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

#include <KiLib/Exceptions/VectorSize.hpp>
#include <algorithm>
#include <iostream>
#include <numeric>
#include <vector>

namespace KiLib
{
   class Sequence
   {
   public:
      template <typename T> static std::vector<T> Build(T, T, T);
      template <typename T> static std::vector<T> BuildEqualSpace(T, T, int);
      template <typename T> static T              min(std::vector<T>);
      template <typename T> static T              max(std::vector<T>);
      template <typename T> static T              sum(std::vector<T>);

   private:
      static bool almostEqualOrLessThan(double a, double b)
      {
         if (a == b)
         {
            return true;
         }
         double delta = 0.00000000001;
         if (a < b + delta)
         {
            if (a > b - delta)
            {
               return true;
            }
            return true;
         }
         return false;
      }
      static bool almostEqualOrGreaterThan(double a, double b)
      {
         if (a == b)
         {
            return true;
         }
         double delta = 0.00000000001;
         if (a > b + delta)
         {
            if (a < b - delta)
            {
               return true;
            }
            return true;
         }
         return false;
      }
   };

   template <typename T> std::vector<T> Sequence::Build(T start, T end, T step)
   {
      std::vector<T> result;
      T              temp = start;
      if (start > end)
      {
         while (almostEqualOrGreaterThan(temp, end))
         {
            result.push_back(temp);
            temp += step;
         }
         return result;
      }
      else
      {
         while (almostEqualOrLessThan(temp, end))
         {
            result.push_back(temp);
            temp += step;
         }
         return result;
      }
   }

   template <typename T> T Sequence::min(std::vector<T> vector)
   {
      if (vector.size() == 0)
      {
         throw VectorSizeException("Sequence::min -> Vector length is invalid");
      }
      return *std::min_element(vector.begin(), vector.end());
   }

   template <typename T> T Sequence::max(std::vector<T> vector)
   {
      if (vector.size() == 0)
      {
         throw VectorSizeException("Sequence::max -> Vector length is invalid");
      }
      return *std::max_element(vector.begin(), vector.end());
   }

   template <typename T> T Sequence::sum(std::vector<T> vector)
   {
      if (vector.size() == 0)
      {
         throw VectorSizeException("Sequence::sum -> Vector length is invalid");
      }

      return std::accumulate(vector.begin(), vector.end(), 0.0);
   }

   template <typename T> std::vector<T> Sequence::BuildEqualSpace(T start, T end, int length)
   {
      std::vector<T> result(length);
      result[0] = start;
      T step    = (end - start) / (double)(length - 1);
      for (int i = 1; i < length; i++)
      {
         result[i] = result[i - 1] + step;
      }
      return result;
   }
} // namespace KiLib
