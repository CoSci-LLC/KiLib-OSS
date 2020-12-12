/**
 * Provides a way to create a getter/setter in a more efficent manner
 * Makes sure that the double is a safe value before returning it.
 */
#pragma once
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