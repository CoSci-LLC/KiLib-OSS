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

#define _USE_MATH_DEFINES
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
         int    count               = 0;
         double rollingSum          = 0.0;
         double rollingSumOfSquares = 0.0;
      };
   } // namespace Utils

} // namespace KiLib