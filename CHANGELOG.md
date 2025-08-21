# Changelog

Notable changes to this project will be documented in this file.

## v6.3.0
---

### Added

- Infiltration models
- Soil and constant updates

### Fixed

- Updated tests

## v6.1.0
---

### Added

- New raster algorithms specific for streams

### Fixed

- Warnings have been fixed

## v6.0.0
---

### Added

- Using CMakePresets to make builds uniform among developers
- Container2D

### Fixed

- Checks added to ensure GetAverage of a raster is not divided by 0
- Builds on macOS, Windows, and Linux

### Removed

- Tests have been removed to simplify build process within the CI. 
  We are going to reimplement the tests and ensure they provide 
  accurate coverage and test features properly. This was missing
  in the previous tests and why they were removed.


## v5.1.0
---

### Added

- Added soil decorators for additional calculations
- Constants for certain physical properties
- New version of spdlog

### Fixed

- Fixed tests for rasters

## v5.0.0
---

### Added

- Elevation condition to Raster::GetCoordMinDistance

### Fixed

- Fixed raster tests


## v4.0.0
---

### Added

- Elevation Condition to Raster::GetCoordMinDistance

### Fixed

- Unit Tests (Thanks @haydn-jones!)

## v3.3.0
---

### Added

- Add function Raster::GetCoordMinDistance with test and new dem for checking
- Add function GetRowCol from index and use it elsewhere
- Add getCellCenter and NormXY

### Fixed

- Fix z value of return coord for GetCoordMinDistance
- Fix error in getNearestCell: use floor instead of round

## v3.2.0
---

### Added

- New distrubtions

### Fixed

- Added Random to KiLib.hpp

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

