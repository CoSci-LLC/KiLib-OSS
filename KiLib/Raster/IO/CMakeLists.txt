# CMakeLists.txt for Raster IO in KiLib

# Add sources to the main project
target_sources(${projectName} PRIVATE
	Raster_DEM.cpp
	Raster_TIFF.cpp
)

# Add sources to the test application
target_sources_test(${projectTestName} PRIVATE
	RasterIO.test.cpp
)