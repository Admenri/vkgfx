// Copyright 2025 Admenri.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#include "gfx/gfx_bind_group.h"

#include "gfx/gfx_bind_group_layout.h"
#include "gfx/gfx_buffer.h"
#include "gfx/gfx_utils.h"

namespace vkgfx {

///////////////////////////////////////////////////////////////////////////////
// GFXBindGroup Implement

GFXBindGroup::GFXBindGroup(VkDescriptorPool descriptor_pool,
                           VkDescriptorSet descriptor_set,
                           RefPtr<GFXDevice> device,
                           WGPUStringView label)
    : descriptor_pool_(descriptor_pool),
      descriptor_set_(descriptor_set),
      device_(device) {
  if (label.data && label.length)
    label_ = std::string(label.data, label.length);
}

GFXBindGroup::~GFXBindGroup() {
  // Implicit release of DescriptorSet
  if (descriptor_pool_)
    vkDestroyDescriptorPool(device_->GetVkHandle(), descriptor_pool_, nullptr);
}

void GFXBindGroup::SetLabel(WGPUStringView label) {
  label_ = std::string(label.data, label.length);
}

void GFXBindGroup::Write(const WGPUBindGroupDescriptor* descriptor) {
  /*if (!descriptor)
    return;

  if (!descriptor->layout)
    return;

  auto* bind_group_layout =
      static_cast<GFXBindGroupLayout*>(descriptor->layout);
  auto layout_entries = bind_group_layout->GetLayoutEntries();

  std::vector<VkWriteDescriptorSet> write_infos;
  for (size_t i = 0; i < descriptor->entryCount; ++i) {
    const auto& descriptor_entry = descriptor->entries[i];

    VkWriteDescriptorSet write_info = {VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET};
    write_info.dstSet = descriptor_set_;
    write_info.dstBinding = descriptor_entry.binding;
    write_info.dstArrayElement = 0;
    write_info.descriptorCount = 1;
    write_info.descriptorType = ;

    if (descriptor_entry.buffer) {
      auto* buffer_info = new VkDescriptorBufferInfo{};
      buffer_info->buffer = ;
      buffer_info->offset = descriptor_entry.offset;
      buffer_info->range = descriptor_entry.size;
      write_info.pBufferInfo = buffer_info;
    } else if (descriptor_entry.sampler) {
      auto* sampler_info = new VkDescriptorImageInfo{};
      sampler_info->sampler = ;
      write_info.pImageInfo = sampler_info;
    } else if (descriptor_entry.textureView) {
      auto* image_info = new VkDescriptorImageInfo{};
      image_info->imageView = ;
      image_info->imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
      write_info.pImageInfo = image_info;
    }

    write_infos.push_back(write_info);
  }

  vkUpdateDescriptorSets(device_->GetVkHandle(), write_infos.size(),
                         write_infos.data(), 0, nullptr);*/
}

}  // namespace vkgfx

///////////////////////////////////////////////////////////////////////////////
// Member Function

GFX_REFCOUNTED_EXPORT(BindGroup, vkgfx::GFXBindGroup);

GFX_EXPORT void GFX_FUNCTION(BindGroupSetLabel)(WGPUBindGroup bindGroup,
                                                WGPUStringView label) {
  auto* self = static_cast<vkgfx::GFXBindGroup*>(bindGroup);
  self->SetLabel(label);
}
