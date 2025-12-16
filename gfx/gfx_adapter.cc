// Copyright 2025 Admenri.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#include "gfx/gfx_adapter.h"

#include <array>

#include "gfx/common/constants.h"
#include "gfx/common/log.h"
#include "gfx/gfx_utils.h"

namespace vkgfx {

using DeviceExtInfo = struct {
  GFXAdapter::DeviceExtension ext;
  const char* name;
};

static constexpr std::array<DeviceExtInfo, GFXAdapter::kExtensionNums>
    kDeviceExtensions{
        DeviceExtInfo{GFXAdapter::kSubgroupSizeControl,
                      VK_EXT_SUBGROUP_SIZE_CONTROL_EXTENSION_NAME},
        DeviceExtInfo{GFXAdapter::kShaderFloat16Int8,
                      VK_KHR_SHADER_FLOAT16_INT8_EXTENSION_NAME},
    };

///////////////////////////////////////////////////////////////////////////////
// GFXAdapter Implement

GFXAdapter::GFXAdapter(VkPhysicalDevice adapter) : adapter_(adapter) {
  ConfigureSupportedExtensions();
}

void GFXAdapter::ConfigureSupportedExtensions() {
  // Extensions
  uint32_t extension_count = 0;
  vkEnumerateDeviceExtensionProperties(adapter_, nullptr, &extension_count,
                                       nullptr);
  std::vector<VkExtensionProperties> available_extensions(extension_count);
  vkEnumerateDeviceExtensionProperties(adapter_, nullptr, &extension_count,
                                       available_extensions.data());

  for (const auto& ext : available_extensions) {
    auto iter = std::find(kDeviceExtensions.begin(), kDeviceExtensions.end(),
                          [&](const DeviceExtInfo& it) {
                            return !std::strcmp(it.name, ext.extensionName);
                          });

    if (iter != kDeviceExtensions.end())
      extensions_[static_cast<size_t>(iter->ext)] = VK_TRUE;
  }

  // Properties
  device_info_.properties_ = {VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2};
  device_info_.subgroup_properties_ = {
      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SUBGROUP_PROPERTIES};
  device_info_.subgroup_size_control_properties_ = {
      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SUBGROUP_SIZE_CONTROL_PROPERTIES};

  NextChainBuilder properties_chain_builder(&device_info_.properties_);
  properties_chain_builder.Add(&device_info_.subgroup_properties_);

  if (extensions_[DeviceExtension::kSubgroupSizeControl])
    properties_chain_builder.Add(
        &device_info_.subgroup_size_control_properties_);

  vkGetPhysicalDeviceProperties2(adapter_, &device_info_.properties_);

  // Features
  device_info_.features_ = {VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2};
  device_info_.shader_float16_int8_features_ = {
      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_FLOAT16_INT8_FEATURES};
  device_info_.shader_16bit_storage_features_ = {
      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_16BIT_STORAGE_FEATURES};
  device_info_.subgroup_size_control_features_ = {
      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SUBGROUP_SIZE_CONTROL_FEATURES};

  NextChainBuilder features_chain_builder(&device_info_.features_);
  features_chain_builder.Add(&device_info_.shader_16bit_storage_features_);

  if (extensions_[DeviceExtension::kShaderFloat16Int8])
    features_chain_builder.Add(&device_info_.shader_float16_int8_features_);

  if (extensions_[DeviceExtension::kSubgroupSizeControl])
    features_chain_builder.Add(&device_info_.subgroup_size_control_features_);

  vkGetPhysicalDeviceFeatures2(adapter_, &device_info_.features_);
}

GFXAdapter::~GFXAdapter() = default;

void GFXAdapter::GetFeatures(WGPUSupportedFeatures* features) {
  if (!features) {
    GFX_ERROR() << "GFXAdapter::GetFeatures: features is null.";
    return;
  }

  const auto feature_names = GetAdapterFeatures();
  WGPUFeatureName* out_features = new WGPUFeatureName[feature_names.size()];
  std::memcpy(out_features, feature_names.data(),
              sizeof(WGPUFeatureName) * feature_names.size());

  features->featureCount = static_cast<uint32_t>(feature_names.size());
  features->features = out_features;
}

WGPUStatus GFXAdapter::GetInfo(WGPUAdapterInfo* info) {
  if (!info) {
    GFX_ERROR() << "GFXAdapter::GetInfo: info is null.";
    return WGPUStatus_Error;
  }

  if (info->nextInChain) {
    GFX_ERROR() << "GFXAdapter::GetInfo: nextInChain is not null.";
    return WGPUStatus_Error;
  }

  info->vendor = MakeStringView("GPU Vendor");
  info->architecture = MakeStringView("GPU Architecture");
  info->device = MakeStringView(device_info_.properties_.properties.deviceName);
  info->description = MakeStringView("GPU Description");

  info->backendType = WGPUBackendType_Vulkan;

  switch (device_info_.properties_.properties.deviceType) {
    case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU:
      info->adapterType = WGPUAdapterType_IntegratedGPU;
      break;
    case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU:
      info->adapterType = WGPUAdapterType_DiscreteGPU;
      break;
    case VK_PHYSICAL_DEVICE_TYPE_CPU:
      info->adapterType = WGPUAdapterType_CPU;
      break;
    default:
      info->adapterType = WGPUAdapterType_Unknown;
      break;
  }

  info->vendorID = device_info_.properties_.properties.vendorID;
  info->deviceID = device_info_.properties_.properties.deviceID;

  if (extensions_[kSubgroupSizeControl]) {
    info->subgroupMinSize =
        device_info_.subgroup_size_control_properties_.minSubgroupSize;
    info->subgroupMaxSize =
        device_info_.subgroup_size_control_properties_.maxSubgroupSize;
  } else {
    info->subgroupMinSize = device_info_.subgroup_properties_.subgroupSize;
    info->subgroupMaxSize = device_info_.subgroup_properties_.subgroupSize;
  }

  return WGPUStatus_Success;
}

WGPUStatus GFXAdapter::GetLimits(WGPULimits* limits) {
  if (!limits) {
    GFX_ERROR() << "GFXAdapter::GetLimits: limits is null.";
    return WGPUStatus_Error;
  }

  if (limits->nextInChain) {
    GFX_ERROR() << "GFXAdapter::GetLimits: nextInChain is not null.";
    return WGPUStatus_Error;
  }

  VkPhysicalDeviceProperties device_properties =
      device_info_.properties_.properties;

  limits->maxTextureDimension1D = device_properties.limits.maxImageDimension1D;
  limits->maxTextureDimension2D = device_properties.limits.maxImageDimension2D;
  limits->maxTextureDimension3D = device_properties.limits.maxImageDimension3D;
  limits->maxTextureArrayLayers = device_properties.limits.maxImageArrayLayers;

  limits->maxBindGroups = device_properties.limits.maxBoundDescriptorSets;
  limits->maxBindGroupsPlusVertexBuffers =
      device_properties.limits.maxBoundDescriptorSets +
      device_properties.limits.maxVertexInputBindings;
  limits->maxBindingsPerBindGroup =
      device_properties.limits.maxDescriptorSetUniformBuffers;
  limits->maxDynamicUniformBuffersPerPipelineLayout =
      device_properties.limits.maxDescriptorSetUniformBuffersDynamic;
  limits->maxDynamicStorageBuffersPerPipelineLayout =
      device_properties.limits.maxDescriptorSetStorageBuffersDynamic;
  limits->maxSampledTexturesPerShaderStage =
      device_properties.limits.maxPerStageDescriptorSampledImages;
  limits->maxSamplersPerShaderStage =
      device_properties.limits.maxPerStageDescriptorSamplers;
  limits->maxStorageBuffersPerShaderStage =
      device_properties.limits.maxPerStageDescriptorStorageBuffers;
  limits->maxStorageTexturesPerShaderStage =
      device_properties.limits.maxPerStageDescriptorStorageImages;
  limits->maxUniformBuffersPerShaderStage =
      device_properties.limits.maxPerStageDescriptorUniformBuffers;
  limits->maxUniformBufferBindingSize =
      device_properties.limits.maxUniformBufferRange;
  limits->maxStorageBufferBindingSize =
      device_properties.limits.maxStorageBufferRange;
  limits->minUniformBufferOffsetAlignment =
      device_properties.limits.minUniformBufferOffsetAlignment;
  limits->minStorageBufferOffsetAlignment =
      device_properties.limits.minStorageBufferOffsetAlignment;
  limits->maxVertexBuffers = device_properties.limits.maxVertexInputBindings;
  limits->maxBufferSize = kAssumedMaxBufferSize;
  limits->maxVertexAttributes =
      device_properties.limits.maxVertexInputAttributes;
  limits->maxVertexBufferArrayStride =
      device_properties.limits.maxVertexInputBindingStride;
  limits->maxInterStageShaderVariables =
      device_properties.limits.maxFragmentInputComponents;
  limits->maxColorAttachments = device_properties.limits.maxColorAttachments;
  limits->maxColorAttachmentBytesPerSample =
      32 * device_properties.limits.maxColorAttachments;

  limits->maxComputeWorkgroupStorageSize =
      device_properties.limits.maxComputeSharedMemorySize;
  limits->maxComputeInvocationsPerWorkgroup =
      device_properties.limits.maxComputeWorkGroupInvocations;
  limits->maxComputeWorkgroupSizeX =
      device_properties.limits.maxComputeWorkGroupSize[0];
  limits->maxComputeWorkgroupSizeY =
      device_properties.limits.maxComputeWorkGroupSize[1];
  limits->maxComputeWorkgroupSizeZ =
      device_properties.limits.maxComputeWorkGroupSize[2];
  limits->maxComputeWorkgroupsPerDimension = std::min({
      device_properties.limits.maxComputeWorkGroupCount[0],
      device_properties.limits.maxComputeWorkGroupCount[1],
      device_properties.limits.maxComputeWorkGroupCount[2],
  });
  limits->maxImmediateSize = kMaxImmediateDataBytes;

  return WGPUStatus_Success;
}

WGPUBool GFXAdapter::HasFeature(WGPUFeatureName feature) {
  return WGPUBool();
}

WGPUFuture GFXAdapter::RequestDevice(
    WGPU_NULLABLE WGPUDeviceDescriptor const* descriptor,
    WGPURequestDeviceCallbackInfo callbackInfo) {
  return WGPUFuture();
}

std::vector<WGPUFeatureName> GFXAdapter::GetAdapterFeatures() {
  VkFormatProperties format_properties;
  std::vector<WGPUFeatureName> feature_names;

  // WGPUFeatureName_CoreFeaturesAndLimits
  feature_names.push_back(WGPUFeatureName_CoreFeaturesAndLimits);

  // WGPUFeatureName_DepthClipControl
  if (device_info_.features_.features.depthClamp) {
    feature_names.push_back(WGPUFeatureName_DepthClipControl);
  }

  // WGPUFeatureName_Depth32FloatStencil8
  vkGetPhysicalDeviceFormatProperties(adapter_, VK_FORMAT_D32_SFLOAT_S8_UINT,
                                      &format_properties);
  if (format_properties.optimalTilingFeatures &
      VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT) {
    feature_names.push_back(WGPUFeatureName_Depth32FloatStencil8);
  }

  // WGPUFeatureName_TextureCompressionBC
  // WGPUFeatureName_TextureCompressionBCSliced3D
  if (device_info_.features_.features.textureCompressionBC) {
    feature_names.push_back(WGPUFeatureName_TextureCompressionBC);
    feature_names.push_back(WGPUFeatureName_TextureCompressionBCSliced3D);
  }

  // WGPUFeatureName_TextureCompressionETC2
  if (device_info_.features_.features.textureCompressionETC2) {
    feature_names.push_back(WGPUFeatureName_TextureCompressionETC2);
  }

  // WGPUFeatureName_TextureCompressionASTC
  // WGPUFeatureName_TextureCompressionASTCSliced3D
  if (device_info_.features_.features.textureCompressionASTC_LDR) {
    feature_names.push_back(WGPUFeatureName_TextureCompressionASTC);

    auto is_ASTC_sliced3d_supported = [&](VkFormat format) {
      VkImageFormatProperties properties;
      VkResult result = vkGetPhysicalDeviceImageFormatProperties(
          adapter_, format, VK_IMAGE_TYPE_3D, VK_IMAGE_TILING_OPTIMAL,
          VK_IMAGE_USAGE_SAMPLED_BIT, {}, &properties);
      return (result == VK_SUCCESS);
    };

    bool texture_compression_ASTC_sliced3d_supported = true;
    for (const auto& astcFormat :
         {VK_FORMAT_ASTC_4x4_UNORM_BLOCK,   VK_FORMAT_ASTC_4x4_SRGB_BLOCK,
          VK_FORMAT_ASTC_5x4_UNORM_BLOCK,   VK_FORMAT_ASTC_5x4_SRGB_BLOCK,
          VK_FORMAT_ASTC_5x5_UNORM_BLOCK,   VK_FORMAT_ASTC_5x5_SRGB_BLOCK,
          VK_FORMAT_ASTC_6x5_UNORM_BLOCK,   VK_FORMAT_ASTC_6x5_SRGB_BLOCK,
          VK_FORMAT_ASTC_6x6_UNORM_BLOCK,   VK_FORMAT_ASTC_6x6_SRGB_BLOCK,
          VK_FORMAT_ASTC_8x5_UNORM_BLOCK,   VK_FORMAT_ASTC_8x5_SRGB_BLOCK,
          VK_FORMAT_ASTC_8x6_UNORM_BLOCK,   VK_FORMAT_ASTC_8x6_SRGB_BLOCK,
          VK_FORMAT_ASTC_8x8_UNORM_BLOCK,   VK_FORMAT_ASTC_8x8_SRGB_BLOCK,
          VK_FORMAT_ASTC_10x5_UNORM_BLOCK,  VK_FORMAT_ASTC_10x5_SRGB_BLOCK,
          VK_FORMAT_ASTC_10x6_UNORM_BLOCK,  VK_FORMAT_ASTC_10x6_SRGB_BLOCK,
          VK_FORMAT_ASTC_10x8_UNORM_BLOCK,  VK_FORMAT_ASTC_10x8_SRGB_BLOCK,
          VK_FORMAT_ASTC_10x10_UNORM_BLOCK, VK_FORMAT_ASTC_10x10_SRGB_BLOCK,
          VK_FORMAT_ASTC_12x10_UNORM_BLOCK, VK_FORMAT_ASTC_12x10_SRGB_BLOCK,
          VK_FORMAT_ASTC_12x12_UNORM_BLOCK, VK_FORMAT_ASTC_12x12_SRGB_BLOCK}) {
      texture_compression_ASTC_sliced3d_supported &=
          is_ASTC_sliced3d_supported(astcFormat);
    }

    if (texture_compression_ASTC_sliced3d_supported)
      feature_names.push_back(WGPUFeatureName_TextureCompressionASTCSliced3D);
  }

  // WGPUFeatureName_TimestampQuery
  if (device_info_.properties_.properties.limits.timestampComputeAndGraphics) {
    feature_names.push_back(WGPUFeatureName_TimestampQuery);
  }

  // WGPUFeatureName_IndirectFirstInstance
  if (device_info_.features_.features.drawIndirectFirstInstance) {
    feature_names.push_back(WGPUFeatureName_IndirectFirstInstance);
  }

  // WGPUFeatureName_ShaderF16
  if (extensions_[DeviceExtension::kShaderFloat16Int8] &&
      device_info_.shader_16bit_storage_features_.storageBuffer16BitAccess &&
      device_info_.shader_float16_int8_features_.shaderFloat16) {
    feature_names.push_back(WGPUFeatureName_ShaderF16);
  }

  // WGPUFeatureName_RG11B10UfloatRenderable
  vkGetPhysicalDeviceFormatProperties(
      adapter_, VK_FORMAT_B10G11R11_UFLOAT_PACK32, &format_properties);
  if (format_properties.optimalTilingFeatures &
      VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT) {
    feature_names.push_back(WGPUFeatureName_RG11B10UfloatRenderable);
  }

  // WGPUFeatureName_BGRA8UnormStorage
  vkGetPhysicalDeviceFormatProperties(adapter_, VK_FORMAT_B8G8R8A8_UNORM,
                                      &format_properties);
  if (format_properties.optimalTilingFeatures &
      VK_FORMAT_FEATURE_STORAGE_IMAGE_BIT) {
    feature_names.push_back(WGPUFeatureName_BGRA8UnormStorage);
  }

  // WGPUFeatureName_Float32Filterable
  vkGetPhysicalDeviceFormatProperties(adapter_, VK_FORMAT_R32_SFLOAT,
                                      &format_properties);
  if (format_properties.optimalTilingFeatures &
      VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT) {
    feature_names.push_back(WGPUFeatureName_Float32Filterable);
  }

  // WGPUFeatureName_Float32Blendable
  vkGetPhysicalDeviceFormatProperties(adapter_, VK_FORMAT_R32G32B32A32_SFLOAT,
                                      &format_properties);
  if (format_properties.optimalTilingFeatures &
      VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT) {
    feature_names.push_back(WGPUFeatureName_Float32Blendable);
  }

  // WGPUFeatureName_ClipDistances
  if (device_info_.features_.features.shaderClipDistance) {
    feature_names.push_back(WGPUFeatureName_ClipDistances);
  }

  // WGPUFeatureName_DualSourceBlending
  if (device_info_.features_.features.dualSrcBlend) {
    feature_names.push_back(WGPUFeatureName_DualSourceBlending);
  }

  // WGPUFeatureName_Subgroups
  if ((device_info_.subgroup_properties_.supportedStages &
       VK_SHADER_STAGE_COMPUTE_BIT) &&
      (device_info_.subgroup_properties_.supportedStages &
       VK_SHADER_STAGE_FRAGMENT_BIT) &&
      (device_info_.subgroup_properties_.supportedOperations &
       VK_SUBGROUP_FEATURE_BASIC_BIT) &&
      (device_info_.subgroup_properties_.supportedOperations &
       VK_SUBGROUP_FEATURE_BALLOT_BIT) &&
      (device_info_.subgroup_properties_.supportedOperations &
       VK_SUBGROUP_FEATURE_SHUFFLE_BIT) &&
      (device_info_.subgroup_properties_.supportedOperations &
       VK_SUBGROUP_FEATURE_SHUFFLE_RELATIVE_BIT) &&
      (device_info_.subgroup_properties_.supportedOperations &
       VK_SUBGROUP_FEATURE_ARITHMETIC_BIT) &&
      (device_info_.subgroup_properties_.supportedOperations &
       VK_SUBGROUP_FEATURE_QUAD_BIT) &&
      extensions_[kSubgroupSizeControl] &&
      device_info_.subgroup_size_control_features_.subgroupSizeControl) {
    feature_names.push_back(WGPUFeatureName_Subgroups);
  }

  // WGPUFeatureName_TextureFormatsTier1
  // WGPUFeatureName_TextureFormatsTier2
  if (device_info_.features_.features.shaderStorageImageExtendedFormats) {
    bool support_tier1_and_tier2 = true;
    for (const auto& format :
         {VK_FORMAT_R16_UNORM, VK_FORMAT_R16_SNORM, VK_FORMAT_R16G16_UNORM,
          VK_FORMAT_R16G16_SNORM, VK_FORMAT_R16G16B16A16_UNORM,
          VK_FORMAT_R16G16B16A16_SNORM, VK_FORMAT_R8_SNORM,
          VK_FORMAT_R8G8_SNORM, VK_FORMAT_R8G8B8A8_SNORM,
          VK_FORMAT_B10G11R11_UFLOAT_PACK32}) {
      VkFormatProperties properties;
      vkGetPhysicalDeviceFormatProperties(adapter_, format, &properties);
      if ((VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BIT |
           VK_FORMAT_FEATURE_COLOR_ATTACHMENT_BLEND_BIT) &
          properties.optimalTilingFeatures) {
        support_tier1_and_tier2 = false;
        break;
      }
    }

    // We cannot enable TextureFormatsTier2 if TextureFormatsTier1 is not
    // enabled. TextureFormatsTier2 only requires
    // shaderStorageImageExtendedFormats from Vulkan so we don't have to check
    // anything else.
    if (support_tier1_and_tier2) {
      feature_names.push_back(WGPUFeatureName_TextureFormatsTier1);
      feature_names.push_back(WGPUFeatureName_TextureFormatsTier2);
    }
  }

  // WGPUFeatureName_PrimitiveIndex
  if (device_info_.features_.features.geometryShader) {
    // TODO: could also potentially use tessellation or mesh shaders?
    feature_names.push_back(WGPUFeatureName_PrimitiveIndex);
  }

  // WGPUFeatureName_TextureComponentSwizzle
  feature_names.push_back(
      WGPUFeatureName_TextureComponentSwizzle);  // Vk 1.0 Core

  return feature_names;
}

}  // namespace vkgfx

///////////////////////////////////////////////////////////////////////////////
// Member Function

GFX_REFCOUNTED_EXPORT(Adapter, vkgfx::GFXAdapter);

GFX_EXPORT void GFX_FUNCTION(
    AdapterGetFeatures)(WGPUAdapter adapter, WGPUSupportedFeatures* features) {
  auto* self = static_cast<vkgfx::GFXAdapter*>(adapter);
  self->GetFeatures(features);
}

GFX_EXPORT WGPUStatus GFX_FUNCTION(AdapterGetInfo)(WGPUAdapter adapter,
                                                   WGPUAdapterInfo* info) {
  auto* self = static_cast<vkgfx::GFXAdapter*>(adapter);
  return self->GetInfo(info);
}

GFX_EXPORT WGPUStatus GFX_FUNCTION(AdapterGetLimits)(WGPUAdapter adapter,
                                                     WGPULimits* limits) {
  auto* self = static_cast<vkgfx::GFXAdapter*>(adapter);
  return self->GetLimits(limits);
}

GFX_EXPORT WGPUBool GFX_FUNCTION(AdapterHasFeature)(WGPUAdapter adapter,
                                                    WGPUFeatureName feature) {
  auto* self = static_cast<vkgfx::GFXAdapter*>(adapter);
  return self->HasFeature(feature);
}

GFX_EXPORT WGPUFuture GFX_FUNCTION(AdapterRequestDevice)(
    WGPUAdapter adapter,
    WGPU_NULLABLE WGPUDeviceDescriptor const* descriptor,
    WGPURequestDeviceCallbackInfo callbackInfo) {
  auto* self = static_cast<vkgfx::GFXAdapter*>(adapter);
  return self->RequestDevice(descriptor, callbackInfo);
}
