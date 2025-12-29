// Copyright 2025 Admenri.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#ifndef GFX_GFX_COMMAND_ENCODER_H_
#define GFX_GFX_COMMAND_ENCODER_H_

#include "gfx/common/refptr.h"
#include "gfx/gfx_config.h"
#include "gfx/gfx_device.h"

struct WGPUCommandEncoderImpl {};

namespace vkgfx {

// https://gpuweb.github.io/gpuweb/#gpucommandencoder
class GFXCommandEncoder : public RefCounted<GFXCommandEncoder>,
                          public WGPUCommandEncoderImpl {
 public:
  GFXCommandEncoder();
  ~GFXCommandEncoder();

  GFXCommandEncoder(const GFXCommandEncoder&) = delete;
  GFXCommandEncoder& operator=(const GFXCommandEncoder&) = delete;

  WGPUComputePassEncoder BeginComputePass(
      WGPUComputePassDescriptor const* descriptor);
  WGPURenderPassEncoder BeginRenderPass(
      WGPURenderPassDescriptor const* descriptor);
  void ClearBuffer(WGPUBuffer buffer, uint64_t offset, uint64_t size);
  void CopyBufferToBuffer(WGPUBuffer source,
                          uint64_t sourceOffset,
                          WGPUBuffer destination,
                          uint64_t destinationOffset,
                          uint64_t size);
  void CopyBufferToTexture(WGPUTexelCopyBufferInfo const* source,
                           WGPUTexelCopyTextureInfo const* destination,
                           WGPUExtent3D const* copySize);
  void CopyTextureToBuffer(WGPUTexelCopyTextureInfo const* source,
                           WGPUTexelCopyBufferInfo const* destination,
                           WGPUExtent3D const* copySize);
  void CopyTextureToTexture(WGPUTexelCopyTextureInfo const* source,
                            WGPUTexelCopyTextureInfo const* destination,
                            WGPUExtent3D const* copySize);
  WGPUCommandBuffer Finish(WGPUCommandBufferDescriptor const* descriptor);
  void InsertDebugMarker(WGPUStringView markerLabel);
  void PopDebugGroup();
  void PushDebugGroup(WGPUStringView groupLabel);
  void ResolveQuerySet(WGPUQuerySet querySet,
                       uint32_t firstQuery,
                       uint32_t queryCount,
                       WGPUBuffer destination,
                       uint64_t destinationOffset);
  void SetLabel(WGPUStringView label);
  void WriteTimestamp(WGPUQuerySet querySet, uint32_t queryIndex);

 private:
  RefPtr<GFXDevice> device_;

  std::string label_;
};

}  // namespace vkgfx

#endif  // GFX_GFX_COMMAND_ENCODER_H_
