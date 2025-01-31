# KiLib Build using CMake

cmake_minimum_required(VERSION 3.15)

# Set the project name 
set(projectName KiLibObjLib)

# Create the project name and version
project(KiLib VERSION 6.1.0 LANGUAGES C CXX)

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
target_link_libraries(KiLib PUBLIC TIFF::TIFF spdlog::spdlog stats)
target_link_libraries(KiLib PUBLIC ${projectName})

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

# Include the KiLib source files
add_subdirectory(KiLib)
SetKiLibOptions(${projectName})

# If we are debug build, let's build the unit tests
#IF(CMAKE_BUILD_TYPE MATCHES DEBUG)
	add_subdirectory(test)
	#ENDIF(CMAKE_BUILD_TYPE MATCHES DEBUG)


# Interface for library -- This is what directories are included when building
# KiLib
target_include_directories(${projectName}
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        "$<INSTALL_INTERFACE:include>"
)

install(TARGETS ${projectName} tiff spdlog stats
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
