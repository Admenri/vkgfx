// Copyright 2025 Admenri.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#include "gfx/gfx_adapter.h"

namespace vkgfx {

// static
void GFXAdapter::AdapterInfoFreeMembers(WGPUAdapterInfo adapterInfo) {}

// static
void GFXAdapter::SupportedFeaturesFreeMembers(
    WGPUSupportedFeatures supportedFeatures) {}

///////////////////////////////////////////////////////////////////////////////
// GFXAdapter Implement

GFXAdapter::GFXAdapter(VkPhysicalDevice adapter) : adapter_(adapter) {}

GFXAdapter::~GFXAdapter() = default;

void GFXAdapter::GetFeatures(WGPUSupportedFeatures* features) {}

WGPUStatus GFXAdapter::GetInfo(WGPUAdapterInfo* info) {
  return WGPUStatus();
}

WGPUStatus GFXAdapter::GetLimits(WGPULimits* limits) {
  return WGPUStatus();
}

WGPUBool GFXAdapter::HasFeature(WGPUFeatureName feature) {
  return WGPUBool();
}

WGPUFuture GFXAdapter::RequestDevice(
    WGPU_NULLABLE WGPUDeviceDescriptor const* descriptor,
    WGPURequestDeviceCallbackInfo callbackInfo) {
  VkPhysicalDeviceFeatures2 device_features{};
  device_features.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2;

  VkDeviceCreateInfo create_info{};
  create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
  create_info.pNext = &device_features;

  VkDevice device;
  VkResult result = vkCreateDevice(adapter_, &create_info, nullptr, &device);
  if (result != VK_SUCCESS) {
    return WGPUFuture();
  }

  return WGPUFuture();
}

}  // namespace vkgfx
