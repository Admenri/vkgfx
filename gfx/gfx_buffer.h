// Copyright 2025 Admenri.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#ifndef GFX_GFX_BUFFER_H_
#define GFX_GFX_BUFFER_H_

#include "gfx/common/refptr.h"
#include "gfx/gfx_config.h"
#include "gfx/gfx_device.h"

struct WGPUBufferImpl {};

namespace vkgfx {

// https://gpuweb.github.io/gpuweb/#gpubuffer
class GFXBuffer : public RefCounted<GFXBuffer>, public WGPUBufferImpl {
 public:
  GFXBuffer(VkBuffer buffer,
            RefPtr<GFXDevice> device,
            const std::string& label);
  ~GFXBuffer();

  GFXBuffer(const GFXBuffer&) = delete;
  GFXBuffer& operator=(const GFXBuffer&) = delete;

  VkBuffer GetVkHandle() const { return buffer_; }

  void Destroy();
  void const* GetConstMappedRange(size_t offset, size_t size);
  void* GetMappedRange(size_t offset, size_t size);
  WGPUBufferMapState GetMapState();
  uint64_t GetSize();
  WGPUBufferUsage GetUsage();
  WGPUFuture MapAsync(WGPUMapMode mode,
                      size_t offset,
                      size_t size,
                      WGPUBufferMapCallbackInfo callbackInfo);
  WGPUStatus ReadMappedRange(size_t offset, void* data, size_t size);
  void SetLabel(WGPUStringView label);
  void Unmap();
  WGPUStatus WriteMappedRange(size_t offset, void const* data, size_t size);

 private:
  VkBuffer buffer_;

  RefPtr<GFXDevice> device_;

  std::string label_;
};

}  // namespace vkgfx

#endif  // GFX_GFX_BUFFER_H_
