// Copyright 2025 Admenri.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#include "gfx/gfx_wgpu.h"

#include <array>
#include <vector>

#include "gfx/common/log.h"
#include "gfx/gfx_instance.h"
#include "gfx/gfx_utils.h"

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

///////////////////////////////////////////////////////////////////////////////
// Global Functions

// static
GFXInstance* CreateInstance(WGPUInstanceDescriptor const* descriptor) {
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

  if (volkGetLoadedInstance() != VK_NULL_HANDLE)
    volkLoadInstance(instance);

  VkDebugUtilsMessengerEXT debug_messenger;
  if (vkCreateDebugUtilsMessengerEXT(instance, &utils_messenger_create_info,
                                     nullptr, &debug_messenger) != VK_SUCCESS)
    debug_messenger = VK_NULL_HANDLE;

  return new GFXInstance(instance, debug_messenger);
}

// static
void GetInstanceFeatures(WGPUSupportedInstanceFeatures* features) {
  features->featureCount =
      static_cast<uint32_t>(kSupportedInstanceFeatures.size());
  features->features = kSupportedInstanceFeatures.data();
}

// static
WGPUStatus GetInstanceLimits(WGPUInstanceLimits* limits) {
  if (limits->nextInChain) {
    GFX_ERROR() << "Unsupported chained struct in GetInstanceLimits.";
    return WGPUStatus_Error;
  }

  limits->timedWaitAnyMaxCount = std::numeric_limits<size_t>::max();

  return WGPUStatus_Success;
}

// static
WGPUBool HasInstanceFeature(WGPUInstanceFeatureName feature) {
  return std::find(kSupportedInstanceFeatures.begin(),
                   kSupportedInstanceFeatures.end(),
                   feature) != kSupportedInstanceFeatures.end();
}

///////////////////////////////////////////////////////////////////////////////
// Free Functions

// static
void AdapterInfoFreeMembers(WGPUAdapterInfo adapterInfo) {
  if (adapterInfo.nextInChain)
    return;

  FreeStringView(adapterInfo.vendor);
  FreeStringView(adapterInfo.architecture);
  FreeStringView(adapterInfo.device);
  FreeStringView(adapterInfo.description);
}

// static
void SupportedFeaturesFreeMembers(WGPUSupportedFeatures supportedFeatures) {
  if (!supportedFeatures.features)
    return;

  delete[] supportedFeatures.features;
}

// static
void SupportedInstanceFeaturesFreeMembers(
    WGPUSupportedInstanceFeatures supportedInstanceFeatures) {
  // No dynamic members to free
}

// static
void SupportedWGSLLanguageFeaturesFreeMembers(
    WGPUSupportedWGSLLanguageFeatures supportedWGSLLanguageFeatures) {
  // TODO: unsupport now
}

// static
void SurfaceCapabilitiesFreeMembers(
    WGPUSurfaceCapabilities surfaceCapabilities) {}

}  // namespace vkgfx

///////////////////////////////////////////////////////////////////////////////
// WGPU Exported Functions

GFX_EXPORT WGPUInstance GFX_FUNCTION(CreateInstance)(
    WGPU_NULLABLE WGPUInstanceDescriptor const* descriptor) {
  return vkgfx::CreateInstance(descriptor);
}

GFX_EXPORT void GFX_FUNCTION(GetInstanceFeatures)(
    WGPUSupportedInstanceFeatures* features) {
  vkgfx::GetInstanceFeatures(features);
}

GFX_EXPORT WGPUStatus
GFX_FUNCTION(GetInstanceLimits)(WGPUInstanceLimits* limits) {
  return vkgfx::GetInstanceLimits(limits);
}

GFX_EXPORT WGPUBool
GFX_FUNCTION(HasInstanceFeature)(WGPUInstanceFeatureName feature) {
  return vkgfx::HasInstanceFeature(feature);
}

GFX_EXPORT WGPUProc GFX_FUNCTION(GetProcAddress)(WGPUStringView procName) {
  // TODO: dynamic lookup functions
  return nullptr;
}

GFX_EXPORT
void GFX_FUNCTION(AdapterInfoFreeMembers)(WGPUAdapterInfo adapterInfo) {
  vkgfx::AdapterInfoFreeMembers(adapterInfo);
}

GFX_EXPORT
void GFX_FUNCTION(SupportedFeaturesFreeMembers)(
    WGPUSupportedFeatures supportedFeatures) {
  vkgfx::SupportedFeaturesFreeMembers(supportedFeatures);
}

GFX_EXPORT
void GFX_FUNCTION(SupportedInstanceFeaturesFreeMembers)(
    WGPUSupportedInstanceFeatures supportedInstanceFeatures) {
  vkgfx::SupportedInstanceFeaturesFreeMembers(supportedInstanceFeatures);
}

GFX_EXPORT
void GFX_FUNCTION(SupportedWGSLLanguageFeaturesFreeMembers)(
    WGPUSupportedWGSLLanguageFeatures supportedWGSLLanguageFeatures) {
  vkgfx::SupportedWGSLLanguageFeaturesFreeMembers(
      supportedWGSLLanguageFeatures);
}

GFX_EXPORT
void GFX_FUNCTION(SurfaceCapabilitiesFreeMembers)(
    WGPUSurfaceCapabilities surfaceCapabilities) {
  vkgfx::SurfaceCapabilitiesFreeMembers(surfaceCapabilities);
}
