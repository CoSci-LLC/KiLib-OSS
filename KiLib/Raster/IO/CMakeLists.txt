# CMakeLists.txt for Raster IO in KiLib

# Add sources to the main project
target_sources(${projectName} PRIVATE
	Raster_DEM.cpp
	Raster_TIFF.cpp
)
