// Copyright 2025 Admenri.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#include "gfx/gfx_bind_group.h"

namespace vkgfx {

///////////////////////////////////////////////////////////////////////////////
// GFXBindGroup Implement

GFXBindGroup::GFXBindGroup() {}

GFXBindGroup::~GFXBindGroup() {}

void GFXBindGroup::SetLabel(WGPUStringView label) {
  label_ = std::string(label.data, label.length);
}

}  // namespace vkgfx

///////////////////////////////////////////////////////////////////////////////
// Member Function

GFX_REFCOUNTED_EXPORT(BindGroup, vkgfx::GFXBindGroup);

GFX_EXPORT void GFX_FUNCTION(BindGroupSetLabel)(WGPUBindGroup bindGroup,
                                                WGPUStringView label) {
  auto* self = static_cast<vkgfx::GFXBindGroup*>(bindGroup);
  self->SetLabel(label);
}
