// Copyright 2025 Admenri.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#include "gfx/gfx_queue.h"

namespace vkgfx {

///////////////////////////////////////////////////////////////////////////////
// GFXQueue Implement

GFXQueue::GFXQueue() {}

GFXQueue::~GFXQueue() {}

WGPUFuture GFXQueue::OnSubmittedWorkDone(
    WGPUQueueWorkDoneCallbackInfo callbackInfo) {
  return WGPUFuture();
}

void GFXQueue::SetLabel(WGPUStringView label) {
  label_ = std::string(label.data, label.length);
}

void GFXQueue::Submit(size_t commandCount, WGPUCommandBuffer const* commands) {}

void GFXQueue::WriteBuffer(WGPUBuffer buffer,
                           uint64_t bufferOffset,
                           void const* data,
                           size_t size) {}

void GFXQueue::WriteTexture(WGPUTexelCopyTextureInfo const* destination,
                            void const* data,
                            size_t dataSize,
                            WGPUTexelCopyBufferLayout const* dataLayout,
                            WGPUExtent3D const* writeSize) {}

}  // namespace vkgfx

///////////////////////////////////////////////////////////////////////////////
// Member Function

GFX_REFCOUNTED_EXPORT(Queue, vkgfx::GFXQueue);

GFX_EXPORT WGPUFuture GFX_FUNCTION(QueueOnSubmittedWorkDone)(
    WGPUQueue queue,
    WGPUQueueWorkDoneCallbackInfo callbackInfo) {
  auto* self = static_cast<vkgfx::GFXQueue*>(queue);
  return self->OnSubmittedWorkDone(callbackInfo);
}

GFX_EXPORT void GFX_FUNCTION(QueueSetLabel)(WGPUQueue queue,
                                            WGPUStringView label) {
  auto* self = static_cast<vkgfx::GFXQueue*>(queue);
  self->SetLabel(label);
}

GFX_EXPORT void GFX_FUNCTION(QueueSubmit)(WGPUQueue queue,
                                          size_t commandCount,
                                          WGPUCommandBuffer const* commands) {
  auto* self = static_cast<vkgfx::GFXQueue*>(queue);
  self->Submit(commandCount, commands);
}

GFX_EXPORT void GFX_FUNCTION(QueueWriteBuffer)(WGPUQueue queue,
                                               WGPUBuffer buffer,
                                               uint64_t bufferOffset,
                                               void const* data,
                                               size_t size) {
  auto* self = static_cast<vkgfx::GFXQueue*>(queue);
  self->WriteBuffer(buffer, bufferOffset, data, size);
}

GFX_EXPORT void GFX_FUNCTION(QueueWriteTexture)(
    WGPUQueue queue,
    WGPUTexelCopyTextureInfo const* destination,
    void const* data,
    size_t dataSize,
    WGPUTexelCopyBufferLayout const* dataLayout,
    WGPUExtent3D const* writeSize) {
  auto* self = static_cast<vkgfx::GFXQueue*>(queue);
  self->WriteTexture(destination, data, dataSize, dataLayout, writeSize);
}
