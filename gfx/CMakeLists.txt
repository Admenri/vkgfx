cmake_minimum_required(VERSION 3.21)

set(VKGFX_GFX_SRC
  common/constants.h
  common/log.cc
  common/log.h
  common/platform.h
  common/refptr.h
  backend_config.h
  gfx_adapter.cc
  gfx_adapter.h
  gfx_device.cc
  gfx_device.h
  gfx_instance.cc
  gfx_instance.h
  gfx_surface.cc
  gfx_surface.h
  gfx_utils.h
  gfx_wgpu.cc
  gfx_wgpu.h
)

add_library(vkgfx STATIC ${VKGFX_GFX_SRC})
target_link_libraries(vkgfx
 PUBLIC
  volk::volk
  webgpu-cpp-header
)
