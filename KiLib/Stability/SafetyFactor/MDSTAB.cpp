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


#include <KiLib/Exceptions/NotImplemented.hpp>
#include <KiLib/Stability/SafetyFactor/MDSTAB.hpp>
#include <KiLib/Stability/SafetyFactor/SafetyFactor.hpp>
#include <algorithm>
#include <cmath>

using namespace KiLib::Stability::SafetyFactor;

MDSTAB::MDSTAB(){};

double MDSTAB::ComputeSF(
   double phi, double m, double z, double Crl, double Crb, double theta, double delta, double gamma_s, double w,
   double l, double gamma_w, bool calcFrd) const
{
   const double stheta = std::sin(theta);
   const double ctheta = std::cos(theta);

   const double sphi = std::sin(phi);
   const double cphi = std::cos(phi);
   const double tphi = std::tan(phi);

   // Central block driving force (Milledge Equation 3)
   const double Fdc = gamma_s * z * stheta * ctheta * w * l;

   // At-rest earth pressure coefficient (Milledge Equation 5)
   const double K0 = 1.0 - sphi;

   // Block cross-slope boundary force (Milledge Equation 7)
   const double Frl =
      0.5 * K0 * (gamma_s - gamma_w * pow(m, 2.0)) * l * pow(z, 2.0) * ctheta * tphi + Crl * l * z * ctheta;

   // Rankine solution for cohesive soils
   const double K = std::max(
      0.0, 4.0 * pow(ctheta, 2.0) * (pow(ctheta, 2.0) - pow(cphi, 2.0)) +
              (4.0 * pow(Crl / (gamma_s * z), 2.0) * pow(cphi, 2.0)) +
              (8.0 * (Crl / (gamma_s * z)) * pow(ctheta, 2.0) * sphi * cphi));

   // Passive (p) and active (a) earth pressure coefficients for Rankine solution for cohesive soils (Milledge Equation
   // 8)
   const double Kp =
      (1.0 / pow(cphi, 2.0)) * (2.0 * pow(ctheta, 2.0) + 2.0 * (Crl / (gamma_s * z)) * cphi * sphi + sqrt(K)) - 1.0;
   const double Ka =
      (1.0 / pow(cphi, 2.0)) * (2.0 * pow(ctheta, 2.0) + 2.0 * (Crl / (gamma_s * z)) * cphi * sphi - sqrt(K)) - 1.0;

   // Slope-parallel passive force on the upslope margin (Milledge Equation 12 modified)
   const double Fdu = 0.5 * Ka * pow(z, 2.0) * (gamma_s - gamma_w * pow(m, 2.0)) * w * cos(delta - theta);

   // Slope-normal passive force on the upslope margin (Milledge Equation 13 modified)
   const double Fnu = 0.5 * Ka * pow(z, 2.0) * (gamma_s - gamma_w * pow(m, 2.0)) * w * sin(delta - theta);

   // Slope-parallel passive force on the downslope margin (Milledge Equation 12)
   double Frd = 0.0;
   if (calcFrd)
   {
      Frd = 0.5 * Kp * pow(z, 2.0) * (gamma_s - gamma_w * pow(m, 2.0)) * w * cos(delta - theta);
   }

   // Slope-normal passive force on the downslope margin (Milledge Equation 13)
   const double Fnd = 0.5 * Kp * pow(z, 2.0) * (gamma_s - gamma_w * pow(m, 2.0)) * w * sin(delta - theta);

   // Weight of block (see Milledge Equation 14)
   const double Fnc = (gamma_s - gamma_w * m) * z * pow(ctheta, 2.0) * w * l;

   // Normal force on basal surface (Milledge Equation 14)
   const double Fnt = Fnc + Fnu - Fnd;

   // Basal resistance force (Milledge Equation 15)
   const double Frb = Crb * w * l + Fnt * tphi;

   // Factor of Safety (Milledge Equation 16)
   const double FS = (Frb + 2.0 * Frl + Frd - Fdu) / Fdc;

   return FS;
}
