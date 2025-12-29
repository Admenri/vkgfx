// Copyright 2025 Admenri.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#include "gfx/gfx_buffer.h"

#include "gfx/gfx_device.h"

namespace vkgfx {

///////////////////////////////////////////////////////////////////////////////
// GFXBuffer Implement

GFXBuffer::GFXBuffer(VkBuffer buffer,
                     RefPtr<GFXDevice> device,
                     const std::string& label)
    : buffer_(buffer), device_(device), label_(label) {}

GFXBuffer::~GFXBuffer() {
  Destroy();
}

void GFXBuffer::Destroy() {
  if (buffer_ && device_) {
    vkDestroyBuffer(device_->GetVkHandle(), buffer_, nullptr);
  }
}

void const* GFXBuffer::GetConstMappedRange(size_t offset, size_t size) {
  return nullptr;
}

void* GFXBuffer::GetMappedRange(size_t offset, size_t size) {
  return nullptr;
}

WGPUBufferMapState GFXBuffer::GetMapState() {
  return WGPUBufferMapState();
}

uint64_t GFXBuffer::GetSize() {
  return 0;
}

WGPUBufferUsage GFXBuffer::GetUsage() {
  return WGPUBufferUsage();
}

WGPUFuture GFXBuffer::MapAsync(WGPUMapMode mode,
                               size_t offset,
                               size_t size,
                               WGPUBufferMapCallbackInfo callbackInfo) {
  return WGPUFuture();
}

WGPUStatus GFXBuffer::ReadMappedRange(size_t offset, void* data, size_t size) {
  return WGPUStatus();
}

void GFXBuffer::SetLabel(WGPUStringView label) {}

void GFXBuffer::Unmap() {}

WGPUStatus GFXBuffer::WriteMappedRange(size_t offset,
                                       void const* data,
                                       size_t size) {
  return WGPUStatus();
}

}  // namespace vkgfx

///////////////////////////////////////////////////////////////////////////////
// Member Function

GFX_REFCOUNTED_EXPORT(Buffer, vkgfx::GFXBuffer);

GFX_EXPORT void GFX_FUNCTION(BufferDestroy)(WGPUBuffer buffer) {
  auto* self = static_cast<vkgfx::GFXBuffer*>(buffer);
  self->Destroy();
}

GFX_EXPORT void const* GFX_FUNCTION(
    BufferGetConstMappedRange)(WGPUBuffer buffer, size_t offset, size_t size) {
  auto* self = static_cast<vkgfx::GFXBuffer*>(buffer);
  return self->GetConstMappedRange(offset, size);
}

GFX_EXPORT void* GFX_FUNCTION(BufferGetMappedRange)(WGPUBuffer buffer,
                                                    size_t offset,
                                                    size_t size) {
  auto* self = static_cast<vkgfx::GFXBuffer*>(buffer);
  return self->GetMappedRange(offset, size);
}

GFX_EXPORT WGPUBufferMapState
GFX_FUNCTION(BufferGetMapState)(WGPUBuffer buffer) {
  auto* self = static_cast<vkgfx::GFXBuffer*>(buffer);
  return self->GetMapState();
}

GFX_EXPORT uint64_t GFX_FUNCTION(BufferGetSize)(WGPUBuffer buffer) {
  auto* self = static_cast<vkgfx::GFXBuffer*>(buffer);
  return self->GetSize();
}

GFX_EXPORT WGPUBufferUsage GFX_FUNCTION(BufferGetUsage)(WGPUBuffer buffer) {
  auto* self = static_cast<vkgfx::GFXBuffer*>(buffer);
  return self->GetUsage();
}

GFX_EXPORT WGPUFuture
GFX_FUNCTION(BufferMapAsync)(WGPUBuffer buffer,
                             WGPUMapMode mode,
                             size_t offset,
                             size_t size,
                             WGPUBufferMapCallbackInfo callbackInfo) {
  auto* self = static_cast<vkgfx::GFXBuffer*>(buffer);
  return self->MapAsync(mode, offset, size, callbackInfo);
}

GFX_EXPORT WGPUStatus GFX_FUNCTION(BufferReadMappedRange)(WGPUBuffer buffer,
                                                          size_t offset,
                                                          void* data,
                                                          size_t size) {
  auto* self = static_cast<vkgfx::GFXBuffer*>(buffer);
  return self->ReadMappedRange(offset, data, size);
}

GFX_EXPORT void GFX_FUNCTION(BufferSetLabel)(WGPUBuffer buffer,
                                             WGPUStringView label) {
  auto* self = static_cast<vkgfx::GFXBuffer*>(buffer);
  self->SetLabel(label);
}

GFX_EXPORT void GFX_FUNCTION(BufferUnmap)(WGPUBuffer buffer) {
  auto* self = static_cast<vkgfx::GFXBuffer*>(buffer);
  self->Unmap();
}

GFX_EXPORT WGPUStatus GFX_FUNCTION(BufferWriteMappedRange)(WGPUBuffer buffer,
                                                           size_t offset,
                                                           void const* data,
                                                           size_t size) {
  auto* self = static_cast<vkgfx::GFXBuffer*>(buffer);
  return self->WriteMappedRange(offset, data, size);
}
