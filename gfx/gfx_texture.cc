// Copyright 2025 Admenri.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#include "gfx/gfx_texture.h"

namespace vkgfx {

///////////////////////////////////////////////////////////////////////////////
// GFXTexture Implement

GFXTexture::GFXTexture() {}

GFXTexture::~GFXTexture() {}

WGPUTextureView GFXTexture::CreateView(
    WGPUTextureViewDescriptor const* descriptor) {
  return WGPUTextureView();
}

void GFXTexture::Destroy() {}

uint32_t GFXTexture::GetDepthOrArrayLayers() {
  return 0;
}

WGPUTextureDimension GFXTexture::GetDimension() {
  return WGPUTextureDimension();
}

WGPUTextureFormat GFXTexture::GetFormat() {
  return WGPUTextureFormat();
}

uint32_t GFXTexture::GetHeight() {
  return 0;
}

uint32_t GFXTexture::GetMipLevelCount() {
  return 0;
}

uint32_t GFXTexture::GetSampleCount() {
  return 0;
}

WGPUTextureUsage GFXTexture::GetUsage() {
  return WGPUTextureUsage();
}

uint32_t GFXTexture::GetWidth() {
  return 0;
}

void GFXTexture::SetLabel(WGPUStringView label) {
  label_ = std::string(label.data, label.length);
}

}  // namespace vkgfx

///////////////////////////////////////////////////////////////////////////////
// Member Function

GFX_REFCOUNTED_EXPORT(Texture, vkgfx::GFXTexture);

GFX_EXPORT WGPUTextureView GFX_FUNCTION(TextureCreateView)(
    WGPUTexture texture,
    WGPU_NULLABLE WGPUTextureViewDescriptor const* descriptor) {
  auto* self = static_cast<vkgfx::GFXTexture*>(texture);
  return self->CreateView(descriptor);
}

GFX_EXPORT void GFX_FUNCTION(TextureDestroy)(WGPUTexture texture) {
  auto* self = static_cast<vkgfx::GFXTexture*>(texture);
  self->Destroy();
}

GFX_EXPORT uint32_t
GFX_FUNCTION(TextureGetDepthOrArrayLayers)(WGPUTexture texture) {
  auto* self = static_cast<vkgfx::GFXTexture*>(texture);
  return self->GetDepthOrArrayLayers();
}

GFX_EXPORT WGPUTextureDimension
GFX_FUNCTION(TextureGetDimension)(WGPUTexture texture) {
  auto* self = static_cast<vkgfx::GFXTexture*>(texture);
  return self->GetDimension();
}

GFX_EXPORT WGPUTextureFormat
GFX_FUNCTION(TextureGetFormat)(WGPUTexture texture) {
  auto* self = static_cast<vkgfx::GFXTexture*>(texture);
  return self->GetFormat();
}

GFX_EXPORT uint32_t GFX_FUNCTION(TextureGetHeight)(WGPUTexture texture) {
  auto* self = static_cast<vkgfx::GFXTexture*>(texture);
  return self->GetHeight();
}

GFX_EXPORT uint32_t GFX_FUNCTION(TextureGetMipLevelCount)(WGPUTexture texture) {
  auto* self = static_cast<vkgfx::GFXTexture*>(texture);
  return self->GetMipLevelCount();
}

GFX_EXPORT uint32_t GFX_FUNCTION(TextureGetSampleCount)(WGPUTexture texture) {
  auto* self = static_cast<vkgfx::GFXTexture*>(texture);
  return self->GetSampleCount();
}

GFX_EXPORT WGPUTextureUsage GFX_FUNCTION(TextureGetUsage)(WGPUTexture texture) {
  auto* self = static_cast<vkgfx::GFXTexture*>(texture);
  return self->GetUsage();
}

GFX_EXPORT uint32_t GFX_FUNCTION(TextureGetWidth)(WGPUTexture texture) {
  auto* self = static_cast<vkgfx::GFXTexture*>(texture);
  return self->GetWidth();
}

GFX_EXPORT void GFX_FUNCTION(TextureSetLabel)(WGPUTexture texture,
                                              WGPUStringView label) {
  auto* self = static_cast<vkgfx::GFXTexture*>(texture);
  self->SetLabel(label);
}
