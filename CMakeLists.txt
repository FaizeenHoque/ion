cmake_minimum_required(VERSION 4.4)
project(ion)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# OpenGL
find_package(OpenGL REQUIRED)

# GLFW
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)

add_subdirectory(
        libraries/glfw-3.5.1
        ${CMAKE_BINARY_DIR}/glfw
)

# GLAD
add_library(glad
        libraries/glad/src/glad.c
)

target_include_directories(glad PUBLIC
        libraries/glad/include
)

# ion
add_executable(ion src/main.cpp
        src/app.cpp
        headers/app.h)

# Include directories
target_include_directories(ion PRIVATE
        libraries/glad/include
        libraries/glfw-3.5.1/include
        libraries/glm
)

# Libraries
target_link_libraries(ion PRIVATE
        glad
        glfw
        OpenGL::GL
)