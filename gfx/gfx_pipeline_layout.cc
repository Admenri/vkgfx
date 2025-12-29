// Copyright 2025 Admenri.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#include "gfx/gfx_pipeline_layout.h"

namespace vkgfx {

///////////////////////////////////////////////////////////////////////////////
// GFXPipelineLayout Implement

GFXPipelineLayout::GFXPipelineLayout() {}

GFXPipelineLayout::~GFXPipelineLayout() {}

void GFXPipelineLayout::SetLabel(WGPUStringView label) {
  label_ = std::string(label.data, label.length);
}

}  // namespace vkgfx

///////////////////////////////////////////////////////////////////////////////
// Member Function

GFX_REFCOUNTED_EXPORT(PipelineLayout, vkgfx::GFXPipelineLayout);

GFX_EXPORT void GFX_FUNCTION(PipelineLayoutSetLabel)(
    WGPUPipelineLayout pipelineLayout,
    WGPUStringView label) {
  auto* self = static_cast<vkgfx::GFXPipelineLayout*>(pipelineLayout);
  self->SetLabel(label);
}
