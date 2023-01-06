CPMAddPackage(
  NAME TIFF
  VERSION 4.5.0
  GITLAB_REPOSITORY "libtiff/libtiff"
  GIT_TAG "v4.5.0"
  OPTIONS
  	"tiff-tests OFF"
	  "tiff-tools OFF"
	  "tiff-contrib OFF"
	  "tiff-docs OFF"
)
target_link_libraries(${projectName} PUBLIC TIFF::TIFF)