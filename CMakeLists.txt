# KiLib Build using CMake

cmake_minimum_required(VERSION 3.13)

# Set the project and test app name
set(projectName KiLibObjLib)
set(projectTestName KiLib.test)

# Create the project name and version
project(KiLib VERSION 5.0.0 LANGUAGES CXX)


# Add KiLib Object Library
add_library(${projectName} OBJECT "") 

# shared libraries need PIC
set_property(TARGET ${projectName} PROPERTY POSITION_INDEPENDENT_CODE 1)

# shared and static libraries built from the same object files
add_library(KiLib STATIC $<TARGET_OBJECTS:${projectName}>)
target_link_libraries(KiLib PUBLIC ${projectName})

# Ensure that the install directory is all setup properly
message("Install Directory: ${CMAKE_INSTALL_INCLUDEDIR}")
if ("${CMAKE_INSTALL_INCLUDEDIR}" STREQUAL "")
    set(CMAKE_INSTALL_INCLUDEDIR "include")
    message("Header File install location: ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_INCLUDEDIR}")
endif()

# Default build is release
if (NOT CMAKE_BUILD_TYPE OR CMAKE_BUILD_TYPE STREQUAL "")
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "" FORCE)
endif()

# If we are debugging, add debug statements to every project
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    if (NOT CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        # only add the Werror flag for debug builds. Production builds won't have this.
        target_compile_options(${projectName} PRIVATE -Werror)
    endif()
    add_compile_definitions(SPDLOG_ACTIVE_LEVEL=1)

    # Should we build the test application?
    if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
        message("Set to build KiLib Tests. To disable, define KILIB_BUILD_TESTS as NO")
        set(KILIB_BUILD_TESTS "YES" CACHE STRING "Build KiLib Tests" FORCE)
    endif()
endif()

# Include dependencies and helper functions
include(CMake/gdal.cmake.in)
include(CMake/target_sources_test.cmake)
include(CMake/spdlog.cmake.in)
include(CMake/stats.cmake.in)
include(CMake/eigen.cmake.in)

# Include the test application
if(KILIB_BUILD_TESTS)
    message("Adding KiLib Tests")
    # Enable testing for the project
    enable_testing()
    add_subdirectory(test)
endif()

# Include the KiLib source files
add_subdirectory(KiLib)

# Interface for library -- This is what directories are included when building
# KiLib
target_include_directories(${projectName}
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        "$<INSTALL_INTERFACE:include>"
)

install(TARGETS ${projectName} spdlog
        EXPORT KiLibTargets
        INCLUDES DESTINATION include
)

install(EXPORT KiLibTargets
        FILE KiLibTargets.cmake
        NAMESPACE KiLib::
        DESTINATION lib/cmake
)

install(TARGETS KiLib 
        EXPORT KiLibTargets
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib  
)


# Define compiler options for each compiler
if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
    # using Clang
    message(STATUS "Clang detected, adding compile flags")
    target_compile_options(${projectName} PRIVATE -Wall)
    # Enforce C++17 features
    target_compile_features(${projectName} PRIVATE cxx_std_17)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    # using GCC
    message(STATUS "GCC detected, adding compile flags")
    target_compile_options(${projectName} PRIVATE -Wall)
    # Enforce C++17 features
    target_compile_features(${projectName} PRIVATE cxx_std_17)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    # using Visual Studio C++
    message(STATUS "msvc detected, adding compile flags")
    target_compile_options(${projectName} PRIVATE  /std:c++latest /EHsc)
endif()
