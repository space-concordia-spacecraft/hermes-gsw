# Minimum Version
cmake_minimum_required(VERSION 3.17)

# Project name
project(GSW)

# C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Includes
include(build/include/utils.cmake)

# Required libraries
find_package(glfw3 CONFIG REQUIRED)
find_package(glad REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(imguizmo CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(assimp CONFIG REQUIRED)
find_package(spdlog REQUIRED)

# Clump source files from src
file(GLOB_RECURSE GSW_SRC "src/**.cpp")

# Create library from source
add_executable("GSW" ${GSW_SRC})

# Link target
target_link_libraries("GSW" PUBLIC glfw glad::glad imgui::imgui imguizmo::imguizmo glm assimp::assimp spdlog::spdlog_header_only)
target_include_directories("GSW" PUBLIC include ${STB_INCLUDE_DIRS})
target_precompile_headers("GSW" PUBLIC include/pch.h)

# Copy assets to build folder
add_dir_copy("GSW" "$<TARGET_FILE_DIR:GSW>" "assets")