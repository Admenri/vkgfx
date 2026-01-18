// Copyright 2025 Admenri.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#ifndef GFX_GFX_BIND_GROUP_LAYOUT_H_
#define GFX_GFX_BIND_GROUP_LAYOUT_H_

#include <span>
#include <vector>

#include "gfx/common/refptr.h"
#include "gfx/gfx_config.h"
#include "gfx/gfx_device.h"

struct WGPUBindGroupLayoutImpl {};

namespace vkgfx {

// https://gpuweb.github.io/gpuweb/#gpubindgrouplayout
class GFXBindGroupLayout : public RefCounted<GFXBindGroupLayout>,
                           public WGPUBindGroupLayoutImpl {
 public:
  struct LayoutEntry {
    // Main struct
    WGPUBindGroupLayoutEntry main;
    // Chained struct
  };

  GFXBindGroupLayout(VkDescriptorSetLayout layout,
                     const std::vector<LayoutEntry>& entries,
                     RefPtr<GFXDevice> device,
                     WGPUStringView label);
  ~GFXBindGroupLayout();

  GFXBindGroupLayout(const GFXBindGroupLayout&) = delete;
  GFXBindGroupLayout& operator=(const GFXBindGroupLayout&) = delete;

  VkDescriptorSetLayout GetVkHandle() const { return layout_; }
  std::span<LayoutEntry> GetLayoutEntries() {
    return std::span<LayoutEntry>(entries_);
  }

  void SetLabel(WGPUStringView label);

 private:
  VkDescriptorSetLayout layout_;
  std::vector<LayoutEntry> entries_;

  RefPtr<GFXDevice> device_;

  std::string label_ = "GFX.BindGroupLayout";
};

}  // namespace vkgfx

#endif  // GFX_GFX_BIND_GROUP_LAYOUT_H_
