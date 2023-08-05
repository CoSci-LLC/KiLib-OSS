# KiLib Build using CMake

cmake_minimum_required(VERSION 3.13)

# Set the project and test app name
set(projectName KiLibObjLib)
set(projectTestName KiLib.test)

# Create the project name and version
project(KiLib VERSION 5.1.0 LANGUAGES C CXX)

# Detect compiler and set variable
if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang|AppleClang")
    set(KILIB_COMPILER_IS_GNU_LIKE TRUE)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    set(KILIB_COMPILER_IS_GNU_LIKE FALSE)
else()
    # Throw an error if the compiler is not supported
    message(FATAL_ERROR "Compiler not supported")
endif()

# Add KiLib Object Library
add_library(${projectName} OBJECT "") 

# shared libraries need PIC
set_property(TARGET ${projectName} PROPERTY POSITION_INDEPENDENT_CODE 1)

# shared and static libraries built from the same object files
add_library(KiLib STATIC $<TARGET_OBJECTS:${projectName}>)
target_link_libraries(KiLib PUBLIC ${projectName} -lstdc++fs)

# Ensure that the install directory is all setup properly
message("Install Directory: ${CMAKE_INSTALL_INCLUDEDIR}")
if ("${CMAKE_INSTALL_INCLUDEDIR}" STREQUAL "")
    set(CMAKE_INSTALL_INCLUDEDIR "include")
    message("Header File install location: ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_INCLUDEDIR}")
endif()

# Include dependencies and helper functions
include(CMake/KiLibOptions.cmake)
include(CMake/CPM.cmake)
include(CMake/target_sources_test.cmake)
include(CMake/spdlog.cmake)
include(CMake/stats.cmake)
include(CMake/libtiff.cmake)

# Include the test application
if(KILIB_BUILD_TESTS)
    message("-- KILIB: Adding KiLib Tests")
    # Enable testing for the project
    enable_testing()
    add_subdirectory(test)
endif()

# Include the KiLib source files
add_subdirectory(KiLib)
SetKiLibOptions(${projectName})

# Interface for library -- This is what directories are included when building
# KiLib
target_include_directories(${projectName}
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        "$<INSTALL_INTERFACE:include>"
)

install(TARGETS ${projectName} tiff spdlog
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

# Default issue is main
if (NOT COSCI_PROJECT_NAME OR COSCI_PROJECT_NAME STREQUAL "")
    set(COSCI_PROJECT_NAME "kilib-oss" CACHE STRING "" FORCE)
endif()

add_custom_target(push_to_repo
	COMMAND bash ${CMAKE_SOURCE_DIR}/CMake/gen_oras_config.sh > oras_config.json
	COMMAND oras push harbor.nibious.com/cosci-llc/${COSCI_PROJECT_NAME}:$ENV{GO_REVISION_GIT}-${CMAKE_BUILD_TYPE}-$ENV{OS_RELEASE} --config oras_config.json libKiLib.a
)
