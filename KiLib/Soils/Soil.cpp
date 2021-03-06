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
const AbsFac<SoilID, Soil> Soil::factory = {{
   {SoilID::ch, []() -> Soil { return Soils::ch(); }},
   {SoilID::cl, []() -> Soil { return Soils::cl(); }},
   {SoilID::gc, []() -> Soil { return Soils::gc(); }},
   {SoilID::gm, []() -> Soil { return Soils::gm(); }},
   {SoilID::gp, []() -> Soil { return Soils::gp(); }},
   {SoilID::gw, []() -> Soil { return Soils::gw(); }},
   {SoilID::mh, []() -> Soil { return Soils::mh(); }},
   {SoilID::ml, []() -> Soil { return Soils::ml(); }},
   {SoilID::oh, []() -> Soil { return Soils::oh(); }},
   {SoilID::ol, []() -> Soil { return Soils::ol(); }},
   {SoilID::sc, []() -> Soil { return Soils::sc(); }},
   {SoilID::sm, []() -> Soil { return Soils::sm(); }},
   {SoilID::sp, []() -> Soil { return Soils::sp(); }},
   {SoilID::sw, []() -> Soil { return Soils::sw(); }},
}};