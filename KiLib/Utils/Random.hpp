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
      static std::vector<double> runif(int, double, double);
      static std::vector<double> rnorm(int, std::vector<double>, double);
      static std::vector<double> rnorm(int, double, double);
      static std::vector<double> pgamma(std::vector<double>, double);
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

      for (unsigned int i = 0; i < data.size(); i++) {
         summation += (data[i] - mean) * (data[i] - mean);
      }
      return std::sqrt(summation / (data.size() - 1));
   };
} // namespace KiLib