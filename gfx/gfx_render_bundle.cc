// Copyright 2025 Admenri.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#include "gfx/gfx_render_bundle.h"

namespace vkgfx {

///////////////////////////////////////////////////////////////////////////////
// GFXRenderBundle Implement

GFXRenderBundle::GFXRenderBundle() {}

GFXRenderBundle::~GFXRenderBundle() {}

void GFXRenderBundle::SetLabel(WGPUStringView label) {
  label_ = std::string(label.data, label.length);
}

}  // namespace vkgfx

///////////////////////////////////////////////////////////////////////////////
// Member Function

GFX_REFCOUNTED_EXPORT(RenderBundle, vkgfx::GFXRenderBundle);

GFX_EXPORT void GFX_FUNCTION(
    RenderBundleSetLabel)(WGPURenderBundle renderBundle, WGPUStringView label) {
  auto* self = static_cast<vkgfx::GFXRenderBundle*>(renderBundle);
  self->SetLabel(label);
}
