/**
 * Provides a way to create a getter/setter in a more efficent manner
 * Makes sure that the double is a safe value before returning it.
 */
#pragma once
#include <KiLib/Exceptions/InvalidArgument.hpp>
#include <cmath>

class SafeDouble
{
public:
   SafeDouble(double value)
   {
      this(value);
   }

   double operator()() const
   {
      return value;
   }
   void operator()(double value)
   {
      if (isnan(value)) {
         throw InvalidArgumentException("Value cannot be NAN");
      }

      this->value = value;
   }

private:
   double value = NAN;
};