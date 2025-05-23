cmake_minimum_required(VERSION 3.10)

project(FastEngine)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

add_subdirectory(deps/SDL3 EXCLUDE_FROM_ALL)

include_directories(${CMAKE_SOURCE_DIR}/src)
file(GLOB SOURCES "src/*.cpp")

add_executable(FastEngine ${SOURCES})

target_link_libraries(FastEngine PRIVATE SDL3::SDL3)