// Copyright 2025 Admenri.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#ifndef GFX_GFX_SURFACE_H_
#define GFX_GFX_SURFACE_H_

#include "gfx/backend_config.h"
#include "gfx/common/refptr.h"

struct WGPUSurfaceImpl {};

namespace vkgfx {

// https://gpuweb.github.io/gpuweb/
class GFXSurface : public RefCounted<GFXSurface>, public WGPUSurfaceImpl {
 public:
  ~GFXSurface();

  GFXSurface(const GFXSurface&) = delete;
  GFXSurface& operator=(const GFXSurface&) = delete;

  void Configure(WGPUSurfaceConfiguration const* config);
  WGPUStatus GetCapabilities(WGPUAdapter adapter,
                             WGPUSurfaceCapabilities* capabilities);
  void GetCurrentTexture(WGPUSurfaceTexture* surfaceTexture);
  WGPUStatus Present();
  void SetLabel(WGPUStringView label);
  void Unconfigure();

 private:
  friend class GFXInstance;
  GFXSurface(RefPtr<GFXInstance> instance, VkSurfaceKHR surface);

  RefPtr<GFXInstance> instance_;

  VkSurfaceKHR surface_;
};

}  // namespace vkgfx

#endif  // GFX_GFX_SURFACE_H_
