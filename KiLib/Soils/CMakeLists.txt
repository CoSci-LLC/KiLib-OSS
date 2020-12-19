# CMakeLists.txt for Soils in KiLib

# Add sources to the main project
target_sources(${projectName} PRIVATE
	SoilType.cpp
)

# Add sources to the test application
target_sources(${projectTestName} PRIVATE
	Soil.test.cpp
)