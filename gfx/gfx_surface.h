// Copyright 2025 Admenri.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#ifndef GFX_GFX_SURFACE_H_
#define GFX_GFX_SURFACE_H_

#include "gfx/common/refptr.h"
#include "gfx/gfx_config.h"
#include "gfx/gfx_instance.h"

struct WGPUSurfaceImpl {};

namespace vkgfx {

// https://gpuweb.github.io/gpuweb/#gpu
class GFXSurface : public RefCounted<GFXSurface>, public WGPUSurfaceImpl {
 public:
  GFXSurface(VkSurfaceKHR surface,
             RefPtr<GFXInstance> instance,
             const std::string& label);
  ~GFXSurface();

  GFXSurface(const GFXSurface&) = delete;
  GFXSurface& operator=(const GFXSurface&) = delete;

  VkSurfaceKHR GetVkHandle() const { return surface_; }

  void Configure(WGPUSurfaceConfiguration const* config);
  WGPUStatus GetCapabilities(WGPUAdapter adapter,
                             WGPUSurfaceCapabilities* capabilities);
  void GetCurrentTexture(WGPUSurfaceTexture* surfaceTexture);
  WGPUStatus Present();
  void SetLabel(WGPUStringView label);
  void Unconfigure();

 private:
  VkSurfaceKHR surface_;

  RefPtr<GFXInstance> instance_;

  std::string label_;
};

}  // namespace vkgfx

#endif  // GFX_GFX_SURFACE_H_
