// Copyright 2025 Admenri.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#ifndef GFX_GFX_RENDER_BUNDLE_ENCODER_H_
#define GFX_GFX_RENDER_BUNDLE_ENCODER_H_

#include "gfx/common/refptr.h"
#include "gfx/gfx_config.h"

struct WGPURenderBundleEncoderImpl {};

namespace vkgfx {

// https://gpuweb.github.io/gpuweb/#gpurenderbundleencoder
class GFXRenderBundleEncoder : public RefCounted<GFXRenderBundleEncoder>,
                               public WGPURenderBundleEncoderImpl {
 public:
  GFXRenderBundleEncoder();
  ~GFXRenderBundleEncoder();

  GFXRenderBundleEncoder(const GFXRenderBundleEncoder&) = delete;
  GFXRenderBundleEncoder& operator=(const GFXRenderBundleEncoder&) = delete;

  void Draw(uint32_t vertexCount,
            uint32_t instanceCount,
            uint32_t firstVertex,
            uint32_t firstInstance);
  void DrawIndexed(uint32_t indexCount,
                   uint32_t instanceCount,
                   uint32_t firstIndex,
                   int32_t baseVertex,
                   uint32_t firstInstance);
  void DrawIndexedIndirect(WGPUBuffer indirectBuffer, uint64_t indirectOffset);
  void DrawIndirect(WGPUBuffer indirectBuffer, uint64_t indirectOffset);
  WGPURenderBundle Finish(WGPURenderBundleDescriptor const* descriptor);
  void InsertDebugMarker(WGPUStringView markerLabel);
  void PopDebugGroup();
  void PushDebugGroup(WGPUStringView groupLabel);
  void SetBindGroup(uint32_t groupIndex,
                    WGPUBindGroup group,
                    size_t dynamicOffsetCount,
                    uint32_t const* dynamicOffsets);
  void SetIndexBuffer(WGPUBuffer buffer,
                      WGPUIndexFormat format,
                      uint64_t offset,
                      uint64_t size);
  void SetLabel(WGPUStringView label);
  void SetPipeline(WGPURenderPipeline pipeline);
  void SetVertexBuffer(uint32_t slot,
                       WGPUBuffer buffer,
                       uint64_t offset,
                       uint64_t size);

 private:
  std::string label_;
};

}  // namespace vkgfx

#endif  // GFX_GFX_RENDER_BUNDLE_ENCODER_H_