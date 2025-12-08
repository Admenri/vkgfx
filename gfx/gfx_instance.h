// Copyright 2025 Admenri.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#ifndef GFX_GFX_INSTANCE_H_
#define GFX_GFX_INSTANCE_H_

#include <limits>
#include <utility>

#include "gfx/backend_config.h"
#include "gfx/common/refptr.h"

struct WGPUInstanceImpl {};

namespace vkgfx {

// https://gpuweb.github.io/gpuweb/#gpu
struct GFXInstance : public RefCounted<GFXInstance>, public WGPUInstanceImpl {
 public:
  static constexpr WGPUFuture kInvalidFuture = {0};

  static constexpr WGPUFuture kImmediateFuture = {
      std::numeric_limits<uint64_t>::max(),
  };

  ~GFXInstance();

  GFXInstance(const GFXInstance&) = delete;
  GFXInstance& operator=(const GFXInstance&) = delete;

  static RefPtr<GFXInstance> Create(
      WGPU_NULLABLE WGPUInstanceDescriptor const* descriptor);
  static void GetFeatures(WGPUSupportedInstanceFeatures* features);
  static WGPUStatus GetLimits(WGPUInstanceLimits* limits);
  static WGPUBool HasFeature(WGPUInstanceFeatureName feature);

  static void SupportedInstanceFeaturesFreeMembers(
      WGPUSupportedInstanceFeatures supportedInstanceFeatures);
  static void SupportedWGSLLanguageFeaturesFreeMembers(
      WGPUSupportedWGSLLanguageFeatures supportedWGSLLanguageFeatures);

  WGPUSurface CreateSurface(WGPUSurfaceDescriptor const* descriptor);
  void GetWGSLLanguageFeatures(WGPUSupportedWGSLLanguageFeatures* features);
  WGPUBool HasWGSLLanguageFeature(WGPUWGSLLanguageFeatureName feature);
  void ProcessEvents();
  WGPUFuture RequestAdapter(
      WGPU_NULLABLE WGPURequestAdapterOptions const* options,
      WGPURequestAdapterCallbackInfo callbackInfo);
  WGPUWaitStatus WaitAny(size_t futureCount,
                         WGPU_NULLABLE WGPUFutureWaitInfo* futures,
                         uint64_t timeoutNS);

 private:
  GFXInstance(VkInstance instance, VkDebugUtilsMessengerEXT debug_messenger);

  VkInstance instance_;
  VkDebugUtilsMessengerEXT debug_messenger_;
};

}  // namespace vkgfx

#endif  //! GFX_GFX_INSTANCE_H_