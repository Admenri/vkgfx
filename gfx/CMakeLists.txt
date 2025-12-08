cmake_minimum_required(VERSION 3.21)

set(GFX_SRC
  common/log.cc
  common/log.h
  common/platform.h
  common/refptr.h
  common/weakptr.h
  backend_config.h
  gfx_adapter.cc
  gfx_adapter.h
  gfx_device.cc
  gfx_device.h
  gfx_instance.cc
  gfx_instance.h
)

add_library(vkgfx STATIC ${GFX_SRC})
target_link_libraries(vkgfx PUBLIC volk::volk webgpu-cpp-header)
