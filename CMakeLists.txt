cmake_minimum_required(VERSION 3.10)
project(FastEngine)

# Add Screen class
add_subdirectory(CoreScreen)

# Source files for your engine
file(GLOB SOURCES "src/*.cpp")
add_executable(FastEngine ${SOURCES})

# Includes for your engine
target_include_directories(FastEngine PRIVATE ${CMAKE_SOURCE_DIR}/src)

# Link everything
target_link_libraries(FastEngine PRIVATE CoreScreen)
