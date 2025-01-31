# Setup the testing executable
# 
# Written by Owen T. Parkins

include(CTest)
include(GoogleTest)

include(../CMake/Gtest.cmake)

set(projectTestName KiLibTest)

# Add the test exe
add_executable(${projectTestName}
	main.cpp
)
SetKiLibOptions(${projectTestName})

add_subdirectory(./Rasters)
# Codecov
if (KILIB_COMPILER_IS_GNU_LIKE)
    message("-- KILIB: Adding coverage flag")
    target_compile_options(${projectTestName} PUBLIC --coverage)
endif()


# Perhaps make a KILIB_BUILD_COVERAGE flag?
# Maybe we just want to build coverage if we are building the tests?
# Merge the top two into a function in SetKiLibOptions.cmake
# We are building as stand-alone project so build the code coverage project
include(../CMake/CodeCoverage.cmake)
append_coverage_compiler_flags()
setup_target_for_coverage_gcovr_html(NAME code_coverage EXECUTABLE ${projectTestName})
 

# Need this to help find test files
target_compile_definitions(${projectTestName} PUBLIC
		TEST_DIRECTORY="${PROJECT_SOURCE_DIR}/test"
)

# Link to the KiLib library
target_link_libraries(${projectTestName} gtest_main KiLib)

target_include_directories(${projectTestName}
PUBLIC
		${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_INCLUDEDIR}
PRIVATE
		../
)

# Discover tests
gtest_discover_tests(${projectTestName})
add_test(NAME KiLibTest COMMAND ${projectTestName})

install(TARGETS ${projectTestName})



