#pragma once

#include <KiLib/Soils/Soil.hpp>
#include <KiLib/Soils/UserDefined.hpp>
#include <KiLib/Soils/ch.hpp>
#include <KiLib/Soils/cl.hpp>
#include <KiLib/Soils/gc.hpp>
#include <KiLib/Soils/gm.hpp>
#include <KiLib/Soils/gmgl.hpp>
#include <KiLib/Soils/gp.hpp>
#include <KiLib/Soils/gw.hpp>
#include <KiLib/Soils/mh.hpp>
#include <KiLib/Soils/ml.hpp>
#include <KiLib/Soils/oh.hpp>
#include <KiLib/Soils/ol.hpp>
#include <KiLib/Soils/ruedlingen.hpp>
#include <KiLib/Soils/sc.hpp>
#include <KiLib/Soils/sm.hpp>
#include <KiLib/Soils/sp.hpp>
#include <KiLib/Soils/sw.hpp>
#include <KiLib/Utils/AbstractFactory.hpp>
#include <KiLib/Utils/enum.hpp>

namespace KiLib::Soils
{
   BETTER_ENUM(SoilID, int, ch, cl, gc, gm, gmgl, gp, gw, mh, ml, oh, ol, ruedlingen, sc, sm, sp, sw)
   const std::vector<std::string> availableSoils(SoilID::_names().begin(), SoilID::_names().end());

   const AbsFac<SoilID, Soil> factory = {{
      {SoilID::ch, []() -> Soil { return Soils::ch(); }},
      {SoilID::cl, []() -> Soil { return Soils::cl(); }},
      {SoilID::gc, []() -> Soil { return Soils::gc(); }},
      {SoilID::gm, []() -> Soil { return Soils::gm(); }},
      {SoilID::gmgl, []() -> Soil { return Soils::gmgl(); }},
      {SoilID::gp, []() -> Soil { return Soils::gp(); }},
      {SoilID::gw, []() -> Soil { return Soils::gw(); }},
      {SoilID::mh, []() -> Soil { return Soils::mh(); }},
      {SoilID::ml, []() -> Soil { return Soils::ml(); }},
      {SoilID::oh, []() -> Soil { return Soils::oh(); }},
      {SoilID::ol, []() -> Soil { return Soils::ol(); }},
      {SoilID::ruedlingen, []() -> Soil { return Soils::ruedlingen(); }},
      {SoilID::sc, []() -> Soil { return Soils::sc(); }},
      {SoilID::sm, []() -> Soil { return Soils::sm(); }},
      {SoilID::sp, []() -> Soil { return Soils::sp(); }},
      {SoilID::sw, []() -> Soil { return Soils::sw(); }},
   }};
} // namespace KiLib::Soils
