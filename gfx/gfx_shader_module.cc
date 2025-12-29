// Copyright 2025 Admenri.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#include "gfx/gfx_shader_module.h"

namespace vkgfx {

///////////////////////////////////////////////////////////////////////////////
// GFXSampler Implement

GFXShaderModule::GFXShaderModule() {}

GFXShaderModule::~GFXShaderModule() {}

WGPUFuture GFXShaderModule::GetCompilationInfo(
    WGPUCompilationInfoCallbackInfo callbackInfo) {
  return WGPUFuture();
}

void GFXShaderModule::SetLabel(WGPUStringView label) {
  label_ = std::string(label.data, label.length);
}

}  // namespace vkgfx

///////////////////////////////////////////////////////////////////////////////
// Member Function

GFX_REFCOUNTED_EXPORT(ShaderModule, vkgfx::GFXShaderModule);

GFX_EXPORT void GFX_FUNCTION(
    ShaderModuleSetLabel)(WGPUShaderModule shaderModule, WGPUStringView label) {
  auto* self = static_cast<vkgfx::GFXShaderModule*>(shaderModule);
  self->SetLabel(label);
}
