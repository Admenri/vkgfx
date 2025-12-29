// Copyright 2025 Admenri.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#include "gfx/gfx_render_pipeline.h"

namespace vkgfx {

///////////////////////////////////////////////////////////////////////////////
// GFXRenderPipeline Implement

GFXRenderPipeline::GFXRenderPipeline() {}

GFXRenderPipeline::~GFXRenderPipeline() {}

WGPUBindGroupLayout GFXRenderPipeline::GetBindGroupLayout(uint32_t groupIndex) {
  return WGPUBindGroupLayout();
}

void GFXRenderPipeline::SetLabel(WGPUStringView label) {
  label_ = std::string(label.data, label.length);
}

}  // namespace vkgfx

///////////////////////////////////////////////////////////////////////////////
// Member Function

GFX_REFCOUNTED_EXPORT(RenderPipeline, vkgfx::GFXRenderPipeline);

GFX_EXPORT WGPUBindGroupLayout GFX_FUNCTION(RenderPipelineGetBindGroupLayout)(
    WGPURenderPipeline renderPipeline,
    uint32_t groupIndex) {
  auto* self = static_cast<vkgfx::GFXRenderPipeline*>(renderPipeline);
  return self->GetBindGroupLayout(groupIndex);
}

GFX_EXPORT void GFX_FUNCTION(RenderPipelineSetLabel)(
    WGPURenderPipeline renderPipeline,
    WGPUStringView label) {
  auto* self = static_cast<vkgfx::GFXRenderPipeline*>(renderPipeline);
  self->SetLabel(label);
}
