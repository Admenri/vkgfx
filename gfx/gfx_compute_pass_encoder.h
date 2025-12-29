// Copyright 2025 Admenri.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#ifndef GFX_GFX_COMPUTE_PASS_ENCODER_H_
#define GFX_GFX_COMPUTE_PASS_ENCODER_H_

#include "gfx/common/refptr.h"
#include "gfx/gfx_config.h"
#include "gfx/gfx_device.h"

struct WGPUComputePassEncoderImpl {};

namespace vkgfx {

// https://gpuweb.github.io/gpuweb/#gpucomputepassencoder
class GFXComputePassEncoder : public RefCounted<GFXComputePassEncoder>,
                              public WGPUComputePassEncoderImpl {
 public:
  GFXComputePassEncoder();
  ~GFXComputePassEncoder();

  GFXComputePassEncoder(const GFXComputePassEncoder&) = delete;
  GFXComputePassEncoder& operator=(const GFXComputePassEncoder&) = delete;

  void DispatchWorkgroups(uint32_t workgroupCountX,
                          uint32_t workgroupCountY,
                          uint32_t workgroupCountZ);
  void DispatchWorkgroupsIndirect(WGPUBuffer indirectBuffer,
                                  uint64_t indirectOffset);
  void End();
  void InsertDebugMarker(WGPUStringView markerLabel);
  void PopDebugGroup();
  void PushDebugGroup(WGPUStringView groupLabel);
  void SetBindGroup(uint32_t groupIndex,
                    WGPU_NULLABLE WGPUBindGroup group,
                    size_t dynamicOffsetCount,
                    uint32_t const* dynamicOffsets);
  void SetLabel(WGPUStringView label);
  void SetPipeline(WGPUComputePipeline pipeline);

 private:
  std::string label_;
};

}  // namespace vkgfx

#endif  // GFX_GFX_COMPUTE_PASS_ENCODER_H_
