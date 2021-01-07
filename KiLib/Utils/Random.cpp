#include <KiLib/Utils/Random.hpp>
#include <stats.hpp>

using namespace KiLib;


std::vector<double> Random::runif(int count, double min, double max)
{
   return stats::runif<std::vector<double>>(count, 1, min, max);
}

std::vector<double> Random::rnorm(int count, std::vector<double> means, double sd)
{
   std::random_device rd;
   std::mt19937       gen{rd()};

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

std::vector<double> Random::rnorm(int count, double mean, double sd)
{
   return stats::rnorm<std::vector<double>>(count, 1, mean, sd);
}

std::vector<double> Random::pgamma(std::vector<double> x, double shape)
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