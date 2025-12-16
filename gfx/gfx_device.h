// Copyright 2025 Admenri.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#ifndef GFX_GFX_DEVICE_H_
#define GFX_GFX_DEVICE_H_

#include "gfx/backend_config.h"
#include "gfx/common/refptr.h"

struct WGPUDeviceImpl {};

namespace vkgfx {

// https://gpuweb.github.io/gpuweb/#device
class GFXDevice : public RefCounted<GFXDevice>, public WGPUDeviceImpl {
 public:
  ~GFXDevice();

  GFXDevice(const GFXDevice&) = delete;
  GFXDevice& operator=(const GFXDevice&) = delete;

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
  GFXDevice(VkDevice device);

  VkDevice device_;
};

}  // namespace vkgfx

#endif  // !GFX_GFX_DEVICE_H_
