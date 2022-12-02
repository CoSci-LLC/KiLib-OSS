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


#include <KiLib/Soils/UserDefined.hpp>

using namespace KiLib::Soils;

void UserDefined::SetName(std::string name)
{
   this->name = name;
}
void UserDefined::SetLongName(std::string longname)
{
   this->longname = longname;
}

void UserDefined::SetPorosity(double value)
{
   this->porosity = value;
}
void UserDefined::SetSaturatedWaterContent(double value)
{
   this->saturatedWaterContent = value;
}
void UserDefined::SetResidualWaterContent(double value)
{
   this->residualWaterContent = value;
}
void UserDefined::SetFieldCapacity(double value)
{
   this->fieldCapacity = value;
}
void UserDefined::SetInitWaterContent(double value)
{
   this->initWaterContent = value;
}
void UserDefined::SetWaterExchangeTerm(double value)
{
   this->waterExchangeTerm = value;
}
void UserDefined::SetVgWetAlpha1(double value)
{
   this->vgWetAlpha1 = value;
}
void UserDefined::SetVgWetN1(double value)
{
   this->vgWetN1 = value;
}

void UserDefined::SetMaxTensileStrain(double value)
{
   this->maxTensileStrain = value;
}

void UserDefined::SetFrictionAngleDistribution(ValueDistribution value)
{
   this->frictionAngle = value;
}

void UserDefined::SetDensityDryDistribution(ValueDistribution value)
{
   this->densityDry = value;
}

void UserDefined::SetCohesionDistribution(ValueDistribution value)
{
   this->cohesion = value;
}

void UserDefined::SetConductivityDistribution(ValueDistribution value)
{
   this->conductivity = value;
}


void UserDefinedValueUniform::SetMin(double value)
{
   this->min = value;
}
void UserDefinedValueUniform::SetMax(double value)
{
   this->max = value;
}

// Parameters for a normal distribution of values
void UserDefinedValueNormal::SetMean(double value)
{
   this->mean = value;
}

void UserDefinedValueNormal::SetStdDev(double value)
{
   this->stdDev = value;
}

void UserDefinedValueDistribution::SetUniformPrimula(ValueUniform value)
{
   this->uniformPrimula = value;
}

void UserDefinedValueDistribution::SetUniform(ValueUniform value)
{
   this->uniform = value;
}

void UserDefinedValueDistribution::SetNormal(ValueNormal value)
{
   this->normal = value;
}

void UserDefinedValueDistribution::SetConstant(double value)
{
   this->constant = value;
}
