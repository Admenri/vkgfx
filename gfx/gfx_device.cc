// Copyright 2025 Admenri.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#include "gfx/gfx_device.h"

#include <map>

#include "gfx/gfx_bind_group.h"
#include "gfx/gfx_bind_group_layout.h"
#include "gfx/gfx_buffer.h"
#include "gfx/gfx_sampler.h"
#include "gfx/gfx_texture.h"
#include "gfx/gfx_utils.h"

namespace vkgfx {

///////////////////////////////////////////////////////////////////////////////
// GFXDevice Implement

GFXDevice::GFXDevice(VkDevice device,
                     RefPtr<GFXAdapter> adapter,
                     WGPUStringView label,
                     WGPUDeviceLostCallbackInfo device_lost_callback,
                     WGPUUncapturedErrorCallbackInfo uncaptured_error_callback)
    : device_(device),
      adapter_(adapter),
      device_lost_callback_(device_lost_callback),
      uncaptured_error_callback_(uncaptured_error_callback) {
  if (label.data && label.length)
    label_ = std::string(label.data, label.length);

  CreateAllocatorInternal();
}

GFXDevice::~GFXDevice() {
  Destroy();
}

void GFXDevice::CallDeviceLostCallback(WGPUDeviceLostReason reason,
                                       const std::string& message) {
  if (device_lost_callback_.callback) {
    WGPUDevice wgpu_device = this;
    WGPUStringView wgpu_string = {message.c_str(), message.size()};
    device_lost_callback_.callback(&wgpu_device, reason, wgpu_string,
                                   device_lost_callback_.userdata1,
                                   device_lost_callback_.userdata2);
  }
}

void GFXDevice::CallDeviceErrorCallback(WGPUErrorType type,
                                        const std::string& message) {
  if (uncaptured_error_callback_.callback) {
    WGPUDevice wgpu_device = this;
    WGPUStringView wgpu_string = {message.c_str(), message.size()};
    uncaptured_error_callback_.callback(&wgpu_device, type, wgpu_string,
                                        uncaptured_error_callback_.userdata1,
                                        uncaptured_error_callback_.userdata2);
  }
}

WGPUBindGroup GFXDevice::CreateBindGroup(
    WGPUBindGroupDescriptor const* descriptor) {
  if (!device_)
    return nullptr;

  if (!descriptor)
    return nullptr;

  if (!descriptor->layout)
    return nullptr;

  auto* bind_group_layout =
      static_cast<GFXBindGroupLayout*>(descriptor->layout);

  // Pool
  std::vector<VkDescriptorPoolSize> pool_sizes;
  for (const auto& it : bind_group_layout->GetLayoutEntries()) {
    auto descriptor_type = ToVulkanDescriptorType(it.main);
    auto iter = std::find_if(pool_sizes.begin(), pool_sizes.end(),
                             [&](const VkDescriptorPoolSize& it) {
                               return it.type == descriptor_type;
                             });

    auto descriptor_count = std::max<uint32_t>(1, it.main.bindingArraySize);
    if (iter != pool_sizes.end()) {
      iter->descriptorCount += descriptor_count;
    } else {
      pool_sizes.push_back(
          VkDescriptorPoolSize{descriptor_type, descriptor_count});
    }
  }

  VkDescriptorPoolCreateInfo pool_create_info = {
      VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO};
  pool_create_info.maxSets = 1;
  pool_create_info.poolSizeCount = pool_sizes.size();
  pool_create_info.pPoolSizes = pool_sizes.data();

  VkDescriptorPool pool;
  if (vkCreateDescriptorPool(device_, &pool_create_info, nullptr, &pool) !=
      VK_SUCCESS)
    return nullptr;

  // Set
  auto vk_layout = bind_group_layout->GetVkHandle();

  VkDescriptorSetAllocateInfo allocate_info = {
      VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO};
  allocate_info.descriptorPool = pool;
  allocate_info.descriptorSetCount = 1;
  allocate_info.pSetLayouts = &vk_layout;

  VkDescriptorSet descriptor_set;
  vkAllocateDescriptorSets(device_, &allocate_info, &descriptor_set);

  auto* bind_group =
      new GFXBindGroup(pool, descriptor_set, this, descriptor->label);
  bind_group->Write(descriptor);

  return AdaptExternalRefCounted(bind_group);
}

WGPUBindGroupLayout GFXDevice::CreateBindGroupLayout(
    WGPUBindGroupLayoutDescriptor const* descriptor) {
  if (!device_)
    return nullptr;

  if (!descriptor)
    return nullptr;

  std::vector<GFXBindGroupLayout::LayoutEntry> layout_entries;
  std::vector<VkDescriptorSetLayoutBinding> vk_bindings;
  for (size_t i = 0; i < descriptor->entryCount; ++i) {
    const auto& entry = descriptor->entries[i];

    VkDescriptorSetLayoutBinding vk_binding = {};
    vk_binding.binding = entry.binding;
    vk_binding.descriptorType = ToVulkanDescriptorType(entry);
    vk_binding.descriptorCount = 1;
    vk_binding.stageFlags = ToVulkanShaderStages(entry.visibility);
    vk_bindings.push_back(vk_binding);

    GFXBindGroupLayout::LayoutEntry layout_entry = {};
    layout_entry.main = entry;
    layout_entries.push_back(layout_entry);
  }

  VkDescriptorSetLayoutCreateInfo create_info = {
      VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO};
  create_info.bindingCount = vk_bindings.size();
  create_info.pBindings = vk_bindings.data();

  VkDescriptorSetLayout layout;
  if (vkCreateDescriptorSetLayout(device_, &create_info, nullptr, &layout) !=
      VK_SUCCESS)
    return nullptr;

  return AdaptExternalRefCounted(
      new GFXBindGroupLayout(layout, layout_entries, this, descriptor->label));
}

WGPUBuffer GFXDevice::CreateBuffer(WGPUBufferDescriptor const* descriptor) {
  if (!device_)
    return nullptr;

  if (!descriptor)
    return nullptr;

  VkBufferCreateInfo create_info = {VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO};
  create_info.size = descriptor->size;
  create_info.usage = ToVulkanBufferUsage(descriptor->usage);

  VmaAllocationCreateInfo allocation_info = {};
  allocation_info.usage = VMA_MEMORY_USAGE_AUTO;

  VkBuffer buffer;
  VmaAllocation allocation;
  if (vmaCreateBuffer(allocator_, &create_info, &allocation_info, &buffer,
                      &allocation, nullptr) != VK_SUCCESS)
    return nullptr;

  if (descriptor->mappedAtCreation) {
  } else {
  }

  return AdaptExternalRefCounted(
      new GFXBuffer(buffer, allocation, this, descriptor->label));
}

WGPUCommandEncoder GFXDevice::CreateCommandEncoder(
    WGPUCommandEncoderDescriptor const* descriptor) {
  if (!device_)
    return nullptr;

  return WGPUCommandEncoder();
}

WGPUComputePipeline GFXDevice::CreateComputePipeline(
    WGPUComputePipelineDescriptor const* descriptor) {
  if (!device_)
    return nullptr;

  return WGPUComputePipeline();
}

WGPUFuture GFXDevice::CreateComputePipelineAsync(
    WGPUComputePipelineDescriptor const* descriptor,
    WGPUCreateComputePipelineAsyncCallbackInfo callbackInfo) {
  if (!device_)
    return GFXInstance::kInvalidFuture;

  return WGPUFuture();
}

WGPUPipelineLayout GFXDevice::CreatePipelineLayout(
    WGPUPipelineLayoutDescriptor const* descriptor) {
  if (!device_)
    return nullptr;

  return WGPUPipelineLayout();
}

WGPUQuerySet GFXDevice::CreateQuerySet(
    WGPUQuerySetDescriptor const* descriptor) {
  if (!device_)
    return nullptr;

  return WGPUQuerySet();
}

WGPURenderBundleEncoder GFXDevice::CreateRenderBundleEncoder(
    WGPURenderBundleEncoderDescriptor const* descriptor) {
  if (!device_)
    return nullptr;

  return WGPURenderBundleEncoder();
}

WGPURenderPipeline GFXDevice::CreateRenderPipeline(
    WGPURenderPipelineDescriptor const* descriptor) {
  if (!device_)
    return nullptr;

  return WGPURenderPipeline();
}

WGPUFuture GFXDevice::CreateRenderPipelineAsync(
    WGPURenderPipelineDescriptor const* descriptor,
    WGPUCreateRenderPipelineAsyncCallbackInfo callbackInfo) {
  if (!device_)
    return GFXInstance::kInvalidFuture;

  return WGPUFuture();
}

WGPUSampler GFXDevice::CreateSampler(WGPUSamplerDescriptor const* descriptor) {
  if (!device_)
    return nullptr;

  if (!descriptor)
    return nullptr;

  VkSamplerCreateInfo create_info = {VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO};
  create_info.addressModeU =
      ToVulkanSamplerAddressMode(descriptor->addressModeU);
  create_info.addressModeV =
      ToVulkanSamplerAddressMode(descriptor->addressModeV);
  create_info.addressModeW =
      ToVulkanSamplerAddressMode(descriptor->addressModeW);
  create_info.magFilter = ToVulkanSamplerFilter(descriptor->magFilter);
  create_info.minFilter = ToVulkanSamplerFilter(descriptor->minFilter);
  create_info.mipmapMode = ToVulkanMipMapMode(descriptor->mipmapFilter);
  create_info.minLod = descriptor->lodMinClamp;
  create_info.maxLod = descriptor->lodMaxClamp;
  if (descriptor->compare != WGPUCompareFunction_Undefined) {
    create_info.compareEnable = VK_TRUE;
    create_info.compareOp;
  } else {
    create_info.compareEnable = VK_FALSE;
    create_info.compareOp = VK_COMPARE_OP_NEVER;
  }

  if (adapter_->GetDeviceInfo().features.features.samplerAnisotropy) {
    create_info.anisotropyEnable = VK_TRUE;
    create_info.maxAnisotropy = descriptor->maxAnisotropy;
  } else {
    create_info.anisotropyEnable = VK_FALSE;
    create_info.maxAnisotropy = 1.0f;
  }

  VkSampler sampler = VK_NULL_HANDLE;
  vkCreateSampler(device_, &create_info, nullptr, &sampler);

  return AdaptExternalRefCounted(
      new GFXSampler(sampler, this, descriptor->label));
}

WGPUShaderModule GFXDevice::CreateShaderModule(
    WGPUShaderModuleDescriptor const* descriptor) {
  if (!device_)
    return nullptr;

  return WGPUShaderModule();
}

WGPUTexture GFXDevice::CreateTexture(WGPUTextureDescriptor const* descriptor) {
  if (!device_)
    return nullptr;

  if (!descriptor)
    return nullptr;

  VkImageCreateInfo create_info = {VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO};
  switch (descriptor->dimension) {
    default:
    case WGPUTextureDimension_1D:
      create_info.imageType = VK_IMAGE_TYPE_1D;
      create_info.extent = VkExtent3D{descriptor->size.width, 1, 1};
      create_info.arrayLayers = 1;
      break;
    case WGPUTextureDimension_2D:
      create_info.imageType = VK_IMAGE_TYPE_2D;
      create_info.extent =
          VkExtent3D{descriptor->size.width, descriptor->size.height, 1};
      create_info.arrayLayers = 1;
      break;
    case WGPUTextureDimension_3D:
      create_info.imageType = VK_IMAGE_TYPE_3D;
      create_info.extent =
          VkExtent3D{descriptor->size.width, descriptor->size.height,
                     descriptor->size.depthOrArrayLayers};
      create_info.arrayLayers = descriptor->size.depthOrArrayLayers;
      break;
  }
  create_info.format = ToVulkanPixelFormat(descriptor->format);
  create_info.mipLevels = descriptor->mipLevelCount;
  create_info.samples = ToVulkanSampleCount(descriptor->sampleCount);
  create_info.tiling = VK_IMAGE_TILING_OPTIMAL;
  create_info.usage = ToVulkanImageUsage(descriptor->usage, descriptor->format);
  create_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
  create_info.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

  VmaAllocationCreateInfo allocation_info = {};
  allocation_info.usage = VMA_MEMORY_USAGE_AUTO;

  VkImage image;
  VmaAllocation allocation;
  if (vmaCreateImage(allocator_, &create_info, &allocation_info, &image,
                     &allocation, nullptr) != VK_SUCCESS)
    return nullptr;

  return AdaptExternalRefCounted(
      new GFXTexture(image, allocation, this, descriptor->label));
}

void GFXDevice::Destroy() {
  if (allocator_) {
    vmaDestroyAllocator(allocator_);
    allocator_ = nullptr;
  }

  if (device_) {
    vkDestroyDevice(device_, nullptr);
    device_ = VK_NULL_HANDLE;
  }

  adapter_.reset();
}

WGPUStatus GFXDevice::GetAdapterInfo(WGPUAdapterInfo* adapterInfo) {
  if (adapter_)
    return adapter_->GetInfo(adapterInfo);

  return WGPUStatus_Error;
}

void GFXDevice::GetFeatures(WGPUSupportedFeatures* features) {
  if (adapter_)
    adapter_->GetFeatures(features);
}

WGPUStatus GFXDevice::GetLimits(WGPULimits* limits) {
  if (adapter_)
    return adapter_->GetLimits(limits);

  return WGPUStatus_Error;
}

WGPUFuture GFXDevice::GetLostFuture() {
  // TODO:
  return GFXInstance::kInvalidFuture;
}

WGPUQueue GFXDevice::GetQueue() {
  return WGPUQueue();
}

WGPUBool GFXDevice::HasFeature(WGPUFeatureName feature) {
  if (adapter_)
    return adapter_->HasFeature(feature);

  return WGPU_FALSE;
}

WGPUFuture GFXDevice::PopErrorScope(
    WGPUPopErrorScopeCallbackInfo callbackInfo) {
  return WGPUFuture();
}

void GFXDevice::PushErrorScope(WGPUErrorFilter filter) {}

void GFXDevice::SetLabel(WGPUStringView label) {
  label_ = std::string(label.data, label.length);
}

void GFXDevice::CreateAllocatorInternal() {
  VmaVulkanFunctions vulkan_functions;
  VmaAllocatorCreateInfo allocator_create_info = {};
  allocator_create_info.physicalDevice = adapter_->GetVkHandle();
  allocator_create_info.device = device_;
  allocator_create_info.instance = adapter_->GetInstance()->GetVkHandle();
  allocator_create_info.vulkanApiVersion = VK_API_VERSION_1_1;
  allocator_create_info.pVulkanFunctions = &vulkan_functions;

  vmaImportVulkanFunctionsFromVolk(&allocator_create_info, &vulkan_functions);
  vmaCreateAllocator(&allocator_create_info, &allocator_);
}

}  // namespace vkgfx

///////////////////////////////////////////////////////////////////////////////
// Member Function

GFX_REFCOUNTED_EXPORT(Device, vkgfx::GFXDevice);

GFX_EXPORT WGPUBindGroup
GFX_FUNCTION(DeviceCreateBindGroup)(WGPUDevice device,
                                    WGPUBindGroupDescriptor const* descriptor) {
  auto* self = static_cast<vkgfx::GFXDevice*>(device);
  return self->CreateBindGroup(descriptor);
}

GFX_EXPORT WGPUBindGroupLayout GFX_FUNCTION(DeviceCreateBindGroupLayout)(
    WGPUDevice device,
    WGPUBindGroupLayoutDescriptor const* descriptor) {
  auto* self = static_cast<vkgfx::GFXDevice*>(device);
  return self->CreateBindGroupLayout(descriptor);
}

GFX_EXPORT WGPU_NULLABLE WGPUBuffer
GFX_FUNCTION(DeviceCreateBuffer)(WGPUDevice device,
                                 WGPUBufferDescriptor const* descriptor) {
  auto* self = static_cast<vkgfx::GFXDevice*>(device);
  return self->CreateBuffer(descriptor);
}

GFX_EXPORT WGPUCommandEncoder GFX_FUNCTION(DeviceCreateCommandEncoder)(
    WGPUDevice device,
    WGPU_NULLABLE WGPUCommandEncoderDescriptor const* descriptor) {
  auto* self = static_cast<vkgfx::GFXDevice*>(device);
  return self->CreateCommandEncoder(descriptor);
}

GFX_EXPORT WGPUComputePipeline GFX_FUNCTION(DeviceCreateComputePipeline)(
    WGPUDevice device,
    WGPUComputePipelineDescriptor const* descriptor) {
  auto* self = static_cast<vkgfx::GFXDevice*>(device);
  return self->CreateComputePipeline(descriptor);
}

GFX_EXPORT WGPUFuture GFX_FUNCTION(DeviceCreateComputePipelineAsync)(
    WGPUDevice device,
    WGPUComputePipelineDescriptor const* descriptor,
    WGPUCreateComputePipelineAsyncCallbackInfo callbackInfo) {
  auto* self = static_cast<vkgfx::GFXDevice*>(device);
  return self->CreateComputePipelineAsync(descriptor, callbackInfo);
}

GFX_EXPORT WGPUPipelineLayout GFX_FUNCTION(DeviceCreatePipelineLayout)(
    WGPUDevice device,
    WGPUPipelineLayoutDescriptor const* descriptor) {
  auto* self = static_cast<vkgfx::GFXDevice*>(device);
  return self->CreatePipelineLayout(descriptor);
}

GFX_EXPORT WGPUQuerySet
GFX_FUNCTION(DeviceCreateQuerySet)(WGPUDevice device,
                                   WGPUQuerySetDescriptor const* descriptor) {
  auto* self = static_cast<vkgfx::GFXDevice*>(device);
  return self->CreateQuerySet(descriptor);
}

GFX_EXPORT WGPURenderBundleEncoder
GFX_FUNCTION(DeviceCreateRenderBundleEncoder)(
    WGPUDevice device,
    WGPURenderBundleEncoderDescriptor const* descriptor) {
  auto* self = static_cast<vkgfx::GFXDevice*>(device);
  return self->CreateRenderBundleEncoder(descriptor);
}

GFX_EXPORT WGPURenderPipeline GFX_FUNCTION(DeviceCreateRenderPipeline)(
    WGPUDevice device,
    WGPURenderPipelineDescriptor const* descriptor) {
  auto* self = static_cast<vkgfx::GFXDevice*>(device);
  return self->CreateRenderPipeline(descriptor);
}

GFX_EXPORT WGPUFuture GFX_FUNCTION(DeviceCreateRenderPipelineAsync)(
    WGPUDevice device,
    WGPURenderPipelineDescriptor const* descriptor,
    WGPUCreateRenderPipelineAsyncCallbackInfo callbackInfo) {
  auto* self = static_cast<vkgfx::GFXDevice*>(device);
  return self->CreateRenderPipelineAsync(descriptor, callbackInfo);
}

GFX_EXPORT WGPUSampler GFX_FUNCTION(DeviceCreateSampler)(
    WGPUDevice device,
    WGPU_NULLABLE WGPUSamplerDescriptor const* descriptor) {
  auto* self = static_cast<vkgfx::GFXDevice*>(device);
  return self->CreateSampler(descriptor);
}

GFX_EXPORT WGPUShaderModule GFX_FUNCTION(DeviceCreateShaderModule)(
    WGPUDevice device,
    WGPUShaderModuleDescriptor const* descriptor) {
  auto* self = static_cast<vkgfx::GFXDevice*>(device);
  return self->CreateShaderModule(descriptor);
}

GFX_EXPORT WGPUTexture
GFX_FUNCTION(DeviceCreateTexture)(WGPUDevice device,
                                  WGPUTextureDescriptor const* descriptor) {
  auto* self = static_cast<vkgfx::GFXDevice*>(device);
  return self->CreateTexture(descriptor);
}

GFX_EXPORT void GFX_FUNCTION(DeviceDestroy)(WGPUDevice device) {
  auto* self = static_cast<vkgfx::GFXDevice*>(device);
  self->Destroy();
}

GFX_EXPORT WGPUStatus GFX_FUNCTION(
    DeviceGetAdapterInfo)(WGPUDevice device, WGPUAdapterInfo* adapterInfo) {
  auto* self = static_cast<vkgfx::GFXDevice*>(device);
  return self->GetAdapterInfo(adapterInfo);
}

GFX_EXPORT void GFX_FUNCTION(
    DeviceGetFeatures)(WGPUDevice device, WGPUSupportedFeatures* features) {
  auto* self = static_cast<vkgfx::GFXDevice*>(device);
  self->GetFeatures(features);
}

GFX_EXPORT WGPUStatus GFX_FUNCTION(DeviceGetLimits)(WGPUDevice device,
                                                    WGPULimits* limits) {
  auto* self = static_cast<vkgfx::GFXDevice*>(device);
  return self->GetLimits(limits);
}

GFX_EXPORT WGPUFuture GFX_FUNCTION(DeviceGetLostFuture)(WGPUDevice device) {
  auto* self = static_cast<vkgfx::GFXDevice*>(device);
  return self->GetLostFuture();
}

GFX_EXPORT WGPUQueue GFX_FUNCTION(DeviceGetQueue)(WGPUDevice device) {
  auto* self = static_cast<vkgfx::GFXDevice*>(device);
  return self->GetQueue();
}

GFX_EXPORT WGPUBool GFX_FUNCTION(DeviceHasFeature)(WGPUDevice device,
                                                   WGPUFeatureName feature) {
  auto* self = static_cast<vkgfx::GFXDevice*>(device);
  return self->HasFeature(feature);
}

GFX_EXPORT WGPUFuture
GFX_FUNCTION(DevicePopErrorScope)(WGPUDevice device,
                                  WGPUPopErrorScopeCallbackInfo callbackInfo) {
  auto* self = static_cast<vkgfx::GFXDevice*>(device);
  return self->PopErrorScope(callbackInfo);
}

GFX_EXPORT void GFX_FUNCTION(DevicePushErrorScope)(WGPUDevice device,
                                                   WGPUErrorFilter filter) {
  auto* self = static_cast<vkgfx::GFXDevice*>(device);
  self->PushErrorScope(filter);
}

GFX_EXPORT void GFX_FUNCTION(DeviceSetLabel)(WGPUDevice device,
                                             WGPUStringView label) {
  auto* self = static_cast<vkgfx::GFXDevice*>(device);
  self->SetLabel(label);
}
