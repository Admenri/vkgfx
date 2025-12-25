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

///////////////////////////////////////////////////////////////////////////////
// Member Function

GFX_REFCOUNTED_EXPORT(Device, vkgfx::GFXDevice);

GFX_EXPORT WGPUBindGroup
GFX_FUNCTION(DeviceCreateBindGroup)(WGPUDevice device,
                                    WGPUBindGroupDescriptor const* descriptor) {
  auto* self = static_cast<vkgfx::GFXDevice*>(device);
  return self->CreateBindGroup(descriptor);
}

GFX_EXPORT WGPUBindGroupLayout GFX_FUNCTION(DeviceCreateBindGroupLayout)(
    WGPUDevice device,
    WGPUBindGroupLayoutDescriptor const* descriptor) {
  auto* self = static_cast<vkgfx::GFXDevice*>(device);
  return self->CreateBindGroupLayout(descriptor);
}

GFX_EXPORT WGPU_NULLABLE WGPUBuffer
GFX_FUNCTION(DeviceCreateBuffer)(WGPUDevice device,
                                 WGPUBufferDescriptor const* descriptor) {
  auto* self = static_cast<vkgfx::GFXDevice*>(device);
  return self->CreateBuffer(descriptor);
}

GFX_EXPORT WGPUCommandEncoder GFX_FUNCTION(DeviceCreateCommandEncoder)(
    WGPUDevice device,
    WGPU_NULLABLE WGPUCommandEncoderDescriptor const* descriptor) {
  auto* self = static_cast<vkgfx::GFXDevice*>(device);
  return self->CreateCommandEncoder(descriptor);
}

GFX_EXPORT WGPUComputePipeline GFX_FUNCTION(DeviceCreateComputePipeline)(
    WGPUDevice device,
    WGPUComputePipelineDescriptor const* descriptor) {
  auto* self = static_cast<vkgfx::GFXDevice*>(device);
  return self->CreateComputePipeline(descriptor);
}

GFX_EXPORT WGPUFuture GFX_FUNCTION(DeviceCreateComputePipelineAsync)(
    WGPUDevice device,
    WGPUComputePipelineDescriptor const* descriptor,
    WGPUCreateComputePipelineAsyncCallbackInfo callbackInfo) {
  auto* self = static_cast<vkgfx::GFXDevice*>(device);
  return self->CreateComputePipelineAsync(descriptor, callbackInfo);
}

GFX_EXPORT WGPUPipelineLayout GFX_FUNCTION(DeviceCreatePipelineLayout)(
    WGPUDevice device,
    WGPUPipelineLayoutDescriptor const* descriptor) {
  auto* self = static_cast<vkgfx::GFXDevice*>(device);
  return self->CreatePipelineLayout(descriptor);
}

GFX_EXPORT WGPUQuerySet
GFX_FUNCTION(DeviceCreateQuerySet)(WGPUDevice device,
                                   WGPUQuerySetDescriptor const* descriptor) {
  auto* self = static_cast<vkgfx::GFXDevice*>(device);
  return self->CreateQuerySet(descriptor);
}

GFX_EXPORT WGPURenderBundleEncoder
GFX_FUNCTION(DeviceCreateRenderBundleEncoder)(
    WGPUDevice device,
    WGPURenderBundleEncoderDescriptor const* descriptor) {
  auto* self = static_cast<vkgfx::GFXDevice*>(device);
  return self->CreateRenderBundleEncoder(descriptor);
}

GFX_EXPORT WGPURenderPipeline GFX_FUNCTION(DeviceCreateRenderPipeline)(
    WGPUDevice device,
    WGPURenderPipelineDescriptor const* descriptor) {
  auto* self = static_cast<vkgfx::GFXDevice*>(device);
  return self->CreateRenderPipeline(descriptor);
}

GFX_EXPORT WGPUFuture GFX_FUNCTION(DeviceCreateRenderPipelineAsync)(
    WGPUDevice device,
    WGPURenderPipelineDescriptor const* descriptor,
    WGPUCreateRenderPipelineAsyncCallbackInfo callbackInfo) {
  auto* self = static_cast<vkgfx::GFXDevice*>(device);
  return self->CreateRenderPipelineAsync(descriptor, callbackInfo);
}

GFX_EXPORT WGPUSampler GFX_FUNCTION(DeviceCreateSampler)(
    WGPUDevice device,
    WGPU_NULLABLE WGPUSamplerDescriptor const* descriptor) {
  auto* self = static_cast<vkgfx::GFXDevice*>(device);
  return self->CreateSampler(descriptor);
}

GFX_EXPORT WGPUShaderModule GFX_FUNCTION(DeviceCreateShaderModule)(
    WGPUDevice device,
    WGPUShaderModuleDescriptor const* descriptor) {
  auto* self = static_cast<vkgfx::GFXDevice*>(device);
  return self->CreateShaderModule(descriptor);
}

GFX_EXPORT WGPUTexture
GFX_FUNCTION(DeviceCreateTexture)(WGPUDevice device,
                                  WGPUTextureDescriptor const* descriptor) {
  auto* self = static_cast<vkgfx::GFXDevice*>(device);
  return self->CreateTexture(descriptor);
}

GFX_EXPORT void GFX_FUNCTION(DeviceDestroy)(WGPUDevice device) {
  auto* self = static_cast<vkgfx::GFXDevice*>(device);
  self->Destroy();
}

GFX_EXPORT WGPUStatus GFX_FUNCTION(
    DeviceGetAdapterInfo)(WGPUDevice device, WGPUAdapterInfo* adapterInfo) {
  auto* self = static_cast<vkgfx::GFXDevice*>(device);
  return self->GetAdapterInfo(adapterInfo);
}

GFX_EXPORT void GFX_FUNCTION(
    DeviceGetFeatures)(WGPUDevice device, WGPUSupportedFeatures* features) {
  auto* self = static_cast<vkgfx::GFXDevice*>(device);
  self->GetFeatures(features);
}

GFX_EXPORT WGPUStatus GFX_FUNCTION(DeviceGetLimits)(WGPUDevice device,
                                                    WGPULimits* limits) {
  auto* self = static_cast<vkgfx::GFXDevice*>(device);
  return self->GetLimits(limits);
}

GFX_EXPORT WGPUFuture GFX_FUNCTION(DeviceGetLostFuture)(WGPUDevice device) {
  auto* self = static_cast<vkgfx::GFXDevice*>(device);
  return self->GetLostFuture();
}

GFX_EXPORT WGPUQueue GFX_FUNCTION(DeviceGetQueue)(WGPUDevice device) {
  auto* self = static_cast<vkgfx::GFXDevice*>(device);
  return self->GetQueue();
}

GFX_EXPORT WGPUBool GFX_FUNCTION(DeviceHasFeature)(WGPUDevice device,
                                                   WGPUFeatureName feature) {
  auto* self = static_cast<vkgfx::GFXDevice*>(device);
  return self->HasFeature(feature);
}

GFX_EXPORT WGPUFuture
GFX_FUNCTION(DevicePopErrorScope)(WGPUDevice device,
                                  WGPUPopErrorScopeCallbackInfo callbackInfo) {
  auto* self = static_cast<vkgfx::GFXDevice*>(device);
  return self->PopErrorScope(callbackInfo);
}

GFX_EXPORT void GFX_FUNCTION(DevicePushErrorScope)(WGPUDevice device,
                                                   WGPUErrorFilter filter) {
  auto* self = static_cast<vkgfx::GFXDevice*>(device);
  self->PushErrorScope(filter);
}

GFX_EXPORT void GFX_FUNCTION(DeviceSetLabel)(WGPUDevice device,
                                             WGPUStringView label) {
  auto* self = static_cast<vkgfx::GFXDevice*>(device);
  self->SetLabel(label);
}

}  // namespace vkgfx
