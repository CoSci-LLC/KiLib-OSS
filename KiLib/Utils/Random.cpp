#include <KiLib/Utils/Random.hpp>


using namespace KiLib;


std::vector<double> Random::runif(int count, double min, double max)
{
   std::vector<double> result(count);

   std::random_device                     rd;
   std::mt19937                           gen{rd()};
   std::uniform_real_distribution<double> distribution(min, max);

   std::generate(result.begin(), result.end(), [&gen, &distribution]() -> double { return distribution(gen); });

   return result;
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
   std::random_device rd;
   std::mt19937       gen{rd()};

   std::vector<double>              results(count);
   std::normal_distribution<double> dist(mean, sd);
   for (int i = 0; i < count; i++)
   {
      results[i] = dist(gen);
   }

   return results;
}

std::vector<double> Random::pgamma(std::vector<double> x, double shape)
{
   for (size_t i = 0; i < x.size(); i++)
   {
      x[i] = KiLib::gammaPDF(x[i], 1, shape);
   }
   return x;
}
