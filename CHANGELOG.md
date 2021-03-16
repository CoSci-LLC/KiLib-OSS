# Changelog

Notable changes to this project will be documented in this file.

## vX.X.X
---
### Added

- Add flattenIndex method to Raster

### Fixed

- When compiling on windows we define _USE_MATH_DEFINES to make sure there are no math constant issues
- getCellPos div by zero

## v2.5.0 (2020-03-03)
---

### Added

- Add Cell Position call to Raster
- Add truncated normal distribution

### Changed

- Libraries are built inside build directory
- CMake install commands make more sense now
- Change MDSTAB Safety Factor to include passive force

