// Copyright 2025 Admenri.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#include "gfx/gfx_sampler.h"

#include "gfx/gfx_device.h"

namespace vkgfx {

///////////////////////////////////////////////////////////////////////////////
// GFXSampler Implement

GFXSampler::GFXSampler(VkSampler sampler,
                       RefPtr<GFXDevice> device,
                       const std::string& label)
    : sampler_(sampler), device_(device), label_(label) {}

GFXSampler::~GFXSampler() {
  if (sampler_)
    vkDestroySampler(device_->GetVkHandle(), sampler_, nullptr);
}

void GFXSampler::SetLabel(WGPUStringView label) {
  label_ = std::string(label.data, label.length);
}

}  // namespace vkgfx

///////////////////////////////////////////////////////////////////////////////
// Member Function

GFX_REFCOUNTED_EXPORT(Sampler, vkgfx::GFXSampler);

GFX_EXPORT void GFX_FUNCTION(SamplerSetLabel)(WGPUSampler sampler,
                                              WGPUStringView label) {
  auto* self = static_cast<vkgfx::GFXSampler*>(sampler);
  self->SetLabel(label);
}
