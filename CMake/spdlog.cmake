# spdlog.cmake.in
#
# Add spdlog to a project
#
# Written Owen T. Parkins

CPMAddPackage("gh:gabime/spdlog@1.11.0")
target_link_libraries(${projectName} PUBLIC spdlog::spdlog)