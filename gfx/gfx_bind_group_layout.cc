// Copyright 2025 Admenri.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#include "gfx/gfx_bind_group_layout.h"

#include "gfx/gfx_device.h"

namespace vkgfx {

///////////////////////////////////////////////////////////////////////////////
// GFXBindGroupLayout Implement

GFXBindGroupLayout::GFXBindGroupLayout(VkDescriptorSetLayout layout,
                                       const std::vector<LayoutEntry>& entries,
                                       RefPtr<GFXDevice> device,
                                       WGPUStringView label)
    : layout_(layout), entries_(entries), device_(device) {
  if (label.data && label.length)
    label_ = std::string(label.data, label.length);
}

GFXBindGroupLayout::~GFXBindGroupLayout() {
  if (layout_ && device_)
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
