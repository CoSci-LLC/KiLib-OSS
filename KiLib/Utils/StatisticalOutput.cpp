/**
 * @file StatisticalOutput.cpp
 *
 * @brief Provides a on the fly calculation of mean and std. dev.
 * Using the method found here:
 * https://stackoverflow.com/questions/10365119/mean-value-and-standard-deviation-of-a-very-huge-data-set
 *
 * @author Owen Parkins
 * Contact: oparkins@nibious.com
 *
 */

#include <KiLib/Utils/StatisticalOutput.hpp>

using namespace KiLib::Utils;

double StatisticalOutput::GetMean() const
{
   return this->rollingSum / count;
}

double StatisticalOutput::GetStdDev() const
{
   return std::sqrt(this->rollingSumOfSquares / (double)count - std::pow(GetMean(), 2));
}

void StatisticalOutput::AddData(double value)
{
   this->rollingSum += value;
   this->rollingSumOfSquares += std::pow(value, 2);
   this->count++;
}