# CMakeLists.txt for Soils in KiLib
# Add sources to the main project
target_sources(${projectName} PRIVATE
	Soil.cpp
	UserDefined.cpp
)

# Add sources to the test application
target_sources_test(${projectTestName} PRIVATE
	Soil.test.cpp
	UserDefined.test.cpp
	Decorators.test.cpp
)
