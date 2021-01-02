/**
 * @file StatisticalOutput.hpp
 *
 * @brief Provides a on the fly calculation of mean and std. dev.
 * Using the method found here:
 * https://stackoverflow.com/questions/10365119/mean-value-and-standard-deviation-of-a-very-huge-data-set
 *
 * @warning This class is not thread safe.
 *
 * @author Owen Parkins
 * Contact: oparkins@nibious.com
 *
 */

#pragma once

#include <cmath>
#include <vector>

namespace KiLib
{
   namespace Utils
   {
      class StatisticalOutput
      {
      public:
         double GetMean() const;
         double GetStdDev() const;

         void AddData(double);

      private:
         int count                  = 0;
         double rollingSum          = 0.0;
         double rollingSumOfSquares = 0.0;
      };
   } // namespace Utils

} // namespace KiLib