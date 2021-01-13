# CMakeLists.txt for Hydrology in KiLib

# Add sources to the main project
target_sources(${projectName} PRIVATE
	BaseHydrology.cpp
	TopModel.cpp
)

# See other CMakeLists.txt files for examples on how to add test sources to this
# module if needed in the future.
