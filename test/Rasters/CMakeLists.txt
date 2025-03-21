# Add sources to the test application

set(projectTestName KiLibTest)
target_sources(${projectTestName} PRIVATE
	        Raster.test.cpp
		RasterIterator.test.cpp
		)
