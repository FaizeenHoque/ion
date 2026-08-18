cmake_minimum_required(VERSION 4.4)
project(ion)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenGL REQUIRED)

set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)

add_subdirectory(
        libraries/glfw-3.5.1
        ${CMAKE_BINARY_DIR}/glfw
)

add_library(glad
        libraries/glad/src/glad.c
)

target_include_directories(glad PUBLIC
        libraries/glad/include
)

add_library(imgui
        libraries/imgui/imgui.cpp
        libraries/imgui/imgui_draw.cpp
        libraries/imgui/imgui_tables.cpp
        libraries/imgui/imgui_widgets.cpp
        libraries/imgui/backends/imgui_impl_glfw.cpp
        libraries/imgui/backends/imgui_impl_opengl3.cpp
)

target_include_directories(imgui PUBLIC
        libraries/imgui
        libraries/imgui/backends
)

target_link_libraries(imgui PUBLIC
        glfw
        OpenGL::GL
)

add_executable(ion
        src/main.cpp
        src/app.cpp
        src/headers/app.h
)

target_include_directories(ion PRIVATE
        libraries/glad/include
        libraries/glfw-3.5.1/include
        libraries/glm
        libraries/imgui
        libraries/imgui/backends
)

target_link_libraries(ion PRIVATE
        glad
        glfw
        OpenGL::GL
        imgui
)

file(GLOB SHADER_FILES ${CMAKE_SOURCE_DIR}/src/shaders/*.glsl)

add_custom_command(
        OUTPUT ${CMAKE_BINARY_DIR}/shaders_copied.stamp
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/src/shaders $<TARGET_FILE_DIR:ion>/shaders
        COMMAND ${CMAKE_COMMAND} -E touch ${CMAKE_BINARY_DIR}/shaders_copied.stamp
        DEPENDS ${SHADER_FILES}
)

add_custom_target(copy_shaders ALL DEPENDS ${CMAKE_BINARY_DIR}/shaders_copied.stamp)
add_dependencies(ion copy_shaders)