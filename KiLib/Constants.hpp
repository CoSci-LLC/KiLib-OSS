#pragma once

namespace KiLib
{
   namespace Constants
   {
      inline constexpr double GRAVITY{9.81};                        // [m/s^2] Gravitational acceleration
      inline constexpr double WATER_DENSITY{999.9};                 // [m^3/kg] at 0 C
      inline constexpr double WATER_COMPRESSIBILITY_0C{5.09e-10};   // [1/Pa] at 0 C
      inline constexpr double WATER_COMPRESSIBILITY_5C{4.94e-10};   // [1/Pa] at 0 C
      inline constexpr double WATER_COMPRESSIBILITY_10C{4.78e-10};  // [1/Pa] at 10 C
      inline constexpr double WATER_COMPRESSIBILITY_15C{4.69e-10};  // [1/Pa] at 15 C
      inline constexpr double WATER_COMPRESSIBILITY_20C{4.59e-10};  // [1/Pa] at 20 C
      inline constexpr double GRAIN_DENSITY{2650};                  // [m^3/kg]
      inline constexpr double QUARTZ_DENSITY{2650};                 // [m^3/kg]
   } // namespace Constants
} // namespace KiLib
