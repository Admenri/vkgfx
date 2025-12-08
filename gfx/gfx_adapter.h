// Copyright 2025 Admenri.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#ifndef GFX_GFX_ADAPTER_H_
#define GFX_GFX_ADAPTER_H_

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

  static void AdapterInfoFreeMembers(WGPUAdapterInfo adapterInfo);
  static void SupportedFeaturesFreeMembers(
      WGPUSupportedFeatures supportedFeatures);

  void GetFeatures(WGPUSupportedFeatures* features);
  WGPUStatus GetInfo(WGPUAdapterInfo* info);
  WGPUStatus GetLimits(WGPULimits* limits);
  WGPUBool HasFeature(WGPUFeatureName feature);
  WGPUFuture RequestDevice(WGPU_NULLABLE WGPUDeviceDescriptor const* descriptor,
                           WGPURequestDeviceCallbackInfo callbackInfo);

 private:
  friend class GFXInstance;
  GFXAdapter(VkPhysicalDevice adapter);

  VkPhysicalDevice adapter_;
};

}  // namespace vkgfx

#endif  // !GFX_GFX_ADAPTER_H_
