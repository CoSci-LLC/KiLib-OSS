# CMakeLists.txt for SoiLDepth in KiLib

# Add sources to the main project
target_sources(${projectName} PRIVATE
	LinInterpDepth.cpp
	BaseSoilDepth.cpp
)

# See other CMakeLists.txt files for examples on how to add test sources to this
# module if needed in the future.