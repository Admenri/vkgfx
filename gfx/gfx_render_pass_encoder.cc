// Copyright 2025 Admenri.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#include "gfx/gfx_render_pass_encoder.h"

namespace vkgfx {

///////////////////////////////////////////////////////////////////////////////
// GFXRenderPassEncoder Implement

GFXRenderPassEncoder::GFXRenderPassEncoder() {}

GFXRenderPassEncoder::~GFXRenderPassEncoder() {}

void GFXRenderPassEncoder::BeginOcclusionQuery(uint32_t queryIndex) {}

void GFXRenderPassEncoder::Draw(uint32_t vertexCount,
                                uint32_t instanceCount,
                                uint32_t firstVertex,
                                uint32_t firstInstance) {}

void GFXRenderPassEncoder::DrawIndexed(uint32_t indexCount,
                                       uint32_t instanceCount,
                                       uint32_t firstIndex,
                                       int32_t baseVertex,
                                       uint32_t firstInstance) {}

void GFXRenderPassEncoder::DrawIndexedIndirect(WGPUBuffer indirectBuffer,
                                               uint64_t indirectOffset) {}

void GFXRenderPassEncoder::DrawIndirect(WGPUBuffer indirectBuffer,
                                        uint64_t indirectOffset) {}

void GFXRenderPassEncoder::End() {}

void GFXRenderPassEncoder::EndOcclusionQuery() {}

void GFXRenderPassEncoder::ExecuteBundles(size_t bundleCount,
                                          WGPURenderBundle const* bundles) {}

void GFXRenderPassEncoder::InsertDebugMarker(WGPUStringView markerLabel) {}

void GFXRenderPassEncoder::PopDebugGroup() {}

void GFXRenderPassEncoder::PushDebugGroup(WGPUStringView groupLabel) {}

void GFXRenderPassEncoder::SetBindGroup(uint32_t groupIndex,
                                        WGPUBindGroup group,
                                        size_t dynamicOffsetCount,
                                        uint32_t const* dynamicOffsets) {}

void GFXRenderPassEncoder::SetBlendConstant(WGPUColor const* color) {}

void GFXRenderPassEncoder::SetIndexBuffer(WGPUBuffer buffer,
                                          WGPUIndexFormat format,
                                          uint64_t offset,
                                          uint64_t size) {}

void GFXRenderPassEncoder::SetLabel(WGPUStringView label) {
  label_ = std::string(label.data, label.length);
}

void GFXRenderPassEncoder::SetPipeline(WGPURenderPipeline pipeline) {}

void GFXRenderPassEncoder::SetScissorRect(uint32_t x,
                                          uint32_t y,
                                          uint32_t width,
                                          uint32_t height) {}

void GFXRenderPassEncoder::SetStencilReference(uint32_t reference) {}

void GFXRenderPassEncoder::SetVertexBuffer(uint32_t slot,
                                           WGPUBuffer buffer,
                                           uint64_t offset,
                                           uint64_t size) {}

void GFXRenderPassEncoder::SetViewport(float x,
                                       float y,
                                       float width,
                                       float height,
                                       float minDepth,
                                       float maxDepth) {}

}  // namespace vkgfx

///////////////////////////////////////////////////////////////////////////////
// Member Function

GFX_REFCOUNTED_EXPORT(RenderPassEncoder, vkgfx::GFXRenderPassEncoder);

GFX_EXPORT void GFX_FUNCTION(RenderPassEncoderBeginOcclusionQuery)(
    WGPURenderPassEncoder renderPassEncoder,
    uint32_t queryIndex) {
  auto* self = static_cast<vkgfx::GFXRenderPassEncoder*>(renderPassEncoder);
  self->BeginOcclusionQuery(queryIndex);
}

GFX_EXPORT void GFX_FUNCTION(RenderPassEncoderDraw)(
    WGPURenderPassEncoder renderPassEncoder,
    uint32_t vertexCount,
    uint32_t instanceCount,
    uint32_t firstVertex,
    uint32_t firstInstance) {
  auto* self = static_cast<vkgfx::GFXRenderPassEncoder*>(renderPassEncoder);
  self->Draw(vertexCount, instanceCount, firstVertex, firstInstance);
}

GFX_EXPORT void GFX_FUNCTION(RenderPassEncoderDrawIndexed)(
    WGPURenderPassEncoder renderPassEncoder,
    uint32_t indexCount,
    uint32_t instanceCount,
    uint32_t firstIndex,
    int32_t baseVertex,
    uint32_t firstInstance) {
  auto* self = static_cast<vkgfx::GFXRenderPassEncoder*>(renderPassEncoder);
  self->DrawIndexed(indexCount, instanceCount, firstIndex, baseVertex,
                    firstInstance);
}

GFX_EXPORT void GFX_FUNCTION(RenderPassEncoderDrawIndexedIndirect)(
    WGPURenderPassEncoder renderPassEncoder,
    WGPUBuffer indirectBuffer,
    uint64_t indirectOffset) {
  auto* self = static_cast<vkgfx::GFXRenderPassEncoder*>(renderPassEncoder);
  self->DrawIndexedIndirect(indirectBuffer, indirectOffset);
}

GFX_EXPORT void GFX_FUNCTION(RenderPassEncoderDrawIndirect)(
    WGPURenderPassEncoder renderPassEncoder,
    WGPUBuffer indirectBuffer,
    uint64_t indirectOffset) {
  auto* self = static_cast<vkgfx::GFXRenderPassEncoder*>(renderPassEncoder);
  self->DrawIndirect(indirectBuffer, indirectOffset);
}

GFX_EXPORT void GFX_FUNCTION(RenderPassEncoderEnd)(
    WGPURenderPassEncoder renderPassEncoder) {
  auto* self = static_cast<vkgfx::GFXRenderPassEncoder*>(renderPassEncoder);
  self->End();
}

GFX_EXPORT void GFX_FUNCTION(RenderPassEncoderEndOcclusionQuery)(
    WGPURenderPassEncoder renderPassEncoder) {
  auto* self = static_cast<vkgfx::GFXRenderPassEncoder*>(renderPassEncoder);
  self->EndOcclusionQuery();
}

GFX_EXPORT void GFX_FUNCTION(RenderPassEncoderExecuteBundles)(
    WGPURenderPassEncoder renderPassEncoder,
    size_t bundleCount,
    WGPURenderBundle const* bundles) {
  auto* self = static_cast<vkgfx::GFXRenderPassEncoder*>(renderPassEncoder);
  self->ExecuteBundles(bundleCount, bundles);
}

GFX_EXPORT void GFX_FUNCTION(RenderPassEncoderInsertDebugMarker)(
    WGPURenderPassEncoder renderPassEncoder,
    WGPUStringView markerLabel) {
  auto* self = static_cast<vkgfx::GFXRenderPassEncoder*>(renderPassEncoder);
  self->InsertDebugMarker(markerLabel);
}

GFX_EXPORT void GFX_FUNCTION(RenderPassEncoderPopDebugGroup)(
    WGPURenderPassEncoder renderPassEncoder) {
  auto* self = static_cast<vkgfx::GFXRenderPassEncoder*>(renderPassEncoder);
  self->PopDebugGroup();
}

GFX_EXPORT void GFX_FUNCTION(RenderPassEncoderPushDebugGroup)(
    WGPURenderPassEncoder renderPassEncoder,
    WGPUStringView groupLabel) {
  auto* self = static_cast<vkgfx::GFXRenderPassEncoder*>(renderPassEncoder);
  self->PushDebugGroup(groupLabel);
}

GFX_EXPORT void GFX_FUNCTION(RenderPassEncoderSetBindGroup)(
    WGPURenderPassEncoder renderPassEncoder,
    uint32_t groupIndex,
    WGPU_NULLABLE WGPUBindGroup group,
    size_t dynamicOffsetCount,
    uint32_t const* dynamicOffsets) {
  auto* self = static_cast<vkgfx::GFXRenderPassEncoder*>(renderPassEncoder);
  self->SetBindGroup(groupIndex, group, dynamicOffsetCount, dynamicOffsets);
}

GFX_EXPORT void GFX_FUNCTION(RenderPassEncoderSetBlendConstant)(
    WGPURenderPassEncoder renderPassEncoder,
    WGPUColor const* color) {
  auto* self = static_cast<vkgfx::GFXRenderPassEncoder*>(renderPassEncoder);
  self->SetBlendConstant(color);
}

GFX_EXPORT void GFX_FUNCTION(RenderPassEncoderSetIndexBuffer)(
    WGPURenderPassEncoder renderPassEncoder,
    WGPUBuffer buffer,
    WGPUIndexFormat format,
    uint64_t offset,
    uint64_t size) {
  auto* self = static_cast<vkgfx::GFXRenderPassEncoder*>(renderPassEncoder);
  self->SetIndexBuffer(buffer, format, offset, size);
}

GFX_EXPORT void GFX_FUNCTION(RenderPassEncoderSetLabel)(
    WGPURenderPassEncoder renderPassEncoder,
    WGPUStringView label) {
  auto* self = static_cast<vkgfx::GFXRenderPassEncoder*>(renderPassEncoder);
  self->SetLabel(label);
}

GFX_EXPORT void GFX_FUNCTION(RenderPassEncoderSetPipeline)(
    WGPURenderPassEncoder renderPassEncoder,
    WGPURenderPipeline pipeline) {
  auto* self = static_cast<vkgfx::GFXRenderPassEncoder*>(renderPassEncoder);
  self->SetPipeline(pipeline);
}

GFX_EXPORT void GFX_FUNCTION(RenderPassEncoderSetScissorRect)(
    WGPURenderPassEncoder renderPassEncoder,
    uint32_t x,
    uint32_t y,
    uint32_t width,
    uint32_t height) {
  auto* self = static_cast<vkgfx::GFXRenderPassEncoder*>(renderPassEncoder);
  self->SetScissorRect(x, y, width, height);
}

GFX_EXPORT void GFX_FUNCTION(RenderPassEncoderSetStencilReference)(
    WGPURenderPassEncoder renderPassEncoder,
    uint32_t reference) {
  auto* self = static_cast<vkgfx::GFXRenderPassEncoder*>(renderPassEncoder);
  self->SetStencilReference(reference);
}

GFX_EXPORT void GFX_FUNCTION(RenderPassEncoderSetVertexBuffer)(
    WGPURenderPassEncoder renderPassEncoder,
    uint32_t slot,
    WGPU_NULLABLE WGPUBuffer buffer,
    uint64_t offset,
    uint64_t size) {
  auto* self = static_cast<vkgfx::GFXRenderPassEncoder*>(renderPassEncoder);
  self->SetVertexBuffer(slot, buffer, offset, size);
}

GFX_EXPORT void GFX_FUNCTION(RenderPassEncoderSetViewport)(
    WGPURenderPassEncoder renderPassEncoder,
    float x,
    float y,
    float width,
    float height,
    float minDepth,
    float maxDepth) {
  auto* self = static_cast<vkgfx::GFXRenderPassEncoder*>(renderPassEncoder);
  self->SetViewport(x, y, width, height, minDepth, maxDepth);
}
