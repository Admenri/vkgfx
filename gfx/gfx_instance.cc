// Copyright 2025 Admenri.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#include "gfx/gfx_instance.h"

#include <array>
#include <vector>

#include "gfx/common/log.h"
#include "gfx/gfx_adapter.h"

namespace vkgfx {

///////////////////////////////////////////////////////////////////////////////
// GFXInstance Implement

GFXInstance::GFXInstance(VkInstance instance,
                         VkDebugUtilsMessengerEXT debug_messenger)
    : instance_(instance), debug_messenger_(debug_messenger) {}

GFXInstance::~GFXInstance() {
  if (debug_messenger_)
    vkDestroyDebugUtilsMessengerEXT(instance_, debug_messenger_, nullptr);
  if (instance_)
    vkDestroyInstance(instance_, nullptr);
}

WGPUSurface GFXInstance::CreateSurface(
    WGPUSurfaceDescriptor const* descriptor) {
  return WGPUSurface();
}

void GFXInstance::GetWGSLLanguageFeatures(
    WGPUSupportedWGSLLanguageFeatures* features) {
  // TODO: WGSL compiler integration
}

WGPUBool GFXInstance::HasWGSLLanguageFeature(
    WGPUWGSLLanguageFeatureName feature) {
  // TODO: WGSL compiler integration
  return WGPUBool();
}

void GFXInstance::ProcessEvents() {
  // No-op
}

WGPUFuture GFXInstance::RequestAdapter(
    WGPURequestAdapterOptions const* options,
    WGPURequestAdapterCallbackInfo callbackInfo) {
  if (!callbackInfo.callback)
    return kInvalidFuture;

  uint32_t adapter_count = 0;
  VkResult count_result =
      vkEnumeratePhysicalDevices(instance_, &adapter_count, nullptr);
  if (count_result != VK_SUCCESS)
    return kInvalidFuture;

  std::vector<VkPhysicalDevice> physical_devices(adapter_count);
  VkResult enum_result = vkEnumeratePhysicalDevices(instance_, &adapter_count,
                                                    physical_devices.data());
  if (count_result != VK_SUCCESS)
    return kInvalidFuture;

  for (uint32_t i = 0; i < adapter_count; ++i) {
    VkPhysicalDevice vk_adapter = physical_devices[i];

    VkPhysicalDeviceProperties device_properties;
    vkGetPhysicalDeviceProperties(vk_adapter, &device_properties);

    GFX_INFO() << "[APIVersion] "
               << VK_API_VERSION_MAJOR(device_properties.apiVersion) << "."
               << VK_API_VERSION_MINOR(device_properties.apiVersion) << "."
               << VK_API_VERSION_PATCH(device_properties.apiVersion);
    GFX_INFO() << "[DriverVersion] " << device_properties.driverVersion;
    GFX_INFO() << "[VendorID] " << device_properties.vendorID;
    GFX_INFO() << "[DeviceID] " << device_properties.deviceID;
    GFX_INFO() << "[DeviceType] " << device_properties.deviceType;
    GFX_INFO() << "[DeviceName] " << device_properties.deviceName;

    VkPhysicalDeviceFeatures device_features;
    vkGetPhysicalDeviceFeatures(vk_adapter, &device_features);
  }

  // TODO: select adapter based on options
  GFXAdapter* adapter_impl = nullptr;
  if (adapter_count)
    adapter_impl = new GFXAdapter(physical_devices[0]);

  WGPUStringView message = {};
  callbackInfo.callback(WGPURequestAdapterStatus_Success, adapter_impl, message,
                        callbackInfo.userdata1, callbackInfo.userdata2);

  return kImmediateFuture;
}

WGPUWaitStatus GFXInstance::WaitAny(size_t futureCount,
                                    WGPUFutureWaitInfo* futures,
                                    uint64_t timeoutNS) {
  if (!futures)
    return WGPUWaitStatus_Error;

  for (size_t i = 0; i < futureCount; ++i)
    if (futures[i].future.id == kImmediateFuture.id)
      futures[i].completed = WGPU_TRUE;

  return WGPUWaitStatus_Success;
}

}  // namespace vkgfx

///////////////////////////////////////////////////////////////////////////////
// Member Function

GFX_REFCOUNTED_EXPORT(Instance, vkgfx::GFXInstance);

GFX_EXPORT WGPUSurface
GFX_FUNCTION(InstanceCreateSurface)(WGPUInstance instance,
                                    WGPUSurfaceDescriptor const* descriptor) {
  auto* self = static_cast<vkgfx::GFXInstance*>(instance);
  return self->CreateSurface(descriptor);
}

GFX_EXPORT void GFX_FUNCTION(InstanceGetWGSLLanguageFeatures)(
    WGPUInstance instance,
    WGPUSupportedWGSLLanguageFeatures* features) {
  auto* self = static_cast<vkgfx::GFXInstance*>(instance);
  self->GetWGSLLanguageFeatures(features);
}

GFX_EXPORT WGPUBool GFX_FUNCTION(InstanceHasWGSLLanguageFeature)(
    WGPUInstance instance,
    WGPUWGSLLanguageFeatureName feature) {
  auto* self = static_cast<vkgfx::GFXInstance*>(instance);
  return self->HasWGSLLanguageFeature(feature);
}

GFX_EXPORT void GFX_FUNCTION(InstanceProcessEvents)(WGPUInstance instance) {
  auto* self = static_cast<vkgfx::GFXInstance*>(instance);
  self->ProcessEvents();
}

GFX_EXPORT WGPUFuture GFX_FUNCTION(InstanceRequestAdapter)(
    WGPUInstance instance,
    WGPU_NULLABLE WGPURequestAdapterOptions const* options,
    WGPURequestAdapterCallbackInfo callbackInfo) {
  auto* self = static_cast<vkgfx::GFXInstance*>(instance);
  return self->RequestAdapter(options, callbackInfo);
}

GFX_EXPORT WGPUWaitStatus
GFX_FUNCTION(InstanceWaitAny)(WGPUInstance instance,
                              size_t futureCount,
                              WGPU_NULLABLE WGPUFutureWaitInfo* futures,
                              uint64_t timeoutNS) {
  auto* self = static_cast<vkgfx::GFXInstance*>(instance);
  return self->WaitAny(futureCount, futures, timeoutNS);
}
