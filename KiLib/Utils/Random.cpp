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


#include <KiLib/Utils/Random.hpp>
#include <stats.hpp>

using namespace KiLib;

std::vector<double> Random::runif(int count, double min, double max, std::mt19937_64 &gen)
{
   std::vector<double> out(count);
   std::generate(out.begin(), out.end(), [&]() -> double { return stats::runif(min, max, gen); });

   return out;
}

std::vector<double> Random::rnorm(int count, std::vector<double> means, double sd, std::mt19937_64 &gen)
{
   unsigned int        meansCount = 0;
   std::vector<double> results(count);

   for (int i = 0; i < count; i++)
   {
      std::normal_distribution<double> dist(means[meansCount], sd);
      meansCount = (meansCount + 1) % means.size();
      results[i] = dist(gen);
   }

   return results;
}

std::vector<double> Random::rnorm(int count, double mean, double sd, std::mt19937_64 &gen)
{
   std::vector<double> out(count);
   std::generate(out.begin(), out.end(), [&]() -> double { return stats::rnorm(mean, sd, gen); });
   return out;
}

// Algorithm 1 from http://web.michaelchughes.com/research/sampling-from-truncated-normal
// Assumes b = inf
std::vector<double> Random::rtnorml(int count, double mean, double sd, double a, std::mt19937_64 &gen)
{
   std::vector<double> out;
   for (int i = 0; i < count; i++)
   {
      double x;
      while (true)
      {
         x = stats::rnorm(mean, sd, gen);
         if (x > a)
         {
            break;
         }
      }
      out.push_back(x);
   }

   return out;
}

std::vector<double> Random::pgamma(const std::vector<double> &x, double shape)
{
   return stats::pgamma(x, shape, 1);
}

/**
 * @brief Returns the quantile of the given value in a triangular distribution
 *
 * @param p The probability
 * @param a The lower bound
 * @param b The upper bound
 * @param c The mode
 * @return double The quantile
 */
double Random::qtri(const double p, const double a, const double b, const double c)
{
   if (p < c)
      return a + std::sqrt((b - a) * (c - a) * p);
   else if (p > c)
      return b - std::sqrt((b - a) * (b - c) * (1 - p));

   return c;
}