cmake_minimum_required(VERSION 3.16)
project(FractaVista)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SDL3       CONFIG REQUIRED)
find_package(SDL3_image CONFIG REQUIRED)
find_package(imgui      CONFIG REQUIRED)
find_package(glm        CONFIG REQUIRED)

file(GLOB_RECURSE SOURCES "src/*.cpp")

add_library(glad STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/glad/src/glad.c
)
target_include_directories(glad PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/glad/include
)

add_executable(FractaVista ${SOURCES})

target_link_libraries(FractaVista PRIVATE
    SDL3::SDL3
    SDL3_image::SDL3_image
    glad
    imgui::imgui
    glm::glm-header-only
)

target_include_directories(FractaVista PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/glad/include
)

file(GLOB_RECURSE COMPUTE_SHADERS "shaders/compute/*.glsl")
file(GLOB_RECURSE SIMPLE_SHADERS  "shaders/simple/*.vert" "shaders/simple/*.frag")

add_custom_command(TARGET FractaVista POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/shaders $<TARGET_FILE_DIR:FractaVista>/shaders
)
