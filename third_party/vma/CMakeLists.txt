cmake_minimum_required(VERSION 3.21)

add_library(vma STATIC vma.cc)
target_link_libraries(vma PUBLIC volk::volk)
target_compile_definitions(vma
 PUBLIC
  VMA_DYNAMIC_VULKAN_FUNCTIONS=1
  VMA_VOLK_HEADER_PATH="volk.h"
)
