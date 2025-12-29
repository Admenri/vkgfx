// Copyright 2025 Admenri.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#ifndef GFX_GFX_SHADER_MODULE_H_
#define GFX_GFX_SHADER_MODULE_H_

#include "gfx/common/refptr.h"
#include "gfx/gfx_config.h"
#include "gfx/gfx_device.h"

struct WGPUShaderModuleImpl {};

namespace vkgfx {

// https://gpuweb.github.io/gpuweb/#gpushadermodule
class GFXShaderModule : public RefCounted<GFXShaderModule>,
                        public WGPUShaderModuleImpl {
 public:
  GFXShaderModule();
  ~GFXShaderModule();

  GFXShaderModule(const GFXShaderModule&) = delete;
  GFXShaderModule& operator=(const GFXShaderModule&) = delete;

  WGPUFuture GetCompilationInfo(WGPUCompilationInfoCallbackInfo callbackInfo);
  void SetLabel(WGPUStringView label);

 private:
  VkShaderModule shader_;

  RefPtr<GFXDevice> device_;

  std::string label_;
};

}  // namespace vkgfx

#endif  // GFX_GFX_SHADER_MODULE_H_
