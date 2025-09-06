cmake_minimum_required(VERSION 3.10)
project(FastEngine)

# Add GLAD
add_library(glad STATIC deps/glad/src/glad.c)
target_include_directories(glad PUBLIC deps/glad/include)

# Add GLFW
add_subdirectory(deps/glfw)

find_package(OpenGL REQUIRED)

# Source files for your engine
file(GLOB SOURCES "src/*.cpp")
add_executable(FastEngine ${SOURCES})

# Includes for your engine
target_include_directories(FastEngine PRIVATE ${CMAKE_SOURCE_DIR}/src)

# Link everything
target_link_libraries(FastEngine PRIVATE glad glfw OpenGL::GL)
