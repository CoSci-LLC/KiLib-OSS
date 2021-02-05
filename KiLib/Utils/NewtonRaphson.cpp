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


#include <KiLib/Utils/NewtonRaphson.hpp>
#include <spdlog/spdlog.h>


using namespace KiLib;


double NewtonRaphson::NewtonRaphsonAnalytic(
   const std::function<double(double)> &f, const std::function<double(double)> &df, double x_o, double tol, int maxIter)
{
   auto iter = 0;
   auto dx   = std::numeric_limits<double>::max();
   auto x1   = x_o;
   auto x2   = x_o;
   SPDLOG_DEBUG("Iter        x_n    x_{n+1}           dx");
   do
   {
      iter++;
      x1        = x2;
      auto dfx1 = df(x1);
      if (dfx1 == 0.0)
      {
         spdlog::error("Newton Raphson Analytic: derivative of function is zero");
         exit(EXIT_FAILURE);
      }
      x2 = x1 - f(x1) / dfx1;
      dx = std::abs(x2 - x1);
      SPDLOG_DEBUG("{:4d} {:10.6f} {:10.6f} {:12.6e}", iter, x1, x2, dx);
   } while ((iter < maxIter) && (dx > tol));
   if (iter >= maxIter)
   {
      spdlog::error("Newton Raphson Analytic: number of iterations exceeded");
      exit(EXIT_FAILURE);
   }
   return x2;
}
