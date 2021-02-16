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

#include <KiLib/Utils/Distributions.hpp>
#include <algorithm>
#include <iostream>
#include <numeric>
#include <random>
#include <vector>

namespace KiLib
{
   class Random
   {
   public:
      static std::vector<double>          runif(int, double, double, std::mt19937_64 &);
      static std::vector<double>          rnorm(int, std::vector<double>, double, std::mt19937_64 &);
      static std::vector<double>          rnorm(int, double, double, std::mt19937_64 &);
      static std::vector<double>          rtnorml(int, double, double, double, std::mt19937_64 &);
      static std::vector<double>          pgamma(std::vector<double>, double);
      template <typename T> static double mean(std::vector<T>);
      template <typename T> static double sd(std::vector<T>);

      static double qtri(const double p, const double a, const double b, const double c);
   };

   template <typename T> double Random::mean(std::vector<T> data)
   {
      return std::accumulate(data.begin(), data.end(), 0.0) / data.size();
   };

   template <typename T> double Random::sd(std::vector<T> data)
   {
      double mean      = Random::mean(data);
      double summation = 0.0;

      for (unsigned int i = 0; i < data.size(); i++)
      {
         summation += (data[i] - mean) * (data[i] - mean);
      }
      return std::sqrt(summation / (data.size() - 1));
   };
} // namespace KiLib