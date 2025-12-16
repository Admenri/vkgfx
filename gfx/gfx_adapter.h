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
struct GFXAdapter : public RefCounted<GFXAdapter>, public WGPUAdapterImpl {
 public:
  ~GFXAdapter();

  GFXAdapter(const GFXAdapter&) = delete;
  GFXAdapter& operator=(const GFXAdapter&) = delete;

  enum DeviceExtension {
    kSubgroupSizeControl,  // promoted to 1.3
    kShaderFloat16Int8,    // promoted to 1.2
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

  VkPhysicalDevice adapter_;

  struct DeviceProperties {
    // Core 1.1
    VkPhysicalDeviceProperties2 properties_;
    VkPhysicalDeviceSubgroupProperties subgroup_properties_;

    // VK_EXT_subgroup_size_control
    VkPhysicalDeviceSubgroupSizeControlProperties
        subgroup_size_control_properties_;
  };

  struct DeviceInfo : public DeviceProperties {
    // Core 1.1
    VkPhysicalDeviceFeatures2 features_;
    VkPhysicalDevice16BitStorageFeatures shader_16bit_storage_features_;

    // VK_KHR_shader_float16_int8
    VkPhysicalDeviceShaderFloat16Int8Features shader_float16_int8_features_;
    // VK_EXT_subgroup_size_control
    VkPhysicalDeviceSubgroupSizeControlFeatures subgroup_size_control_features_;
  } device_info_;

  std::array<VkBool32, kExtensionNums> extensions_{false};
};

}  // namespace vkgfx

#endif  // !GFX_GFX_ADAPTER_H_
