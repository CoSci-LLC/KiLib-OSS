#pragma once

#include <algorithm>
#include <functional>
#include <iostream>
#include <random>
#include <vector>

namespace KiLib
{
   class NewtonRaphson
   {
   public:
      static double NewtonRaphsonAnalytic(
         const std::function<double(double)> &f, const std::function<double(double)> &df, double x_o, double tol,
         int maxIter);
   };

} // namespace KiLib
