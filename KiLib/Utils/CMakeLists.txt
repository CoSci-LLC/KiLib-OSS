# CMakeLists.txt for Utils in KiLib

# Add sources to the main project
target_sources(${projectName} PRIVATE
	CSVReader.cpp
	NewtonRaphson.cpp
	Random.cpp
	StatisticalOutput.cpp
)

# Add sources to the test application
target_sources_test(${projectTestName} PRIVATE
	CSVReader.test.cpp
	NewtonRaphson.test.cpp
	Random.test.cpp
	Sequence.test.cpp
	StatisticalOutput.test.cpp
)