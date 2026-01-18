// Copyright 2025 Admenri.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#ifndef GFX_GFX_BIND_GROUP_H_
#define GFX_GFX_BIND_GROUP_H_

#include "gfx/common/refptr.h"
#include "gfx/gfx_config.h"
#include "gfx/gfx_device.h"

struct WGPUBindGroupImpl {};

namespace vkgfx {

// https://gpuweb.github.io/gpuweb/#gpubindgroup
class GFXBindGroup : public RefCounted<GFXBindGroup>, public WGPUBindGroupImpl {
 public:
  GFXBindGroup(VkDescriptorPool descriptor_pool,
               VkDescriptorSet descriptor_set,
               RefPtr<GFXDevice> device,
               WGPUStringView label);
  ~GFXBindGroup();

  GFXBindGroup(const GFXBindGroup&) = delete;
  GFXBindGroup& operator=(const GFXBindGroup&) = delete;

  VkDescriptorSet GetVkHandle() const { return descriptor_set_; }

  void SetLabel(WGPUStringView label);
  void Write(const WGPUBindGroupDescriptor* descriptor);

 private:
  VkDescriptorPool descriptor_pool_;
  VkDescriptorSet descriptor_set_;

  RefPtr<GFXDevice> device_;

  std::string label_ = "GFX.BindGroup";
};

}  // namespace vkgfx

#endif  // GFX_GFX_BIND_GROUP_H_
