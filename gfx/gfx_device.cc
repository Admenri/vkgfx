// Copyright 2025 Admenri.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#include "gfx/gfx_device.h"

namespace vkgfx {

///////////////////////////////////////////////////////////////////////////////
// GFXDevice Implement

GFXDevice::GFXDevice(VkDevice device) : device_(device) {}

GFXDevice::~GFXDevice() {}

WGPUBindGroup GFXDevice::CreateBindGroup(
    WGPUBindGroupDescriptor const* descriptor) {
  return WGPUBindGroup();
}

WGPUBindGroupLayout GFXDevice::CreateBindGroupLayout(
    WGPUBindGroupLayoutDescriptor const* descriptor) {
  return WGPUBindGroupLayout();
}

WGPUBuffer GFXDevice::CreateBuffer(WGPUBufferDescriptor const* descriptor) {
  return WGPUBuffer();
}

WGPUCommandEncoder GFXDevice::CreateCommandEncoder(
    WGPUCommandEncoderDescriptor const* descriptor) {
  return WGPUCommandEncoder();
}

WGPUComputePipeline GFXDevice::CreateComputePipeline(
    WGPUComputePipelineDescriptor const* descriptor) {
  return WGPUComputePipeline();
}

WGPUFuture GFXDevice::CreateComputePipelineAsync(
    WGPUComputePipelineDescriptor const* descriptor,
    WGPUCreateComputePipelineAsyncCallbackInfo callbackInfo) {
  return WGPUFuture();
}

WGPUPipelineLayout GFXDevice::CreatePipelineLayout(
    WGPUPipelineLayoutDescriptor const* descriptor) {
  return WGPUPipelineLayout();
}

WGPUQuerySet GFXDevice::CreateQuerySet(
    WGPUQuerySetDescriptor const* descriptor) {
  return WGPUQuerySet();
}

WGPURenderBundleEncoder GFXDevice::CreateRenderBundleEncoder(
    WGPURenderBundleEncoderDescriptor const* descriptor) {
  return WGPURenderBundleEncoder();
}

WGPURenderPipeline GFXDevice::CreateRenderPipeline(
    WGPURenderPipelineDescriptor const* descriptor) {
  return WGPURenderPipeline();
}

WGPUFuture GFXDevice::CreateRenderPipelineAsync(
    WGPURenderPipelineDescriptor const* descriptor,
    WGPUCreateRenderPipelineAsyncCallbackInfo callbackInfo) {
  return WGPUFuture();
}

WGPUSampler GFXDevice::CreateSampler(WGPUSamplerDescriptor const* descriptor) {
  return WGPUSampler();
}

WGPUShaderModule GFXDevice::CreateShaderModule(
    WGPUShaderModuleDescriptor const* descriptor) {
  return WGPUShaderModule();
}

WGPUTexture GFXDevice::CreateTexture(WGPUTextureDescriptor const* descriptor) {
  return WGPUTexture();
}

void GFXDevice::Destroy() {}

WGPUStatus GFXDevice::GetAdapterInfo(WGPUAdapterInfo* adapterInfo) {
  return WGPUStatus();
}

void GFXDevice::GetFeatures(WGPUSupportedFeatures* features) {}

WGPUStatus GFXDevice::GetLimits(WGPULimits* limits) {
  return WGPUStatus();
}

WGPUFuture GFXDevice::GetLostFuture() {
  return WGPUFuture();
}

WGPUQueue GFXDevice::GetQueue() {
  return WGPUQueue();
}

WGPUBool GFXDevice::HasFeature(WGPUFeatureName feature) {
  return WGPUBool();
}

WGPUFuture GFXDevice::PopErrorScope(
    WGPUPopErrorScopeCallbackInfo callbackInfo) {
  return WGPUFuture();
}

void GFXDevice::PushErrorScope(WGPUErrorFilter filter) {}

void GFXDevice::SetLabel(WGPUStringView label) {}

}  // namespace vkgfx
