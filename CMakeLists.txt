cmake_minimum_required(VERSION 3.27)

project(FractaVista
    VERSION 0.1
    DESCRIPTION "A real-time fractal viewer"
    LANGUAGES CXX C
)

# ————————————————————————————————
# 1) Project-wide Settings
# ————————————————————————————————
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
include(GNUInstallDirs)

# ————————————————————————————————
# 2) Fetch Dependencies (vcpkg, pkg‑config, etc.)
# ————————————————————————————————
find_package(SDL3          CONFIG REQUIRED)
find_package(SDL3_image    CONFIG REQUIRED)
find_package(imgui         CONFIG REQUIRED)
find_package(glm           CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(nfd           CONFIG REQUIRED)
find_package(spdlog	       CONFIG REQUIRED)
find_package(ZLIB                 REQUIRED)
find_package(PNG           CONFIG REQUIRED)
find_package(JPEG                 REQUIRED)

# ————————————————————————————————
# 3) External Libraries: glad
# ————————————————————————————————
add_library(glad STATIC vendor/glad/src/gl.c)
target_include_directories(glad PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/glad/include
)

# ————————————————————————————————
# 4) Executable & Source Files
# ————————————————————————————————
add_executable(FractaVista
    src/main.cpp
    src/app/Application.cpp
    src/core/Window.cpp
    src/fractal/FractalComputer.cpp
    src/gfx/Shader.cpp
    src/gfx/Texture.cpp
    src/ui/CameraController.cpp
    src/ui/Theme.cpp
    src/ui/UIManager.cpp
    src/util/Logger.cpp
)

target_include_directories(FractaVista PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(FractaVista PRIVATE
    SDL3::SDL3
    SDL3_image::SDL3_image
    imgui::imgui
    glm::glm
    nlohmann_json::nlohmann_json
    nfd::nfd
    spdlog::spdlog
    ZLIB::ZLIB
    PNG::PNG
    JPEG::JPEG
    glad
)

# ————————————————————————————————
# 5) Compiler Flags
# ————————————————————————————————
if (MSVC)
    target_compile_options(FractaVista PRIVATE /W4 /permissive-)
else()
    target_compile_options(FractaVista PRIVATE -Wall -Wextra -Wpedantic)
endif()

# ————————————————————————————————
# 6) Post‑build: Copy assets & DLLs
# ————————————————————————————————
add_custom_command(TARGET FractaVista POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/assets"
        "$<TARGET_FILE_DIR:FractaVista>/assets"
    COMMENT "Copying assets to build directory"
)

if (WIN32)
    add_custom_command(TARGET FractaVista POST_BUILD
        COMMAND_EXPAND_LISTS
        COMMAND ${CMAKE_COMMAND} -E copy
            $<TARGET_RUNTIME_DLLS:FractaVista>
            $<TARGET_FILE_DIR:FractaVista>
        COMMENT "Copying runtime DLLs to build directory"
    )
endif()

# ————————————————————————————————
# 7) Install Rules
# ————————————————————————————————
install(TARGETS FractaVista
    RUNTIME     DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY     DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE     DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
install(DIRECTORY assets/ DESTINATION ${CMAKE_INSTALL_BINDIR}/assets)

if (WIN32)
    # This generator expression will automatically include the correct
    # DLLs for the current build type (Debug or Release).
    install(FILES $<TARGET_RUNTIME_DLLS:FractaVista>
        DESTINATION ${CMAKE_INSTALL_BINDIR}
        COMPONENT RuntimeLibraries
        OPTIONAL
    )
endif()
