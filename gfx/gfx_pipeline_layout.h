// Copyright 2025 Admenri.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#ifndef GFX_GFX_PIPELINE_LAYOUT_H_
#define GFX_GFX_PIPELINE_LAYOUT_H_

#include "gfx/common/refptr.h"
#include "gfx/gfx_config.h"
#include "gfx/gfx_device.h"

struct WGPUPipelineLayoutImpl {};

namespace vkgfx {

// https://gpuweb.github.io/gpuweb/#gpupipelinelayout
class GFXPipelineLayout : public RefCounted<GFXPipelineLayout>,
                          public WGPUPipelineLayoutImpl {
 public:
  GFXPipelineLayout();
  ~GFXPipelineLayout();

  GFXPipelineLayout(const GFXPipelineLayout&) = delete;
  GFXPipelineLayout& operator=(const GFXPipelineLayout&) = delete;

  VkPipelineLayout GetVkHandle() const { return layout_; }

  void SetLabel(WGPUStringView label);

 private:
  VkPipelineLayout layout_;

  RefPtr<GFXDevice> device_;

  std::string label_;
};

}  // namespace vkgfx

#endif  // GFX_GFX_PIPELINE_LAYOUT_H_
