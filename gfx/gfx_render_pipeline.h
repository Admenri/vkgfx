// Copyright 2025 Admenri.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#ifndef GFX_GFX_PIPELINE_H_
#define GFX_GFX_PIPELINE_H_

#include "gfx/common/refptr.h"
#include "gfx/gfx_config.h"

struct WGPURenderPipelineImpl {};

namespace vkgfx {

// https://gpuweb.github.io/gpuweb/#gpurenderpipeline
class GFXRenderPipeline : public RefCounted<GFXRenderPipeline>,
                          public WGPURenderPipelineImpl {
 public:
  GFXRenderPipeline();
  ~GFXRenderPipeline();

  GFXRenderPipeline(const GFXRenderPipeline&) = delete;
  GFXRenderPipeline& operator=(const GFXRenderPipeline&) = delete;

  WGPUBindGroupLayout GetBindGroupLayout(uint32_t groupIndex);
  void SetLabel(WGPUStringView label);

 private:
  std::string label_;
};

}  // namespace vkgfx

#endif  // GFX_GFX_PIPELINE_H_
