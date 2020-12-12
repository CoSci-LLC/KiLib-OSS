#pragma once

#include <cmath>

// Required for math constants on Windows
#ifdef _WINDOWS
#include <corecrt_math_defines.h>
#endif

namespace KiLib
{

   // https://en.wikipedia.org/wiki/Normal_distribution#Cumulative_distribution_function

   /**
    * @brief Cumulative distribution function for normal distribution
    *
    * @param x
    * @param mean
    * @param stdev
    * @return double
    */
   inline double normalCDF(double x, double mean, double stdev)
   {
      return 0.5 * (1.0 + std::erf((x - mean) / (stdev * M_SQRT2)));
   }


   /**
    * @brief Cumulative distribution function for weibull distribution
    *
    * @param x
    * @param shape
    * @param scale
    * @return double
    */
   inline double weibullCDF(double x, double shape, double scale)
   {
      return 1.0 - std::exp(-std::pow(x / scale, shape));
   }

   /**
    * @brief Probability density function for weibull distribution
    *
    * @param x
    * @param shape
    * @param scale
    * @return double
    */
   inline double weibullPDF(double x, double shape, double scale)
   {
      return (shape / scale) * std::pow(x / scale, shape - 1.0) * std::exp(-std::pow(x / scale, shape));
   }

   /**
    * @brief Weibull survival value
    *
    * @param x
    * @param shape
    * @param scale
    * @return double
    */
   inline double weibullSurvival(double x, double shape, double scale)
   {
      return std::exp(-std::pow(x / scale, shape));
   }

   /**
    * @brief Mode for weibull distribution
    *
    * @param shape
    * @param scale
    * @return double
    */
   inline double weibullMode(double shape, double scale)
   {
      if (shape <= 1.0)
         return 0.0;
      else
         return scale * std::pow((shape - 1.0) / shape, 1.0 / shape);
   }

   /**
    * @brief Probability density function for gamma distribution
    *
    * @param x
    * @param scale
    * @param shape
    * @return double
    */
   inline double gammaPDF(double x, double scale, double shape)
   {
      return (std::pow(x, shape - 1.0) * std::exp(-1.0 * x / scale)) / (std::tgamma(shape) * std::pow(scale, shape));
   }


} // namespace KiLib