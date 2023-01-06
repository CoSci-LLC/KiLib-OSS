# Setup the testing executable
# 
# Written by Owen T. Parkins

include(CTest)
include(GoogleTest)

include(../CMake/Gtest.cmake)

# Add the test exe
add_executable(${projectTestName}
	main.cpp
)
SetKiLibOptions(${projectTestName})

# Codecov
if (KILIB_COMPILER_IS_GNU_LIKE)
    target_compile_options(${projectTestName} PRIVATE --coverage)
endif()

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
		./
)

# Discover tests
gtest_discover_tests(${projectTestName})
add_test(NAME KiLibTest COMMAND ${projectTestName})
