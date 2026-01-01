cmake_minimum_required(VERSION 3.10)
project(FastEngine)

add_subdirectory(RenderSurface)
add_subdirectory(FastEngineCore)

file(GLOB SOURCES "src/*.cpp")
add_executable(FastEngine ${SOURCES})

target_include_directories(FastEngine PRIVATE ${CMAKE_SOURCE_DIR}/src)

target_link_libraries(FastEngine PRIVATE RenderSurface FastEngineCore)
