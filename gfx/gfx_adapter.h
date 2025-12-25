// Copyright 2025 Admenri.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#ifndef GFX_GFX_ADAPTER_H_
#define GFX_GFX_ADAPTER_H_

#include <vector>

#include "gfx/backend_config.h"
#include "gfx/common/refptr.h"

struct WGPUAdapterImpl {};

namespace vkgfx {

// https://gpuweb.github.io/gpuweb/#adapter
class GFXAdapter : public RefCounted<GFXAdapter>, public WGPUAdapterImpl {
 public:
  ~GFXAdapter();

  GFXAdapter(const GFXAdapter&) = delete;
  GFXAdapter& operator=(const GFXAdapter&) = delete;

  enum DeviceExtension {
    kSubgroupSizeControl,             // promoted to 1.3
    kShaderFloat16Int8,               // promoted to 1.2
    kShaderSubgroupExtendedTypes,     // promoted to 1.2
    kShaderDemoteToHelperInvocation,  // promoted to 1.3
    kShaderIntegerDotProduct,         // promoted to 1.3
    kExtensionNums,
  };

  void GetFeatures(WGPUSupportedFeatures* features);
  WGPUStatus GetInfo(WGPUAdapterInfo* info);
  WGPUStatus GetLimits(WGPULimits* limits);
  WGPUBool HasFeature(WGPUFeatureName feature);
  WGPUFuture RequestDevice(WGPU_NULLABLE WGPUDeviceDescriptor const* descriptor,
                           WGPURequestDeviceCallbackInfo callbackInfo);

 private:
  friend class GFXInstance;
  GFXAdapter(VkPhysicalDevice adapter);

  void ConfigureSupportedExtensions();
  std::vector<WGPUFeatureName> GetAdapterFeatures();
  void GetDeviceQueueFamilies();

  VkPhysicalDevice adapter_;

  struct DeviceProperties {
    // Core 1.1
    VkPhysicalDeviceProperties2 properties;
    VkPhysicalDeviceSubgroupProperties subgroup_properties;

    // VK_EXT_subgroup_size_control
    VkPhysicalDeviceSubgroupSizeControlProperties
        subgroup_size_control_properties;
  };

  struct DeviceFeatures {
    // Core 1.1
    VkPhysicalDeviceFeatures2 features;
    VkPhysicalDevice16BitStorageFeatures shader_16bit_storage_features;

    // VK_KHR_shader_float16_int8
    VkPhysicalDeviceShaderFloat16Int8Features shader_float16_int8_features;
    // VK_EXT_subgroup_size_control
    VkPhysicalDeviceSubgroupSizeControlFeatures subgroup_size_control_features;
    // VK_KHR_shader_subgroup_extended_types
    VkPhysicalDeviceShaderSubgroupExtendedTypesFeaturesKHR
        shader_subgroup_extended_types_features;
    // VK_EXT_shader_demote_to_helper_invocation
    VkPhysicalDeviceShaderDemoteToHelperInvocationFeaturesEXT
        shader_demote_to_helper_invocation_features;
    // VK_KHR_shader_integer_dot_product
    VkPhysicalDeviceShaderIntegerDotProductFeaturesKHR
        shader_integer_dot_product_features;
  };

  struct DeviceInfo : public DeviceProperties, public DeviceFeatures {
  } device_info_;

  std::array<VkBool32, kExtensionNums> extensions_{false};
  std::vector<VkQueueFamilyProperties> queue_families_;
};

}  // namespace vkgfx

#endif  // !GFX_GFX_ADAPTER_H_
