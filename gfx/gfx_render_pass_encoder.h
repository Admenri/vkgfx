// Copyright 2025 Admenri.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#ifndef GFX_GFX_RENDER_PASS_ENCODER_H_
#define GFX_GFX_RENDER_PASS_ENCODER_H_

#include "gfx/common/refptr.h"
#include "gfx/gfx_config.h"

struct WGPURenderPassEncoderImpl {};

namespace vkgfx {

// https://gpuweb.github.io/gpuweb/#gpurenderpassencoder
class GFXRenderPassEncoder : public RefCounted<GFXRenderPassEncoder>,
                             public WGPURenderPassEncoderImpl {
 public:
  GFXRenderPassEncoder();
  ~GFXRenderPassEncoder();

  GFXRenderPassEncoder(const GFXRenderPassEncoder&) = delete;
  GFXRenderPassEncoder& operator=(const GFXRenderPassEncoder&) = delete;

  void BeginOcclusionQuery(uint32_t queryIndex);
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
  void End();
  void EndOcclusionQuery();
  void ExecuteBundles(size_t bundleCount, WGPURenderBundle const* bundles);
  void InsertDebugMarker(WGPUStringView markerLabel);
  void PopDebugGroup();
  void PushDebugGroup(WGPUStringView groupLabel);
  void SetBindGroup(uint32_t groupIndex,
                    WGPUBindGroup group,
                    size_t dynamicOffsetCount,
                    uint32_t const* dynamicOffsets);
  void SetBlendConstant(WGPUColor const* color);
  void SetIndexBuffer(WGPUBuffer buffer,
                      WGPUIndexFormat format,
                      uint64_t offset,
                      uint64_t size);
  void SetLabel(WGPUStringView label);
  void SetPipeline(WGPURenderPipeline pipeline);
  void SetScissorRect(uint32_t x, uint32_t y, uint32_t width, uint32_t height);
  void SetStencilReference(uint32_t reference);
  void SetVertexBuffer(uint32_t slot,
                       WGPUBuffer buffer,
                       uint64_t offset,
                       uint64_t size);
  void SetViewport(float x,
                   float y,
                   float width,
                   float height,
                   float minDepth,
                   float maxDepth);

 private:
  std::string label_;
};

}  // namespace vkgfx

#endif  // GFX_GFX_RENDER_PASS_ENCODER_H_
