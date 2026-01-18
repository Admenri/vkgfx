// Copyright 2025 Admenri.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#ifndef GFX_GFX_TEXTURE_H_
#define GFX_GFX_TEXTURE_H_

#include "gfx/common/refptr.h"
#include "gfx/gfx_config.h"
#include "gfx/gfx_device.h"

#include "vma/vma.h"

struct WGPUTextureImpl {};

namespace vkgfx {

// https://gpuweb.github.io/gpuweb/#gputexture
class GFXTexture : public RefCounted<GFXTexture>, public WGPUTextureImpl {
 public:
  GFXTexture(VkImage image,
             VmaAllocation allocation,
             RefPtr<GFXDevice> device,
             WGPUStringView label);
  ~GFXTexture();

  GFXTexture(const GFXTexture&) = delete;
  GFXTexture& operator=(const GFXTexture&) = delete;

  WGPUTextureView CreateView(WGPUTextureViewDescriptor const* descriptor);
  void Destroy();
  uint32_t GetDepthOrArrayLayers();
  WGPUTextureDimension GetDimension();
  WGPUTextureFormat GetFormat();
  uint32_t GetHeight();
  uint32_t GetMipLevelCount();
  uint32_t GetSampleCount();
  WGPUTextureUsage GetUsage();
  uint32_t GetWidth();
  void SetLabel(WGPUStringView label);

 private:
  VkImage image_;
  VmaAllocation allocation_;

  RefPtr<GFXDevice> device_;

  std::string label_ = "GFX.Texture";
};

}  // namespace vkgfx

#endif  // GFX_GFX_TEXTURE_H_
