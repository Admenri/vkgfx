// Copyright 2025 Admenri.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#include "gfx/gfx_command_encoder.h"

namespace vkgfx {

///////////////////////////////////////////////////////////////////////////////
// GFXCommandEncoder Implement

GFXCommandEncoder::GFXCommandEncoder() {}

GFXCommandEncoder::~GFXCommandEncoder() {}

WGPUComputePassEncoder GFXCommandEncoder::BeginComputePass(
    WGPUComputePassDescriptor const* descriptor) {
  return WGPUComputePassEncoder();
}

WGPURenderPassEncoder GFXCommandEncoder::BeginRenderPass(
    WGPURenderPassDescriptor const* descriptor) {
  return WGPURenderPassEncoder();
}

void GFXCommandEncoder::ClearBuffer(WGPUBuffer buffer,
                                    uint64_t offset,
                                    uint64_t size) {}

void GFXCommandEncoder::CopyBufferToBuffer(WGPUBuffer source,
                                           uint64_t sourceOffset,
                                           WGPUBuffer destination,
                                           uint64_t destinationOffset,
                                           uint64_t size) {}

void GFXCommandEncoder::CopyBufferToTexture(
    WGPUTexelCopyBufferInfo const* source,
    WGPUTexelCopyTextureInfo const* destination,
    WGPUExtent3D const* copySize) {}

void GFXCommandEncoder::CopyTextureToBuffer(
    WGPUTexelCopyTextureInfo const* source,
    WGPUTexelCopyBufferInfo const* destination,
    WGPUExtent3D const* copySize) {}

void GFXCommandEncoder::CopyTextureToTexture(
    WGPUTexelCopyTextureInfo const* source,
    WGPUTexelCopyTextureInfo const* destination,
    WGPUExtent3D const* copySize) {}

WGPUCommandBuffer GFXCommandEncoder::Finish(
    WGPUCommandBufferDescriptor const* descriptor) {
  return WGPUCommandBuffer();
}

void GFXCommandEncoder::InsertDebugMarker(WGPUStringView markerLabel) {}

void GFXCommandEncoder::PopDebugGroup() {}

void GFXCommandEncoder::PushDebugGroup(WGPUStringView groupLabel) {}

void GFXCommandEncoder::ResolveQuerySet(WGPUQuerySet querySet,
                                        uint32_t firstQuery,
                                        uint32_t queryCount,
                                        WGPUBuffer destination,
                                        uint64_t destinationOffset) {}

void GFXCommandEncoder::SetLabel(WGPUStringView label) {
  label_ = std::string(label.data, label.length);
}

void GFXCommandEncoder::WriteTimestamp(WGPUQuerySet querySet,
                                       uint32_t queryIndex) {}

}  // namespace vkgfx

///////////////////////////////////////////////////////////////////////////////
// Member Function

GFX_REFCOUNTED_EXPORT(CommandEncoder, vkgfx::GFXCommandEncoder);

GFX_EXPORT WGPUComputePassEncoder GFX_FUNCTION(CommandEncoderBeginComputePass)(
    WGPUCommandEncoder commandEncoder,
    WGPU_NULLABLE WGPUComputePassDescriptor const* descriptor) {
  auto* self = static_cast<vkgfx::GFXCommandEncoder*>(commandEncoder);
  return self->BeginComputePass(descriptor);
}

GFX_EXPORT WGPURenderPassEncoder GFX_FUNCTION(CommandEncoderBeginRenderPass)(
    WGPUCommandEncoder commandEncoder,
    WGPURenderPassDescriptor const* descriptor) {
  auto* self = static_cast<vkgfx::GFXCommandEncoder*>(commandEncoder);
  return self->BeginRenderPass(descriptor);
}

GFX_EXPORT void GFX_FUNCTION(CommandEncoderClearBuffer)(
    WGPUCommandEncoder commandEncoder,
    WGPUBuffer buffer,
    uint64_t offset,
    uint64_t size) {
  auto* self = static_cast<vkgfx::GFXCommandEncoder*>(commandEncoder);
  self->ClearBuffer(buffer, offset, size);
}

GFX_EXPORT void GFX_FUNCTION(CommandEncoderCopyBufferToBuffer)(
    WGPUCommandEncoder commandEncoder,
    WGPUBuffer source,
    uint64_t sourceOffset,
    WGPUBuffer destination,
    uint64_t destinationOffset,
    uint64_t size) {
  auto* self = static_cast<vkgfx::GFXCommandEncoder*>(commandEncoder);
  self->CopyBufferToBuffer(source, sourceOffset, destination, destinationOffset,
                           size);
}

GFX_EXPORT void GFX_FUNCTION(CommandEncoderCopyBufferToTexture)(
    WGPUCommandEncoder commandEncoder,
    WGPUTexelCopyBufferInfo const* source,
    WGPUTexelCopyTextureInfo const* destination,
    WGPUExtent3D const* copySize) {
  auto* self = static_cast<vkgfx::GFXCommandEncoder*>(commandEncoder);
  self->CopyBufferToTexture(source, destination, copySize);
}

GFX_EXPORT void GFX_FUNCTION(CommandEncoderCopyTextureToBuffer)(
    WGPUCommandEncoder commandEncoder,
    WGPUTexelCopyTextureInfo const* source,
    WGPUTexelCopyBufferInfo const* destination,
    WGPUExtent3D const* copySize) {
  auto* self = static_cast<vkgfx::GFXCommandEncoder*>(commandEncoder);
  self->CopyTextureToBuffer(source, destination, copySize);
}

GFX_EXPORT void GFX_FUNCTION(CommandEncoderCopyTextureToTexture)(
    WGPUCommandEncoder commandEncoder,
    WGPUTexelCopyTextureInfo const* source,
    WGPUTexelCopyTextureInfo const* destination,
    WGPUExtent3D const* copySize) {
  auto* self = static_cast<vkgfx::GFXCommandEncoder*>(commandEncoder);
  self->CopyTextureToTexture(source, destination, copySize);
}

GFX_EXPORT WGPUCommandBuffer GFX_FUNCTION(CommandEncoderFinish)(
    WGPUCommandEncoder commandEncoder,
    WGPU_NULLABLE WGPUCommandBufferDescriptor const* descriptor) {
  auto* self = static_cast<vkgfx::GFXCommandEncoder*>(commandEncoder);
  return self->Finish(descriptor);
}

GFX_EXPORT void GFX_FUNCTION(CommandEncoderInsertDebugMarker)(
    WGPUCommandEncoder commandEncoder,
    WGPUStringView markerLabel) {
  auto* self = static_cast<vkgfx::GFXCommandEncoder*>(commandEncoder);
  self->InsertDebugMarker(markerLabel);
}

GFX_EXPORT void GFX_FUNCTION(CommandEncoderPopDebugGroup)(
    WGPUCommandEncoder commandEncoder) {
  auto* self = static_cast<vkgfx::GFXCommandEncoder*>(commandEncoder);
  self->PopDebugGroup();
}

GFX_EXPORT void GFX_FUNCTION(CommandEncoderPushDebugGroup)(
    WGPUCommandEncoder commandEncoder,
    WGPUStringView groupLabel) {
  auto* self = static_cast<vkgfx::GFXCommandEncoder*>(commandEncoder);
  self->PushDebugGroup(groupLabel);
}

GFX_EXPORT void GFX_FUNCTION(CommandEncoderResolveQuerySet)(
    WGPUCommandEncoder commandEncoder,
    WGPUQuerySet querySet,
    uint32_t firstQuery,
    uint32_t queryCount,
    WGPUBuffer destination,
    uint64_t destinationOffset) {
  auto* self = static_cast<vkgfx::GFXCommandEncoder*>(commandEncoder);
  self->ResolveQuerySet(querySet, firstQuery, queryCount, destination,
                        destinationOffset);
}

GFX_EXPORT void GFX_FUNCTION(CommandEncoderSetLabel)(
    WGPUCommandEncoder commandEncoder,
    WGPUStringView label) {
  auto* self = static_cast<vkgfx::GFXCommandEncoder*>(commandEncoder);
  self->SetLabel(label);
}

GFX_EXPORT void GFX_FUNCTION(CommandEncoderWriteTimestamp)(
    WGPUCommandEncoder commandEncoder,
    WGPUQuerySet querySet,
    uint32_t queryIndex) {
  auto* self = static_cast<vkgfx::GFXCommandEncoder*>(commandEncoder);
  self->WriteTimestamp(querySet, queryIndex);
}
