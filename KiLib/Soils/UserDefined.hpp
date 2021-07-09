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


#pragma once

#include <KiLib/Soils/Soil.hpp>
#include <KiLib/Utils/SafeDouble.hpp>


namespace KiLib::Soils
{
   struct UserDefinedValueUniform : public ValueUniform
   {
      void SetMin(double value);
      void SetMax(double value);
   };

   struct UserDefinedValueNormal : public ValueNormal
   {
      void SetMean(double value);
      void SetStdDev(double value);
   };

   struct UserDefinedValueDistribution : public ValueDistribution
   {
      void SetUniformPrimula(ValueUniform);
      void SetUniform(ValueUniform);
      void SetNormal(ValueNormal);
      void SetConstant(double);
   };


   class UserDefined : public Soil
   {
   public:
      UserDefined()
      {
         this->name     = "ud";
         this->longname = "User Defined Soil";
      };

      void SetName(std::string name);
      void SetLongName(std::string longname);
      void SetPorosity(double value);
      void SetSaturatedWaterContent(double value);
      void SetResidualWaterContent(double value);
      void SetFieldCapacity(double value);
      void SetInitWaterContent(double value);
      void SetWaterExchangeTerm(double value);
      void SetVgWetAlpha1(double value);
      void SetVgWetN1(double value);
      void SetMaxTensileStrain(double value);

      void SetFrictionAngle(ValueDistribution value);
      void SetDensityDry(ValueDistribution value);
      void SetCohesion(ValueDistribution value);
      void SetConductivity(ValueDistribution value);
   };
} // namespace KiLib::Soils
