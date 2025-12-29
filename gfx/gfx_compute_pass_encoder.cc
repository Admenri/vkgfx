// Copyright 2025 Admenri.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#include "gfx/gfx_compute_pass_encoder.h"

namespace vkgfx {

///////////////////////////////////////////////////////////////////////////////
// GFXComputePassEncoder Implement

GFXComputePassEncoder::GFXComputePassEncoder() {}

GFXComputePassEncoder::~GFXComputePassEncoder() {}

void GFXComputePassEncoder::DispatchWorkgroups(uint32_t workgroupCountX,
                                               uint32_t workgroupCountY,
                                               uint32_t workgroupCountZ) {}

void GFXComputePassEncoder::DispatchWorkgroupsIndirect(
    WGPUBuffer indirectBuffer,
    uint64_t indirectOffset) {}

void GFXComputePassEncoder::End() {}

void GFXComputePassEncoder::InsertDebugMarker(WGPUStringView markerLabel) {}

void GFXComputePassEncoder::PopDebugGroup() {}

void GFXComputePassEncoder::PushDebugGroup(WGPUStringView groupLabel) {}

void GFXComputePassEncoder::SetBindGroup(uint32_t groupIndex,
                                         WGPU_NULLABLE WGPUBindGroup group,
                                         size_t dynamicOffsetCount,
                                         uint32_t const* dynamicOffsets) {}

void GFXComputePassEncoder::SetLabel(WGPUStringView label) {
  label_ = std::string(label.data, label.length);
}

void GFXComputePassEncoder::SetPipeline(WGPUComputePipeline pipeline) {}

}  // namespace vkgfx

///////////////////////////////////////////////////////////////////////////////
// Member Function

GFX_REFCOUNTED_EXPORT(ComputePassEncoder, vkgfx::GFXComputePassEncoder);

GFX_EXPORT void GFX_FUNCTION(ComputePassEncoderDispatchWorkgroups)(
    WGPUComputePassEncoder computePassEncoder,
    uint32_t workgroupCountX,
    uint32_t workgroupCountY,
    uint32_t workgroupCountZ) {
  auto* self = static_cast<vkgfx::GFXComputePassEncoder*>(computePassEncoder);
  self->DispatchWorkgroups(workgroupCountX, workgroupCountY, workgroupCountZ);
}

GFX_EXPORT void GFX_FUNCTION(ComputePassEncoderDispatchWorkgroupsIndirect)(
    WGPUComputePassEncoder computePassEncoder,
    WGPUBuffer indirectBuffer,
    uint64_t indirectOffset) {
  auto* self = static_cast<vkgfx::GFXComputePassEncoder*>(computePassEncoder);
  self->DispatchWorkgroupsIndirect(indirectBuffer, indirectOffset);
}

GFX_EXPORT void GFX_FUNCTION(ComputePassEncoderEnd)(
    WGPUComputePassEncoder computePassEncoder) {
  auto* self = static_cast<vkgfx::GFXComputePassEncoder*>(computePassEncoder);
  self->End();
}

GFX_EXPORT void GFX_FUNCTION(ComputePassEncoderInsertDebugMarker)(
    WGPUComputePassEncoder computePassEncoder,
    WGPUStringView markerLabel) {
  auto* self = static_cast<vkgfx::GFXComputePassEncoder*>(computePassEncoder);
  self->InsertDebugMarker(markerLabel);
}

GFX_EXPORT void GFX_FUNCTION(ComputePassEncoderPopDebugGroup)(
    WGPUComputePassEncoder computePassEncoder) {
  auto* self = static_cast<vkgfx::GFXComputePassEncoder*>(computePassEncoder);
  self->PopDebugGroup();
}

GFX_EXPORT void GFX_FUNCTION(ComputePassEncoderPushDebugGroup)(
    WGPUComputePassEncoder computePassEncoder,
    WGPUStringView groupLabel) {
  auto* self = static_cast<vkgfx::GFXComputePassEncoder*>(computePassEncoder);
  self->PushDebugGroup(groupLabel);
}

GFX_EXPORT void GFX_FUNCTION(ComputePassEncoderSetBindGroup)(
    WGPUComputePassEncoder computePassEncoder,
    uint32_t groupIndex,
    WGPU_NULLABLE WGPUBindGroup group,
    size_t dynamicOffsetCount,
    uint32_t const* dynamicOffsets) {
  auto* self = static_cast<vkgfx::GFXComputePassEncoder*>(computePassEncoder);
  self->SetBindGroup(groupIndex, group, dynamicOffsetCount, dynamicOffsets);
}

GFX_EXPORT void GFX_FUNCTION(ComputePassEncoderSetLabel)(
    WGPUComputePassEncoder computePassEncoder,
    WGPUStringView label) {
  auto* self = static_cast<vkgfx::GFXComputePassEncoder*>(computePassEncoder);
  self->SetLabel(label);
}

GFX_EXPORT void GFX_FUNCTION(ComputePassEncoderSetPipeline)(
    WGPUComputePassEncoder computePassEncoder,
    WGPUComputePipeline pipeline) {
  auto* self = static_cast<vkgfx::GFXComputePassEncoder*>(computePassEncoder);
  self->SetPipeline(pipeline);
}
