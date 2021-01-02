#include <KiLib/Exceptions/NotImplemented.hpp>
#include <KiLib/Stability/SafetyFactor/MDSTAB.hpp>
#include <KiLib/Stability/SafetyFactor/SafetyFactor.hpp>
#include <cmath>

using namespace KiLib::Stability::SafetyFactor;

MDSTab::MDSTab(){};

double MDSTab::ComputeSF(
   double phi, double m, double z, double Crl, double Crb, double theta, double delta, double gamma_s, double w,
   double l) const
{
   // Central block driving force (Milledge Equation 3)
   double Fdc = gamma_s * z * sin(theta) * cos(theta) * w * l;

   // At-rest earth pressure coefficient (Milledge Equation 5)
   double K0 = 1.0 - sin(phi);

   // Block cross-slope boundary force (Milledge Equation 7)
   double Frl =
      0.5 * K0 * (gamma_s - gamma_w * pow(m, 2)) * l * pow(z, 2) * cos(theta) * tan(phi) + Crl * l * z * cos(theta);

   // Rankine solution for cohesive soils
   double K = std::max(
      0.0, 4.0 * pow(cos(theta), 2) * (pow(cos(theta), 2) - pow(cos(phi), 2)) +
              (4 * pow(Crl / (gamma_s * z), 2) * pow(cos(phi), 2)) +
              (8 * (Crl / (gamma_s * z)) * pow(cos(theta), 2) * sin(phi) * cos(phi)));

   // Passive (p) and active (a) earth pressure coefficients for Rankine solution for cohesive soils (Milledge Equation
   // 8)
   double Kp =
      (1 / pow(cos(phi), 2)) * (2 * pow(cos(theta), 2) + 2 * (Crl / (gamma_s * z)) * cos(phi) * sin(phi) + sqrt(K)) - 1;
   double Ka =
      (1 / pow(cos(phi), 2)) * (2 * pow(cos(theta), 2) + 2 * (Crl / (gamma_s * z)) * cos(phi) * sin(phi) - sqrt(K)) - 1;

   // Slope-parallel passive force on the upslope margin (Milledge Equation 12 modified)
   double Fdu = 0.5 * Ka * pow(z, 2) * (gamma_s - gamma_w * pow(m, 2)) * w * cos(delta - theta);

   // Slope-normal passive force on the upslope margin (Milledge Equation 13 modified)
   double Fnu = 0.5 * Ka * pow(z, 2) * (gamma_s - gamma_w * pow(m, 2)) * w * sin(delta - theta);

   // Slope-parallel passive force on the downslope margin (Milledge Equation 12)
   // double Frd = 0.5 * Kp * pow(z, 2) * (gamma_s - gamma_w * pow(m, 2)) * w * cos(delta - theta);

   // THIS IS IN THE MATLAB CODE. MAY NEED AN OPTION HERE TO NOT SET IT TO ZERO AND USE EQUATION ABOVE. TBD
   double Frd = 0.0;

   // Slope-normal passive force on the downslope margin (Milledge Equation 13)
   double Fnd = 0.5 * Kp * pow(z, 2) * (gamma_s - gamma_w * pow(m, 2)) * w * sin(delta - theta);

   // Weight of block (see Milledge Equation 14)
   double Fnc = (gamma_s - gamma_w * m) * z * pow(cos(theta), 2) * w * l;

   // Normal force on basal surface (Milledge Equation 14)
   double Fnt = Fnc + Fnu - Fnd;

   // Basal resistance force (Milledge Equation 15)
   double Frb = Crb * w * l + Fnt * tan(phi);

   // Factor of Safety (Milledge Equation 16)
   double FS = (Frb + 2 * Frl + Frd - Fdu) / Fdc;

   return FS;
}