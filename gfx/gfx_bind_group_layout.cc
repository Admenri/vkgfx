// Copyright 2025 Admenri.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#include "gfx/gfx_bind_group_layout.h"

#include "gfx/gfx_device.h"

namespace vkgfx {

///////////////////////////////////////////////////////////////////////////////
// GFXBindGroupLayout Implement

GFXBindGroupLayout::GFXBindGroupLayout(VkDescriptorSetLayout layout,
                                       RefPtr<GFXDevice> device,
                                       const std::string& label)
    : layout_(layout), device_(device), label_(label) {}

GFXBindGroupLayout::~GFXBindGroupLayout() {
  if (layout_)
    vkDestroyDescriptorSetLayout(device_->GetVkHandle(), layout_, nullptr);
}

void GFXBindGroupLayout::SetLabel(WGPUStringView label) {
  label_ = std::string(label.data, label.length);
}

}  // namespace vkgfx

///////////////////////////////////////////////////////////////////////////////
// Member Function

GFX_REFCOUNTED_EXPORT(BindGroupLayout, vkgfx::GFXBindGroupLayout);

GFX_EXPORT void GFX_FUNCTION(BindGroupLayoutSetLabel)(
    WGPUBindGroupLayout bindGroupLayout,
    WGPUStringView label) {
  auto* self = static_cast<vkgfx::GFXBindGroupLayout*>(bindGroupLayout);
  self->SetLabel(label);
}
