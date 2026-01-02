// Copyright 2025 Admenri.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#ifndef GFX_GFX_DEVICE_H_
#define GFX_GFX_DEVICE_H_

#include <string>

#include "gfx/common/refptr.h"
#include "gfx/gfx_adapter.h"
#include "gfx/gfx_config.h"

#include "vma/vma.h"

struct WGPUDeviceImpl {};

namespace vkgfx {

// https://gpuweb.github.io/gpuweb/#gpudevice
class GFXDevice : public RefCounted<GFXDevice>, public WGPUDeviceImpl {
 public:
  GFXDevice(VkDevice device,
            RefPtr<GFXAdapter> adapter,
            const std::string& label,
            WGPUDeviceLostCallbackInfo device_lost_callback,
            WGPUUncapturedErrorCallbackInfo uncaptured_error_callback);
  ~GFXDevice();

  GFXDevice(const GFXDevice&) = delete;
  GFXDevice& operator=(const GFXDevice&) = delete;

  VkDevice GetVkHandle() const { return device_; }

  void CallDeviceLostCallback(WGPUDeviceLostReason reason,
                              const std::string& message);
  void CallDeviceErrorCallback(WGPUErrorType type, const std::string& message);

 public:
  WGPUBindGroup CreateBindGroup(WGPUBindGroupDescriptor const* descriptor);
  WGPUBindGroupLayout CreateBindGroupLayout(
      WGPUBindGroupLayoutDescriptor const* descriptor);
  WGPUBuffer CreateBuffer(WGPUBufferDescriptor const* descriptor);
  WGPUCommandEncoder CreateCommandEncoder(
      WGPUCommandEncoderDescriptor const* descriptor);
  WGPUComputePipeline CreateComputePipeline(
      WGPUComputePipelineDescriptor const* descriptor);
  WGPUFuture CreateComputePipelineAsync(
      WGPUComputePipelineDescriptor const* descriptor,
      WGPUCreateComputePipelineAsyncCallbackInfo callbackInfo);
  WGPUPipelineLayout CreatePipelineLayout(
      WGPUPipelineLayoutDescriptor const* descriptor);
  WGPUQuerySet CreateQuerySet(WGPUQuerySetDescriptor const* descriptor);
  WGPURenderBundleEncoder CreateRenderBundleEncoder(
      WGPURenderBundleEncoderDescriptor const* descriptor);
  WGPURenderPipeline CreateRenderPipeline(
      WGPURenderPipelineDescriptor const* descriptor);
  WGPUFuture CreateRenderPipelineAsync(
      WGPURenderPipelineDescriptor const* descriptor,
      WGPUCreateRenderPipelineAsyncCallbackInfo callbackInfo);
  WGPUSampler CreateSampler(WGPUSamplerDescriptor const* descriptor);
  WGPUShaderModule CreateShaderModule(
      WGPUShaderModuleDescriptor const* descriptor);
  WGPUTexture CreateTexture(WGPUTextureDescriptor const* descriptor);
  void Destroy();
  WGPUStatus GetAdapterInfo(WGPUAdapterInfo* adapterInfo);
  void GetFeatures(WGPUSupportedFeatures* features);
  WGPUStatus GetLimits(WGPULimits* limits);
  WGPUFuture GetLostFuture();
  WGPUQueue GetQueue();
  WGPUBool HasFeature(WGPUFeatureName feature);
  WGPUFuture PopErrorScope(WGPUPopErrorScopeCallbackInfo callbackInfo);
  void PushErrorScope(WGPUErrorFilter filter);
  void SetLabel(WGPUStringView label);

 private:
  void DestroyInternal();
  void CreateAllocatorInternal();

  VkDevice device_;

  RefPtr<GFXAdapter> adapter_;
  VmaAllocator allocator_;

  std::string label_;
  WGPUDeviceLostCallbackInfo device_lost_callback_;
  WGPUUncapturedErrorCallbackInfo uncaptured_error_callback_;
};

}  // namespace vkgfx

#endif  // !GFX_GFX_DEVICE_H_
