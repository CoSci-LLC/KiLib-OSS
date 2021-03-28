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


#include <KiLib/Soils/Soils.hpp>

using namespace KiLib::Soils;

std::string Soil::GetName() const
{
   return this->name;
}
std::string Soil::GetLongName() const
{
   return this->longname;
}

double Soil::GetPorosity() const
{
   return this->porosity;
}
double Soil::GetSaturatedWaterContent() const
{
   return this->saturatedWaterContent;
}
double Soil::GetResidualWaterContent() const
{
   return this->residualWaterContent;
}
double Soil::GetFieldCapacity() const
{
   return this->fieldCapacity;
}
double Soil::GetInitWaterContent() const
{
   return this->initWaterContent;
}
double Soil::GetWaterExchangeTerm() const
{
   return this->waterExchangeTerm;
}
double Soil::GetVgWetAlpha1() const
{
   return this->vgWetAlpha1;
}
double Soil::GetVgWetN1() const
{
   return this->vgWetN1;
}
double Soil::GetPoreFracMatrix() const
{
   return this->poreFracMatrix;
}
double Soil::GetPoreFracFractures() const
{
   return this->poreFracFractures;
}
double Soil::GetMaxTensileStrain() const
{
   return this->maxTensileStrain;
}

ValueDistribution Soil::GetFrictionAngle() const
{
   return this->frictionAngle;
}

ValueDistribution Soil::GetDensityDry() const
{
   return this->densityDry;
}

ValueDistribution Soil::GetCohesion() const
{
   return this->cohesion;
}

ValueDistribution Soil::GetConductivity() const
{
   return this->conductivity;
}


double ValueUniform::GetMin() const
{
   return this->min;
}
double ValueUniform::GetMax() const
{
   return this->max;
}

// Parameters for a normal distribution of values
double ValueNormal::GetMean() const
{
   return this->mean;
}
double ValueNormal::GetStdDev() const
{
   return this->stdDev;
}

ValueUniform ValueDistribution::GetUniformPrimula() const
{
   return this->uniformPrimula;
}
ValueUniform ValueDistribution::GetUniform() const
{
   return this->uniform;
}
ValueNormal ValueDistribution::GetNormal() const
{
   return this->normal;
}
double ValueDistribution::GetConstant() const
{
   return this->constant;
}