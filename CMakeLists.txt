cmake_minimum_required(VERSION 3.5)

set(projectName KiLib)
set(projectTestName ${projectName}.test)
project(${projectName} VERSION 2.3.2 LANGUAGES CXX)
set(EXTERNAL_INSTALL_LOCATION ${CMAKE_BINARY_DIR}/external)

include(CMake/Sources.cmake.in)
add_library(${projectName} STATIC ${MAIN_SOURCES} )

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

message("Install Directory: ${CMAKE_INSTALL_INCLUDEDIR}")
if ("${CMAKE_INSTALL_INCLUDEDIR}" STREQUAL "")
    set(CMAKE_INSTALL_INCLUDEDIR "include")
    message("Updated Install Directory: ${CMAKE_INSTALL_INCLUDEDIR}")
endif()

# Default build is release
if (NOT CMAKE_BUILD_TYPE OR CMAKE_BUILD_TYPE STREQUAL "")
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "" FORCE)
endif()

  

include(CMake/spdlog.cmake.in)
include(CMake/stats.cmake.in)
include(CMake/yaml-cpp.cmake.in)
include(CMake/KiLibTest.cmake.in)


# Interface for library
target_include_directories(${projectName}
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<BUILD_INTERFACE:${EXTERNAL_INSTALL_LOCATION}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/${projectName}>
    PRIVATE
        ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_INCLUDEDIR}
)

message("Header File install location: ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_INCLUDEDIR}")

# For install dir features
include(GNUInstallDirs)
install(
    TARGETS
        ${projectName}
        spdlog
        yaml-cpp
    EXPORT
        ${projectName}Targets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
install(EXPORT ${projectName}Targets
    FILE ${projectName}Targets.cmake
    NAMESPACE ${projectName}::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${projectName}
)

install(DIRECTORY ${projectName}/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${projectName}/
    FILES_MATCHING 
        PATTERN "*.h*"
)

install(DIRECTORY ${EXTERNAL_INSTALL_LOCATION}/include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/
    FILES_MATCHING 
        PATTERN "*.h*"
        PATTERN "*.ipp"
)

# Define KILIB_RESOURCE_PATH
add_compile_definitions(
    KILIB_RESOURCE_PATH="${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}/KiLib/resources"
    TEST_DIRECTORY="${PROJECT_SOURCE_DIR}/test"
)

message("Resource install location: ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}/KiLib/resources")


#####################
# ConfigVersion file
#####################
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/${projectName}ConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
)

configure_package_config_file(
    ${CMAKE_CURRENT_LIST_DIR}/CMake/${projectName}Config.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/${projectName}Config.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/KiLib
)

## Install all the helper files
install(
    FILES
      ${CMAKE_CURRENT_BINARY_DIR}/${projectName}Config.cmake
      ${CMAKE_CURRENT_BINARY_DIR}/${projectName}ConfigVersion.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/KiLib
)

# Install resources
install(DIRECTORY
    ${CMAKE_CURRENT_SOURCE_DIR}/resources
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/${projectName}
)


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

target_link_libraries(${projectName} PUBLIC yaml-cpp spdlog::spdlog stats gcem)

# Git commit hook installation
include(CMake/GitPreCommitHook.cmake.in)
