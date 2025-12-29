// Copyright 2025 Admenri.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#ifndef GFX_GFX_QUEUE_H_
#define GFX_GFX_QUEUE_H_

#include "gfx/common/refptr.h"
#include "gfx/gfx_config.h"
#include "gfx/gfx_device.h"

struct WGPUQueueImpl {};

namespace vkgfx {

// https://gpuweb.github.io/gpuweb/#gpuqueue
class GFXQueue : public RefCounted<GFXQueue>, public WGPUQueueImpl {
 public:
  GFXQueue();
  ~GFXQueue();

  GFXQueue(const GFXQueue&) = delete;
  GFXQueue& operator=(const GFXQueue&) = delete;

  VkQueue GetVkHandle() const { return queue_; }

  WGPUFuture OnSubmittedWorkDone(WGPUQueueWorkDoneCallbackInfo callbackInfo);
  void SetLabel(WGPUStringView label);
  void Submit(size_t commandCount, WGPUCommandBuffer const* commands);
  void WriteBuffer(WGPUBuffer buffer,
                   uint64_t bufferOffset,
                   void const* data,
                   size_t size);
  void WriteTexture(WGPUTexelCopyTextureInfo const* destination,
                    void const* data,
                    size_t dataSize,
                    WGPUTexelCopyBufferLayout const* dataLayout,
                    WGPUExtent3D const* writeSize);

 private:
  VkQueue queue_;

  RefPtr<GFXDevice> device_;

  std::string label_;
};

}  // namespace vkgfx

#endif  // GFX_GFX_QUEUE_H_
