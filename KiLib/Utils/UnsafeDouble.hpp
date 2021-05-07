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
 * Provides a way to create a getter/setter in a more efficent manner
 * Makes sure that the double is a safe value before returning it.
 */
#pragma once
#define _USE_MATH_DEFINES
#include <KiLib/Exceptions/InvalidArgument.hpp>
#include <cmath>

class UnsafeDouble
{
public:
   UnsafeDouble() : value(NAN)
   {
   }
   UnsafeDouble(double value) : value(value)
   {
   }

   double operator()() const
   {
      return this->value;
   }
   void operator()(double value)
   {
      this->value = value;
   }

private:
   double value = NAN;
};