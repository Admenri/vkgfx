// Copyright 2025 Admenri.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#include "gfx/gfx_instance.h"

#include <array>
#include <vector>

#include "gfx/common/log.h"
#include "gfx/gfx_adapter.h"

namespace vkgfx {

namespace {

const std::vector<const char*> kLayerNames = {
    "VK_LAYER_KHRONOS_validation",
};

const std::vector<const char*> kExtensionNames = {
    VK_EXT_DEBUG_UTILS_EXTENSION_NAME,
};

const std::array<WGPUInstanceFeatureName, 3> kSupportedInstanceFeatures = {
    WGPUInstanceFeatureName::WGPUInstanceFeatureName_TimedWaitAny,
    WGPUInstanceFeatureName::WGPUInstanceFeatureName_ShaderSourceSPIRV,
    WGPUInstanceFeatureName::WGPUInstanceFeatureName_MultipleDevicesPerAdapter,
};

VKAPI_ATTR VkBool32 VKAPI_CALL OnInstanceCreationDebugUtilsCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageTypes,
    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
    void* pUserData) {
  GFX_DEBUG() << pCallbackData->pMessage;
  return VK_FALSE;
}

bool InitializeVolkLoader() {
  static VkResult volk_result = VK_NOT_READY;
  static std::once_flag volk_init_flag;
  std::call_once(volk_init_flag, [&]() { volk_result = volkInitialize(); });
  return volk_result == VK_SUCCESS;
}

}  // namespace

// static
RefPtr<GFXInstance> GFXInstance::Create(
    WGPU_NULLABLE WGPUInstanceDescriptor const* descriptor) {
  if (!InitializeVolkLoader()) {
    GFX_ERROR() << "Failed to initialize volk loader.";
    return nullptr;
  }

  uint32_t instance_version = 0;
  vkEnumerateInstanceVersion(&instance_version);
  if (instance_version < VK_API_VERSION_1_1) {
    GFX_ERROR() << "Vulkan 1.1 or higher is required.";
    return nullptr;
  }

  uint32_t layer_count;
  vkEnumerateInstanceLayerProperties(&layer_count, nullptr);
  std::vector<VkLayerProperties> available_layers(layer_count);
  vkEnumerateInstanceLayerProperties(&layer_count, available_layers.data());
  for (const auto& layer : available_layers) {
    GFX_INFO() << "Available layer: " << layer.layerName;
  }

  uint32_t extension_count;
  vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, nullptr);
  std::vector<VkExtensionProperties> available_extensions(extension_count);
  vkEnumerateInstanceExtensionProperties(nullptr, &extension_count,
                                         available_extensions.data());
  for (const auto& extension : available_extensions) {
    GFX_INFO() << "Available extension: " << extension.extensionName;
  }

  // Vulkan 1.1 required
  VkApplicationInfo application_info{};
  application_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  application_info.pEngineName = "VkGFX";
  application_info.engineVersion = VK_MAKE_VERSION(0, 0, 0);
  application_info.apiVersion = VK_API_VERSION_1_1;

  // Instance info with validation layers and debug utils extension enabled
  VkInstanceCreateInfo create_info{};
  create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  create_info.pApplicationInfo = &application_info;
  create_info.enabledLayerCount = static_cast<uint32_t>(kLayerNames.size());
  create_info.ppEnabledLayerNames = kLayerNames.data();
  create_info.enabledExtensionCount =
      static_cast<uint32_t>(kExtensionNames.size());
  create_info.ppEnabledExtensionNames = kExtensionNames.data();

  // Debug utils messenger create info for instance creation
  VkDebugUtilsMessengerCreateInfoEXT utils_messenger_create_info{};
  utils_messenger_create_info.sType =
      VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
  utils_messenger_create_info.messageSeverity =
      VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
      VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
  utils_messenger_create_info.messageType =
      VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
      VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
      VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
  utils_messenger_create_info.pfnUserCallback =
      OnInstanceCreationDebugUtilsCallback;
  utils_messenger_create_info.pUserData = nullptr;

  create_info.pNext = &utils_messenger_create_info;

  VkInstance instance;
  if (vkCreateInstance(&create_info, nullptr, &instance) != VK_SUCCESS) {
    GFX_ERROR() << "Failed to create Vulkan instance.";
    return nullptr;
  }

  volkLoadInstance(instance);

  VkDebugUtilsMessengerEXT debug_messenger;
  if (vkCreateDebugUtilsMessengerEXT(instance, &utils_messenger_create_info,
                                     nullptr, &debug_messenger) != VK_SUCCESS)
    debug_messenger = VK_NULL_HANDLE;

  return RefPtr<GFXInstance>(new GFXInstance(instance, debug_messenger));
}

// static
void GFXInstance::GetFeatures(WGPUSupportedInstanceFeatures* features) {
  features->featureCount =
      static_cast<uint32_t>(kSupportedInstanceFeatures.size());
  features->features = kSupportedInstanceFeatures.data();
}

// static
WGPUStatus GFXInstance::GetLimits(WGPUInstanceLimits* limits) {
  if (limits->nextInChain) {
    GFX_ERROR() << "Unsupported chained struct in GFXInstance::GetLimits.";
    return WGPUStatus_Error;
  }

  limits->timedWaitAnyMaxCount = std::numeric_limits<size_t>::max();
  return WGPUStatus_Success;
}

// static
WGPUBool GFXInstance::HasFeature(WGPUInstanceFeatureName feature) {
  return std::find(kSupportedInstanceFeatures.begin(),
                   kSupportedInstanceFeatures.end(),
                   feature) != kSupportedInstanceFeatures.end();
}

// static
void GFXInstance::SupportedInstanceFeaturesFreeMembers(
    WGPUSupportedInstanceFeatures supportedInstanceFeatures) {
  // Nothing to free for now.
}

// static
void GFXInstance::SupportedWGSLLanguageFeaturesFreeMembers(
    WGPUSupportedWGSLLanguageFeatures supportedWGSLLanguageFeatures) {
  // TODO: WGSL compiler integration
}

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

void GFXInstance::ProcessEvents() {}

WGPUFuture GFXInstance::RequestAdapter(
    WGPU_NULLABLE WGPURequestAdapterOptions const* options,
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

  GFXAdapter* adapter_impl = nullptr;
  if (adapter_count)
    adapter_impl = new GFXAdapter(physical_devices[0]);

  WGPUStringView message = {};
  callbackInfo.callback(WGPURequestAdapterStatus_Success, adapter_impl, message,
                        callbackInfo.userdata1, callbackInfo.userdata2);

  return kImmediateFuture;
}

WGPUWaitStatus GFXInstance::WaitAny(size_t futureCount,
                                    WGPU_NULLABLE WGPUFutureWaitInfo* futures,
                                    uint64_t timeoutNS) {
  if (!futures)
    return WGPUWaitStatus_Error;

  for (size_t i = 0; i < futureCount; ++i)
    if (futures[i].future.id == kImmediateFuture.id)
      futures[i].completed = WGPU_TRUE;

  return WGPUWaitStatus_Success;
}

}  // namespace vkgfx
