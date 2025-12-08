cmake_minimum_required(VERSION 3.21)

project(VKGFX LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
include_directories("${PROJECT_SOURCE_DIR}")

add_subdirectory(third_party/volk)
add_subdirectory(third_party/webgpu-cpp)

add_subdirectory(gfx)
add_subdirectory(tests)
