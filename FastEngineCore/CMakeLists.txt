cmake_minimum_required(VERSION 3.10)
project(FastEngineCore)

file(GLOB SOURCES "src/*.cpp")
add_library(FastEngineCore STATIC ${SOURCES})

target_include_directories(FastEngineCore PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(FastEngineCore PUBLIC RenderSurface)