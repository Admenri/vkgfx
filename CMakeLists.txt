cmake_minimum_required(VERSION 3.21)

project(VKGFX LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
include_directories("${PROJECT_SOURCE_DIR}")

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
  set(VOLK_STATIC_DEFINES
    VK_USE_PLATFORM_WIN32_KHR=1
  )
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
  set(VOLK_STATIC_DEFINES
    VK_USE_PLATFORM_XCB_KHR=1
    VK_USE_PLATFORM_XLIB_KHR=1
  )
elseif(CMAKE_SYSTEM_NAME STREQUAL "Android")
  set(VOLK_STATIC_DEFINES
    VK_USE_PLATFORM_ANDROID_KHR=1
  )
else()
  message(FATAL_ERROR "Unsupported platform")
endif()

include_directories(third_party)

add_subdirectory(third_party/volk)
add_subdirectory(third_party/vma)
add_subdirectory(third_party/webgpu-cpp)

add_subdirectory(gfx)
add_subdirectory(tests)
