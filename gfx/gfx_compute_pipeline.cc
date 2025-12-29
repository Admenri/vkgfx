// Copyright 2025 Admenri.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#include "gfx/gfx_compute_pipeline.h"

namespace vkgfx {

///////////////////////////////////////////////////////////////////////////////
// GFXComputePipeline Implement

GFXComputePipeline::GFXComputePipeline() {}

GFXComputePipeline::~GFXComputePipeline() {}

WGPUBindGroupLayout GFXComputePipeline::GetBindGroupLayout(
    uint32_t groupIndex) {
  return WGPUBindGroupLayout();
}

void GFXComputePipeline::SetLabel(WGPUStringView label) {
  label_ = std::string(label.data, label.length);
}

}  // namespace vkgfx

///////////////////////////////////////////////////////////////////////////////
// Member Function

GFX_REFCOUNTED_EXPORT(ComputePipeline, vkgfx::GFXComputePipeline);

GFX_EXPORT WGPUBindGroupLayout GFX_FUNCTION(ComputePipelineGetBindGroupLayout)(
    WGPUComputePipeline computePipeline,
    uint32_t groupIndex) {
  auto* self = static_cast<vkgfx::GFXComputePipeline*>(computePipeline);
  return self->GetBindGroupLayout(groupIndex);
}

GFX_EXPORT void GFX_FUNCTION(ComputePipelineSetLabel)(
    WGPUComputePipeline computePipeline,
    WGPUStringView label) {
  auto* self = static_cast<vkgfx::GFXComputePipeline*>(computePipeline);
  self->SetLabel(label);
}
