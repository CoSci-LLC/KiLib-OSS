# KiLib Build using CMake

cmake_minimum_required(VERSION 3.13)

# Set the project and test app name
set(projectName KiLib)
set(projectTestName ${projectName}.test)

# Create the project name and version
project(${projectName} VERSION 2.4.3 LANGUAGES CXX)


# Add KiLib Library
add_library(${projectName} STATIC "") 

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

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
    add_compile_definitions(SPDLOG_ACTIVE_LEVEL=1)

    # Should we build the test application?
    if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
        message("Set to build KiLib Tests. To disable, define KILIB_BUILD_TESTS as NO")
        set(KILIB_BUILD_TESTS "YES" CACHE STRING "Build KiLib Tests" FORCE)
    endif()
endif()

# Include dependencies and helper functions
include(CMake/target_sources_test.cmake)
include(CMake/spdlog.cmake.in)
include(CMake/stats.cmake.in)
include(CMake/yaml-cpp.cmake.in)
include(CMake/libtiff.cmake.in)

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
    PRIVATE
        ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_INCLUDEDIR}
)

# Define KILIB_RESOURCE_PATH
target_compile_definitions(${projectName} PUBLIC
    KILIB_RESOURCE_PATH="${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}/KiLib/resources"
)

# Install soil resources
message("Resource install location: ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}/KiLib/resources")
install(DIRECTORY
    ${CMAKE_CURRENT_SOURCE_DIR}/resources
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/${projectName}
)

# Define compiler options for each compiler
if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    # using Clang
    message(STATUS "Clang detected, adding compile flags")
    target_compile_options(${projectName} PRIVATE -Wall -Werror)
    # Enforce C++17 features
    target_compile_features(${projectName} PRIVATE cxx_std_17)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    # using GCC
    message(STATUS "GCC detected, adding compile flags")
    target_compile_options(${projectName} PRIVATE -Wall -Werror)
    # Enforce C++17 features
    target_compile_features(${projectName} PRIVATE cxx_std_17)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    # using Visual Studio C++
    message(STATUS "msvc detected, adding compile flags")
    target_compile_options(${projectName} PRIVATE  /std:c++latest /EHsc)
endif()

# Git commit hook installation
include(CMake/GitPreCommitHook.cmake.in)
