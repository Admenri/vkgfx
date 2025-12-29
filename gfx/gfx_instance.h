// Copyright 2025 Admenri.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#ifndef GFX_GFX_INSTANCE_H_
#define GFX_GFX_INSTANCE_H_

#include <limits>
#include <utility>

#include "gfx/common/refptr.h"
#include "gfx/gfx_config.h"

struct WGPUInstanceImpl {};

namespace vkgfx {

// https://gpuweb.github.io/gpuweb/#gpu
class GFXInstance : public RefCounted<GFXInstance>, public WGPUInstanceImpl {
 public:
  static constexpr WGPUFuture kInvalidFuture = {0};
  static constexpr WGPUFuture kImmediateFuture = {
      std::numeric_limits<uint64_t>::max(),
  };

  GFXInstance(VkInstance instance, VkDebugUtilsMessengerEXT debug_messenger);
  ~GFXInstance();

  GFXInstance(const GFXInstance&) = delete;
  GFXInstance& operator=(const GFXInstance&) = delete;

  VkInstance GetVkHandle() const { return instance_; }

  WGPUSurface CreateSurface(WGPUSurfaceDescriptor const* descriptor);
  void GetWGSLLanguageFeatures(WGPUSupportedWGSLLanguageFeatures* features);
  WGPUBool HasWGSLLanguageFeature(WGPUWGSLLanguageFeatureName feature);
  void ProcessEvents();
  WGPUFuture RequestAdapter(WGPURequestAdapterOptions const* options,
                            WGPURequestAdapterCallbackInfo callbackInfo);
  WGPUWaitStatus WaitAny(size_t futureCount,
                         WGPUFutureWaitInfo* futures,
                         uint64_t timeoutNS);

 private:
  VkInstance instance_;

  VkDebugUtilsMessengerEXT debug_messenger_;
};

}  // namespace vkgfx

#endif  //! GFX_GFX_INSTANCE_H_