// Copyright 2025 Admenri.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#ifndef GFX_GFX_ADAPTER_H_
#define GFX_GFX_ADAPTER_H_

#include <array>
#include <span>
#include <vector>

#include "gfx/common/refptr.h"
#include "gfx/gfx_config.h"
#include "gfx/gfx_instance.h"

struct WGPUAdapterImpl {};

namespace vkgfx {

// https://gpuweb.github.io/gpuweb/#gpuadapter
class GFXAdapter : public RefCounted<GFXAdapter>, public WGPUAdapterImpl {
 public:
  enum DeviceExtension {
    kSubgroupSizeControl,             // promoted to 1.3
    kShaderFloat16Int8,               // promoted to 1.2
    kShaderSubgroupExtendedTypes,     // promoted to 1.2
    kShaderDemoteToHelperInvocation,  // promoted to 1.3
    kShaderIntegerDotProduct,         // promoted to 1.3
    kSwapchain,                       // never promoted
    kDepthClipEnable,                 // never promoted
    kExtensionNums,
  };

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

  struct DeviceInfo : public DeviceProperties, public DeviceFeatures {};

  GFXAdapter(VkPhysicalDevice adapter, RefPtr<GFXInstance> instance);
  ~GFXAdapter();

  GFXAdapter(const GFXAdapter&) = delete;
  GFXAdapter& operator=(const GFXAdapter&) = delete;

  VkPhysicalDevice GetVkHandle() const { return adapter_; }

  RefPtr<GFXInstance> GetInstance() const { return instance_; }
  const DeviceInfo& GetDeviceInfo() const { return device_info_; }

 public:
  void GetFeatures(WGPUSupportedFeatures* features);
  WGPUStatus GetInfo(WGPUAdapterInfo* info);
  WGPUStatus GetLimits(WGPULimits* limits);
  WGPUBool HasFeature(WGPUFeatureName feature);
  WGPUFuture RequestDevice(WGPUDeviceDescriptor const* descriptor,
                           WGPURequestDeviceCallbackInfo callbackInfo);

 private:
  void ConfigureAdapterInternal();
  std::vector<WGPUFeatureName> GetAdapterFeatures();

  VkPhysicalDevice adapter_;
  std::array<VkBool32, kExtensionNums> extensions_{false};

  RefPtr<GFXInstance> instance_;
  DeviceInfo device_info_;
};

}  // namespace vkgfx

#endif  // GFX_GFX_ADAPTER_H_
