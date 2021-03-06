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