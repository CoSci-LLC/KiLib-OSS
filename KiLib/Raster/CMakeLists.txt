# CMakeLists.txt for Raster in KiLib

# Add sources to the main project
target_sources(${projectName} PRIVATE
	Raster.cpp
)

# See other CMakeLists.txt files for examples on how to add test sources to this
# module if needed in the future.

# Add sources to the test application
target_sources_test(${projectTestName} PRIVATE
	Raster.test.cpp
)

add_subdirectory(IO)