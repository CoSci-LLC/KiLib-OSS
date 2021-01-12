# KiLib-OSS 
#### Version v2.4.1

*A Scientific Library for "Earth" (Ki) surface processes*

## Dependencies
All dependencies required for this library will be download via the CMake build process

## Building and Installation

### Linux

```bash
$ mkdir build
$ cd build 
$ cmake .. -DCMAKE_INSTALL_PREFIX=<install location>
$ make
$ make install
```

### Windows
Visual Studio 2019 is the preferred method for building KiLib on Windows. CMake is also required.

Clone or download the KiLib Library and open a VS 2019 Developer Console in that directory. Then:

```bash
$ mkdir build
$ cd build
$ cmake ..
$ cmake --build .
```

## Usage
To automatically download KiLib, add the following to your CMakeLists.txt:
```
### KiLib Install Process
include(FetchContent)

FetchContent_Populate(
	kilib_proj
	QUIET
	GIT_REPOSITORY https://github.com/CoSci-LLC/KiLib-OSS.git
  GIT_TAG v2.4.1
	SOURCE_DIR     kilib_proj              # (Relative) path within in the build directory.
)

# ------------------------------------------------------------------------------------
# And now you can already add and use it, like it's a part/target of your own project!

add_subdirectory(${kilib_proj_SOURCE_DIR} kilib_proj/build)
include_directories(${kilib_proj_SOURCE_DIR} ${CMAKE_INSTALL_PREFIX}/include)

# To be able to use stats lib
include_directories(${stats_proj_SOURCE_DIR}/include)
```

Then to use in your project, use:
```
target_link_libraries(${projectName} PUBLIC KiLib)
```

## Debug Builds

KiLib has the option to be built with debugging features. This will enable debug prints (as long as `spdlog` active level is set to debug as well) and it will add stack traces and other useful debugging features. To build KiLib in debug mode, add the `-DCMAKE_BUILD_TYPE=Debug` flag to your CMake command like:

```bash
...
$ cmake .. -DCMAKE_INSTALL_PREFIX=<install location> -DCMAKE_BUILD_TYPE=Debug
...
```

Building in Debug mode will also build the tests for KiLib.

## Running Tests

To run KiLib tests, follow these commands:

```bash
$ mkdir build
$ cd build 
$ cmake .. -DCMAKE_INSTALL_PREFIX=<install location> -DCMAKE_BUILD_TYPE=Debug
$ make
$ make install
$ ctest
```

The `make install` or equivalent is important because KiLib tests require the soils to be installed in the proper location for a few of the tests to pass.

## Classes

### Root Models
(`kilib/RootModel.hpp`) Includes `RootModel` base (abstract) class which is inherited by a couple other model implementations. These models are used to compute forces produced by tree roots.

### Soil Types
(`kilib/SoilType.hpp`) Includes `SoilType` class that contains properties related to soil force computations.

### 3D Vector Class
(`kilib/Vec3.hpp`) `Vec3` is a general-purpose 3-dimensional vector class. It supports many different operations.

### Raster
(`kilib/Raster.hpp`) `Raster` is a base class that can read DEM files, such as elevation and contributing area.

### Distributions
(`kilib/Distributions.hpp`) implements several useful probability distribution related functions that are used throughout this code.
