// Copyright 2025 Admenri.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#include "gfx/gfx_surface.h"

#include "gfx/gfx_instance.h"

namespace vkgfx {

///////////////////////////////////////////////////////////////////////////////
// GFXSurface Implement

GFXSurface::GFXSurface(RefPtr<GFXInstance> instance, VkSurfaceKHR surface)
    : instance_(instance), surface_(surface) {}

GFXSurface::~GFXSurface() {
  if (surface_)
    vkDestroySurfaceKHR(**instance_, surface_, nullptr);
}

void GFXSurface::Configure(WGPUSurfaceConfiguration const* config) {}

WGPUStatus GFXSurface::GetCapabilities(WGPUAdapter adapter,
                                       WGPUSurfaceCapabilities* capabilities) {
  return WGPUStatus();
}

void GFXSurface::GetCurrentTexture(WGPUSurfaceTexture* surfaceTexture) {}

WGPUStatus GFXSurface::Present() {
  return WGPUStatus();
}

void GFXSurface::SetLabel(WGPUStringView label) {}

void GFXSurface::Unconfigure() {}

///////////////////////////////////////////////////////////////////////////////
// Member Function

GFX_REFCOUNTED_EXPORT(Surface, vkgfx::GFXSurface);

GFX_EXPORT void GFX_FUNCTION(SurfaceConfigure)(
    WGPUSurface surface,
    WGPUSurfaceConfiguration const* config) {
  auto* self = static_cast<vkgfx::GFXSurface*>(surface);
  self->Configure(config);
}

GFX_EXPORT WGPUStatus
GFX_FUNCTION(SurfaceGetCapabilities)(WGPUSurface surface,
                                     WGPUAdapter adapter,
                                     WGPUSurfaceCapabilities* capabilities) {
  auto* self = static_cast<vkgfx::GFXSurface*>(surface);
  return self->GetCapabilities(adapter, capabilities);
}

GFX_EXPORT void GFX_FUNCTION(SurfaceGetCurrentTexture)(
    WGPUSurface surface,
    WGPUSurfaceTexture* surfaceTexture) {
  auto* self = static_cast<vkgfx::GFXSurface*>(surface);
  self->GetCurrentTexture(surfaceTexture);
}

GFX_EXPORT WGPUStatus GFX_FUNCTION(SurfacePresent)(WGPUSurface surface) {
  auto* self = static_cast<vkgfx::GFXSurface*>(surface);
  return self->Present();
}

GFX_EXPORT void GFX_FUNCTION(SurfaceSetLabel)(WGPUSurface surface,
                                              WGPUStringView label) {
  auto* self = static_cast<vkgfx::GFXSurface*>(surface);
  self->SetLabel(label);
}

GFX_EXPORT void GFX_FUNCTION(SurfaceUnconfigure)(WGPUSurface surface) {
  auto* self = static_cast<vkgfx::GFXSurface*>(surface);
  self->Unconfigure();
}

}  // namespace vkgfx
