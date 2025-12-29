// Copyright 2025 Admenri.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#include "gfx/gfx_texture_view.h"

namespace vkgfx {

///////////////////////////////////////////////////////////////////////////////
// GFXTextureView Implement

GFXTextureView::GFXTextureView() {}

GFXTextureView::~GFXTextureView() {}

void GFXTextureView::SetLabel(WGPUStringView label) {
  label_ = std::string(label.data, label.length);
}

}  // namespace vkgfx

///////////////////////////////////////////////////////////////////////////////
// Member Function

GFX_REFCOUNTED_EXPORT(TextureView, vkgfx::GFXTextureView);

GFX_EXPORT void GFX_FUNCTION(TextureViewSetLabel)(WGPUTextureView textureView,
                                                  WGPUStringView label) {
  auto* self = static_cast<vkgfx::GFXTextureView*>(textureView);
  self->SetLabel(label);
}
