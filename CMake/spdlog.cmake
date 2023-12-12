# spdlog.cmake.in
#
# Add spdlog to a project
#
# Written Owen T. Parkins

CPMAddPackage(
  NAME spdlog
  VERSION 1.12.0
  GITHUB_REPOSITORY "gabime/spdlog"
  GIT_TAG "v1.12.0"
  OPTIONS
    "SPDLOG_BUILD_SHARED OFF"
)
target_link_libraries(${projectName} PUBLIC spdlog::spdlog)
