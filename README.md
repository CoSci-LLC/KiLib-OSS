# KiLib-OSS 
#### Version v2.3.0
*A Scientific Library for "Earth" (Ki) surface processes*

## Dependencies
All dependencies required for this library will be download via the CMake build process

## Building and Installation

### Linux
```
$ mkdir build
$ cd build 
$ cmake .. -DCMAKE_INSTALL_PREFIX=<install location>
$ make
$ make install
```

### Windows
Visual Studio 2019 is the preferred method for building KiLib on Windows. CMake is also required.

Clone or download the KiLib Library and open a VS 2019 Developer Console in that directory. Then:
```
mkdir build
cd build
cmake ..
cmake --build .
```

## Usage
To automatically download KiLib, add the following to your CMakeLists.txt:
```
### KiLib Install Process

set(EXTERNAL_INSTALL_LOCATION ${CMAKE_BINARY_DIR}/external)

if(WIN32)
  set(BYPRODUCT_SPDLOG ${EXTERNAL_INSTALL_LOCATION}/lib/spdlogd.lib)
  set(BYPRODUCT_YAMLCPP ${EXTERNAL_INSTALL_LOCATION}/lib/libyaml-cppmdd.lib)
  set(BYPRODUCT_KILIB ${EXTERNAL_INSTALL_LOCATION}/lib/KiLib.lib)
endif(WIN32)

if(UNIX)
  set(BYPRODUCT_SPDLOG ${EXTERNAL_INSTALL_LOCATION}/lib/libspdlog.a)
  set(BYPRODUCT_YAMLCPP ${EXTERNAL_INSTALL_LOCATION}/lib/libyaml-cpp.a)
  set(BYPRODUCT_KILIB ${EXTERNAL_INSTALL_LOCATION}/lib/libKiLib.a)
endif(UNIX)


# KiLib
include(ExternalProject)
ExternalProject_Add(project_KiLib
  GIT_REPOSITORY git@github.com:CoSci-LLC/KiLib.git
  GIT_TAG issue/\\\#44
  CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${EXTERNAL_INSTALL_LOCATION} -DCMAKE_PROJECT_NAME=${CMAKE_PROJECT_NAME} -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_INCLUDEDIR=include
  BUILD_BYPRODUCTS  ${BYPRODUCT_SPDLOG} ${BYPRODUCT_YAMLCPP} ${BYPRODUCT_KILIB}
)
include_directories(${EXTERNAL_INSTALL_LOCATION}/include/)
include_directories(${EXTERNAL_INSTALL_LOCATION}/include/KiLib/)
add_library(KiLib STATIC IMPORTED)
set_property(TARGET KiLib PROPERTY IMPORTED_LOCATION ${BYPRODUCT_KILIB})

# Spdlog
add_library(spdlog STATIC IMPORTED GLOBAL)
set_property(TARGET spdlog PROPERTY IMPORTED_LOCATION ${BYPRODUCT_SPDLOG})
add_dependencies(spdlog project_KiLib)

# yaml-cpp
add_library(yaml-cpp STATIC IMPORTED GLOBAL)
set_property(TARGET yaml-cpp PROPERTY IMPORTED_LOCATION ${BYPRODUCT_YAMLCPP})
add_dependencies(yaml-cpp project_KiLib)

target_link_libraries(KiLib INTERFACE yaml-cpp spdlog)

target_link_libraries(${projectName} KiLib)
```

Be sure to replace `${projectName}` with your project name.

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
