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
   spdlog::debug("Iter        x_n    x_{n+1}           dx");
   do {
      iter++;
      x1        = x2;
      auto dfx1 = df(x1);
      if (dfx1 == 0.0) {
         spdlog::error("Newton Raphson Analytic: derivative of function is zero");
         exit(EXIT_FAILURE);
      }
      x2 = x1 - f(x1) / dfx1;
      dx = std::abs(x2 - x1);
      spdlog::debug("{:4d} {:10.6f} {:10.6f} {:12.6e}", iter, x1, x2, dx);
   } while ((iter < maxIter) && (dx > tol));
   if (iter >= maxIter) {
      spdlog::error("Newton Raphson Analytic: number of iterations exceeded");
      exit(EXIT_FAILURE);
   }
   return x2;
}
