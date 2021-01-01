# CMakeLists.txt file to include each module

# Exceptions doesn't have a CMakeLists.txt file since it only has header files

# Include Raster Module
add_subdirectory(Raster)

# Include Soils Module
add_subdirectory(Soils)

# Include Utils Module
add_subdirectory(Utils)