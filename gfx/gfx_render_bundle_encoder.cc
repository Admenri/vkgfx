// Copyright 2025 Admenri.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#include "gfx/gfx_render_bundle_encoder.h"

namespace vkgfx {

///////////////////////////////////////////////////////////////////////////////
// GFXRenderBundleEncoder Implement

GFXRenderBundleEncoder::GFXRenderBundleEncoder() {}

GFXRenderBundleEncoder::~GFXRenderBundleEncoder() {}

void GFXRenderBundleEncoder::Draw(uint32_t vertexCount,
                                  uint32_t instanceCount,
                                  uint32_t firstVertex,
                                  uint32_t firstInstance) {}

void GFXRenderBundleEncoder::DrawIndexed(uint32_t indexCount,
                                         uint32_t instanceCount,
                                         uint32_t firstIndex,
                                         int32_t baseVertex,
                                         uint32_t firstInstance) {}

void GFXRenderBundleEncoder::DrawIndexedIndirect(WGPUBuffer indirectBuffer,
                                                 uint64_t indirectOffset) {}

void GFXRenderBundleEncoder::DrawIndirect(WGPUBuffer indirectBuffer,
                                          uint64_t indirectOffset) {}

WGPURenderBundle GFXRenderBundleEncoder::Finish(
    WGPURenderBundleDescriptor const* descriptor) {
  return WGPURenderBundle();
}

void GFXRenderBundleEncoder::InsertDebugMarker(WGPUStringView markerLabel) {}

void GFXRenderBundleEncoder::PopDebugGroup() {}

void GFXRenderBundleEncoder::PushDebugGroup(WGPUStringView groupLabel) {}

void GFXRenderBundleEncoder::SetBindGroup(uint32_t groupIndex,
                                          WGPUBindGroup group,
                                          size_t dynamicOffsetCount,
                                          uint32_t const* dynamicOffsets) {}

void GFXRenderBundleEncoder::SetIndexBuffer(WGPUBuffer buffer,
                                            WGPUIndexFormat format,
                                            uint64_t offset,
                                            uint64_t size) {}

void GFXRenderBundleEncoder::SetLabel(WGPUStringView label) {
  label_ = std::string(label.data, label.length);
}

void GFXRenderBundleEncoder::SetPipeline(WGPURenderPipeline pipeline) {}

void GFXRenderBundleEncoder::SetVertexBuffer(uint32_t slot,
                                             WGPUBuffer buffer,
                                             uint64_t offset,
                                             uint64_t size) {}

}  // namespace vkgfx

///////////////////////////////////////////////////////////////////////////////
// Member Function

GFX_REFCOUNTED_EXPORT(RenderBundleEncoder, vkgfx::GFXRenderBundleEncoder);

GFX_EXPORT void GFX_FUNCTION(RenderBundleEncoderDraw)(
    WGPURenderBundleEncoder renderBundleEncoder,
    uint32_t vertexCount,
    uint32_t instanceCount,
    uint32_t firstVertex,
    uint32_t firstInstance) {
  auto* self = static_cast<vkgfx::GFXRenderBundleEncoder*>(renderBundleEncoder);
  self->Draw(vertexCount, instanceCount, firstVertex, firstInstance);
}

GFX_EXPORT void GFX_FUNCTION(RenderBundleEncoderDrawIndexed)(
    WGPURenderBundleEncoder renderBundleEncoder,
    uint32_t indexCount,
    uint32_t instanceCount,
    uint32_t firstIndex,
    int32_t baseVertex,
    uint32_t firstInstance) {
  auto* self = static_cast<vkgfx::GFXRenderBundleEncoder*>(renderBundleEncoder);
  self->DrawIndexed(indexCount, instanceCount, firstIndex, baseVertex,
                    firstInstance);
}

GFX_EXPORT void GFX_FUNCTION(RenderBundleEncoderDrawIndexedIndirect)(
    WGPURenderBundleEncoder renderBundleEncoder,
    WGPUBuffer indirectBuffer,
    uint64_t indirectOffset) {
  auto* self = static_cast<vkgfx::GFXRenderBundleEncoder*>(renderBundleEncoder);
  self->DrawIndexedIndirect(indirectBuffer, indirectOffset);
}

GFX_EXPORT void GFX_FUNCTION(RenderBundleEncoderDrawIndirect)(
    WGPURenderBundleEncoder renderBundleEncoder,
    WGPUBuffer indirectBuffer,
    uint64_t indirectOffset) {
  auto* self = static_cast<vkgfx::GFXRenderBundleEncoder*>(renderBundleEncoder);
  self->DrawIndirect(indirectBuffer, indirectOffset);
}

GFX_EXPORT WGPURenderBundle GFX_FUNCTION(RenderBundleEncoderFinish)(
    WGPURenderBundleEncoder renderBundleEncoder,
    WGPU_NULLABLE WGPURenderBundleDescriptor const* descriptor) {
  auto* self = static_cast<vkgfx::GFXRenderBundleEncoder*>(renderBundleEncoder);
  return self->Finish(descriptor);
}

GFX_EXPORT void GFX_FUNCTION(RenderBundleEncoderInsertDebugMarker)(
    WGPURenderBundleEncoder renderBundleEncoder,
    WGPUStringView markerLabel) {
  auto* self = static_cast<vkgfx::GFXRenderBundleEncoder*>(renderBundleEncoder);
  self->InsertDebugMarker(markerLabel);
}

GFX_EXPORT void GFX_FUNCTION(RenderBundleEncoderPopDebugGroup)(
    WGPURenderBundleEncoder renderBundleEncoder) {
  auto* self = static_cast<vkgfx::GFXRenderBundleEncoder*>(renderBundleEncoder);
  self->PopDebugGroup();
}

GFX_EXPORT void GFX_FUNCTION(RenderBundleEncoderPushDebugGroup)(
    WGPURenderBundleEncoder renderBundleEncoder,
    WGPUStringView groupLabel) {
  auto* self = static_cast<vkgfx::GFXRenderBundleEncoder*>(renderBundleEncoder);
  self->PushDebugGroup(groupLabel);
}

GFX_EXPORT void GFX_FUNCTION(RenderBundleEncoderSetBindGroup)(
    WGPURenderBundleEncoder renderBundleEncoder,
    uint32_t groupIndex,
    WGPU_NULLABLE WGPUBindGroup group,
    size_t dynamicOffsetCount,
    uint32_t const* dynamicOffsets) {
  auto* self = static_cast<vkgfx::GFXRenderBundleEncoder*>(renderBundleEncoder);
  self->SetBindGroup(groupIndex, group, dynamicOffsetCount, dynamicOffsets);
}

GFX_EXPORT void GFX_FUNCTION(RenderBundleEncoderSetIndexBuffer)(
    WGPURenderBundleEncoder renderBundleEncoder,
    WGPUBuffer buffer,
    WGPUIndexFormat format,
    uint64_t offset,
    uint64_t size) {
  auto* self = static_cast<vkgfx::GFXRenderBundleEncoder*>(renderBundleEncoder);
  self->SetIndexBuffer(buffer, format, offset, size);
}

GFX_EXPORT void GFX_FUNCTION(RenderBundleEncoderSetLabel)(
    WGPURenderBundleEncoder renderBundleEncoder,
    WGPUStringView label) {
  auto* self = static_cast<vkgfx::GFXRenderBundleEncoder*>(renderBundleEncoder);
  self->SetLabel(label);
}

GFX_EXPORT void GFX_FUNCTION(RenderBundleEncoderSetPipeline)(
    WGPURenderBundleEncoder renderBundleEncoder,
    WGPURenderPipeline pipeline) {
  auto* self = static_cast<vkgfx::GFXRenderBundleEncoder*>(renderBundleEncoder);
  self->SetPipeline(pipeline);
}

GFX_EXPORT void GFX_FUNCTION(RenderBundleEncoderSetVertexBuffer)(
    WGPURenderBundleEncoder renderBundleEncoder,
    uint32_t slot,
    WGPU_NULLABLE WGPUBuffer buffer,
    uint64_t offset,
    uint64_t size) {
  auto* self = static_cast<vkgfx::GFXRenderBundleEncoder*>(renderBundleEncoder);
  self->SetVertexBuffer(slot, buffer, offset, size);
}
