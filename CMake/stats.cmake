# stats.cmake.in
#
# Add stats to a project
#
# Written Owen T. Parkins

CPMAddPackage("gh:kthohr/gcem@1.16.0")
target_link_libraries(${projectName} PUBLIC gcem)

# Download stats into a lib/ folder to reduce the amount of time it takes to
# compile
include(FetchContent)
FetchContent_Populate(
  stats_proj
  QUIET
  GIT_REPOSITORY  https://github.com/kthohr/stats
  GIT_TAG         v3.2.0
  SOURCE_DIR      _deps/stats-src
  BINARY_DIR      _deps/stats-build
  SUBBUILD_DIR    _deps/stats-subbuild
)
 
# Stats is special because it doesn't include a cmake project.
# we must add an interface library for use
add_library(stats INTERFACE)
target_include_directories(stats INTERFACE $<BUILD_INTERFACE:${stats_proj_SOURCE_DIR}/include> $<INSTALL_INTERFACE:include>)
target_link_libraries(stats INTERFACE gcem)

# Add STATS_USE_OPENMP and STATS_ENABLE_STDVEC_WRAPPERS to stats log
target_compile_definitions(stats INTERFACE STATS_ENABLE_STDVEC_WRAPPERS)

# Default build is with OpenMP
if (NOT DEFINED STATS_OPENMP_ENABLE OR STATS_OPENMP_ENABLE STREQUAL "YES")
  if(KILIB_COMPILER_IS_GNU_LIKE)
    message("Enabling OpenMP For Stats Lib")
    find_package(OpenMP REQUIRED)
    target_link_libraries(stats INTERFACE OpenMP::OpenMP_CXX)
  endif()
endif()


# Install stats target for others to use
install(TARGETS stats EXPORT statsTargets DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(EXPORT statsTargets DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/stats)

# Link project with stats
target_link_libraries(${projectName} PUBLIC stats)
