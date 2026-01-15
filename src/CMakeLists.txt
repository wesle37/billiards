cmake_minimum_required(VERSION 3.31.6)
project(billiards)
include(FetchContent)

# set the output directory for built objects.
# This makes sure that the dynamic library goes into the build directory automatically.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

add_subdirectory(/home/wesle/projects/billiards/src/vendored/SDL EXCLUDE_FROM_ALL)

add_executable(main main.c)
target_link_libraries(main 
PRIVATE SDL3::SDL3)