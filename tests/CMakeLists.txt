cmake_minimum_required(VERSION 3.21)

add_executable(test_instance test_instance.cc)
target_link_libraries(test_instance PRIVATE vkgfx webgpu-cpp)
