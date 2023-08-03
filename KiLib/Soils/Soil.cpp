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
#include <KiLib/Exceptions/NotImplemented.hpp>

using namespace KiLib::Soils;

std::string Soil::GetName() const
{
   return this->name;
}
std::string Soil::GetLongName() const
{
   return this->longname;
}

double Soil::GetDensityDry() const
{
   throw NotImplementedException(
      "GetDensityDry not implemented in soil base class. Use the DensityDryGen Decorator class");
}

double Soil::GetPorosity() const
{
   return this->porosity.value();
}
double Soil::GetSaturatedWaterContent() const
{
   return this->saturatedWaterContent.value();
}
double Soil::GetResidualWaterContent() const
{
   return this->residualWaterContent.value();
}
double Soil::GetDensityWet() const
{
   return this->densityWet.value();
}
double Soil::GetFieldCapacity() const
{
   return this->fieldCapacity.value();
}
double Soil::GetInitWaterContent() const
{
   return this->initWaterContent.value();
}
double Soil::GetWaterExchangeTerm() const
{
   return this->waterExchangeTerm.value();
}
double Soil::GetVGWetAlpha1() const
{
   return this->vgWetAlpha1.value();
}
double Soil::GetVGWetN1() const
{
   return this->vgWetN1.value();
}
double Soil::GetPoreFracMatrix() const
{
   return this->fieldCapacity.value() / this->porosity.value();
}
double Soil::GetPoreFracFractures() const
{
   return 1.0 - GetPoreFracMatrix();
}
double Soil::GetMaxTensileStrain() const
{
   return this->maxTensileStrain.value();
}

ValueDistribution Soil::GetFrictionAngleDistribution() const
{
   return this->frictionAngle;
}

ValueDistribution Soil::GetDensityDryDistribution() const
{
   return this->densityDry;
}

ValueDistribution Soil::GetCohesionDistribution() const
{
   return this->cohesion;
}

ValueDistribution Soil::GetConductivityDistributon() const
{
   return this->conductivity;
}


double ValueUniform::GetMin() const
{
   return this->min.value();
}
double ValueUniform::GetMax() const
{
   return this->max.value();
}

// Parameters for a normal distribution of values
double ValueNormal::GetMean() const
{
   return this->mean.value();
}
double ValueNormal::GetStdDev() const
{
   return this->stdDev.value();
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
   return this->constant.value();
}
