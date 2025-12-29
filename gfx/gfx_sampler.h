// Copyright 2025 Admenri.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#ifndef GFX_GFX_SAMPLER_H_
#define GFX_GFX_SAMPLER_H_

#include "gfx/common/refptr.h"
#include "gfx/gfx_config.h"
#include "gfx/gfx_device.h"

struct WGPUSamplerImpl {};

namespace vkgfx {

// https://gpuweb.github.io/gpuweb/#gpusampler
class GFXSampler : public RefCounted<GFXSampler>, public WGPUSamplerImpl {
 public:
  GFXSampler(VkSampler sampler,
             RefPtr<GFXDevice> device,
             const std::string& label);
  ~GFXSampler();

  GFXSampler(const GFXSampler&) = delete;
  GFXSampler& operator=(const GFXSampler&) = delete;

  void SetLabel(WGPUStringView label);

 private:
  VkSampler sampler_;

  RefPtr<GFXDevice> device_;

  std::string label_;
};

}  // namespace vkgfx

#endif
