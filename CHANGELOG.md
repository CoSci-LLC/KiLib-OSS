# Changelog

Notable changes to this project will be documented in this file.

## v3.1.0
---

### Added

- New Soils

### Fixed

- Random point generation in RAster

## v3.0.0
---

### Fixed

- MacOS compilation will work now
- Compiles with Clang

### Changed

- Breaking changes with soils class
- Raster has been updated
- Random is now a namespace


## v2.5.1
---
### Added

- Add flattenIndex method to Raster
- Add Rasterize method to raster
- Add GetAverage method to raster
- Refactored soil class, construction now with factory and enum
- Add getValid indices and assertAgreeDim to Raster class
- Split raster class up a bit

### Fixed

- When compiling on windows we define _USE_MATH_DEFINES to make sure there are no math constant issues
- getCellPos div by zero
- rtnorml insertion past bound of vector

## v2.5.0 (2020-03-03)
---

### Added

- Add Cell Position call to Raster
- Add truncated normal distribution

### Changed

- Libraries are built inside build directory
- CMake install commands make more sense now
- Change MDSTAB Safety Factor to include passive force

