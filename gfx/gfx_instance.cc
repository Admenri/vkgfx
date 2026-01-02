// Copyright 2025 Admenri.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#include "gfx/gfx_instance.h"

#include <array>
#include <vector>

#include "gfx/common/log.h"
#include "gfx/common/platform.h"
#include "gfx/gfx_adapter.h"
#include "gfx/gfx_surface.h"
#include "gfx/gfx_utils.h"

namespace vkgfx {

///////////////////////////////////////////////////////////////////////////////
// GFXInstance Implement

GFXInstance::GFXInstance(VkInstance instance,
                         VkDebugUtilsMessengerEXT debug_messenger)
    : instance_(instance), debug_messenger_(debug_messenger) {}

GFXInstance::~GFXInstance() {
  if (instance_ && debug_messenger_)
    vkDestroyDebugUtilsMessengerEXT(instance_, debug_messenger_, nullptr);
  if (instance_)
    vkDestroyInstance(instance_, nullptr);
}

WGPUSurface GFXInstance::CreateSurface(
    WGPUSurfaceDescriptor const* descriptor) {
  if (!descriptor)
    return nullptr;

  ChainedStructExtractor chained_extractor(descriptor->nextInChain);
  VkSurfaceKHR surface = VK_NULL_HANDLE;

#if GFX_PLATFORM_IS(WIN32)
  auto* surface_source_hwnd =
      chained_extractor.GetStruct<WGPUSurfaceSourceWindowsHWND>(
          WGPUSType_SurfaceSourceWindowsHWND);

  VkWin32SurfaceCreateInfoKHR win32_create_info = {
      VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR};
  win32_create_info.hinstance =
      reinterpret_cast<HINSTANCE>(surface_source_hwnd->hinstance);
  win32_create_info.hwnd = reinterpret_cast<HWND>(surface_source_hwnd->hwnd);

  vkCreateWin32SurfaceKHR(instance_, &win32_create_info, nullptr, &surface);
#else
#error Unsupport Platform Surface
#endif

  if (surface == VK_NULL_HANDLE)
    return nullptr;

  return AdaptExternalRefCounted(
      new GFXSurface(surface, this, descriptor->label));
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
  if (!callbackInfo.callback) {
    GFX_ERROR() << __FUNCTION__ << ": callback is null.";
    return kInvalidFuture;
  }

  uint32_t adapter_count = 0;
  vkEnumeratePhysicalDevices(instance_, &adapter_count, nullptr);
  std::vector<VkPhysicalDevice> physical_devices(adapter_count);
  vkEnumeratePhysicalDevices(instance_, &adapter_count,
                             physical_devices.data());
  if (!adapter_count) {
    GFX_ERROR() << __FUNCTION__ << ": no adapters found.";
    return kInvalidFuture;
  }

  for (uint32_t i = 0; i < adapter_count; ++i) {
    VkPhysicalDevice vk_adapter = physical_devices[i];

    VkPhysicalDeviceProperties device_properties;
    vkGetPhysicalDeviceProperties(vk_adapter, &device_properties);

    GFX_INFO() << "========================= Device " << i
               << " ========================";
    GFX_INFO() << "[APIVersion] "
               << VK_API_VERSION_MAJOR(device_properties.apiVersion) << "."
               << VK_API_VERSION_MINOR(device_properties.apiVersion) << "."
               << VK_API_VERSION_PATCH(device_properties.apiVersion);
    GFX_INFO() << "[DriverVersion] " << device_properties.driverVersion;
    GFX_INFO() << "[VendorID] " << device_properties.vendorID;
    GFX_INFO() << "[DeviceID] " << device_properties.deviceID;
    GFX_INFO() << "[DeviceType] " << device_properties.deviceType;
    GFX_INFO() << "[DeviceName] " << device_properties.deviceName;
    GFX_INFO() << "===========================================================";
  }

  // TODO: select adapter based on options
  GFXAdapter* adapter_impl = nullptr;
  if (adapter_count)
    adapter_impl = new GFXAdapter(physical_devices[0], this);

  WGPUStringView message = {};
  callbackInfo.callback(WGPURequestAdapterStatus_Success,
                        AdaptExternalRefCounted(adapter_impl), message,
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
