// Copyright 2025 Admenri.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#ifndef GFX_GFX_COMPUTE_PIPELINE_H_
#define GFX_GFX_COMPUTE_PIPELINE_H_

#include "gfx/common/refptr.h"
#include "gfx/gfx_config.h"
#include "gfx/gfx_device.h"

struct WGPUComputePipelineImpl {};

namespace vkgfx {

// https://gpuweb.github.io/gpuweb/#gpucomputepipeline
class GFXComputePipeline : public RefCounted<GFXComputePipeline>,
                           public WGPUComputePipelineImpl {
 public:
  GFXComputePipeline();
  ~GFXComputePipeline();

  GFXComputePipeline(const GFXComputePipeline&) = delete;
  GFXComputePipeline& operator=(const GFXComputePipeline&) = delete;

  VkPipeline GetVkPipeline() const { return pipeline_; }

  WGPUBindGroupLayout GetBindGroupLayout(uint32_t groupIndex);
  void SetLabel(WGPUStringView label);

 private:
  VkPipeline pipeline_;

  RefPtr<GFXDevice> device_;

  std::string label_;
};

}  // namespace vkgfx

#endif  // GFX_GFX_COMPUTE_PIPELINE_H_
