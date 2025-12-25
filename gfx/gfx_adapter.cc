// Copyright 2025 Admenri.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#include "gfx/gfx_adapter.h"

#include <array>
#include <span>

#include "gfx/common/constants.h"
#include "gfx/common/log.h"
#include "gfx/gfx_device.h"
#include "gfx/gfx_instance.h"
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
        DeviceExtInfo{GFXAdapter::kShaderSubgroupExtendedTypes,
                      VK_KHR_SHADER_SUBGROUP_EXTENDED_TYPES_EXTENSION_NAME},
        DeviceExtInfo{GFXAdapter::kShaderDemoteToHelperInvocation,
                      VK_EXT_SHADER_DEMOTE_TO_HELPER_INVOCATION_EXTENSION_NAME},
        DeviceExtInfo{GFXAdapter::kShaderIntegerDotProduct,
                      VK_KHR_SHADER_INTEGER_DOT_PRODUCT_EXTENSION_NAME},
    };

///////////////////////////////////////////////////////////////////////////////
// GFXAdapter Implement

GFXAdapter::GFXAdapter(VkPhysicalDevice adapter) : adapter_(adapter) {
  ConfigureSupportedExtensions();
  GetDeviceQueueFamilies();
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
  device_info_.properties = {VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2};
  device_info_.subgroup_properties = {
      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SUBGROUP_PROPERTIES};
  device_info_.subgroup_size_control_properties = {
      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SUBGROUP_SIZE_CONTROL_PROPERTIES};

  NextChainBuilder properties_chain_builder(&device_info_.properties);
  properties_chain_builder.Add(&device_info_.subgroup_properties);

  if (extensions_[DeviceExtension::kSubgroupSizeControl])
    properties_chain_builder.Add(
        &device_info_.subgroup_size_control_properties);

  vkGetPhysicalDeviceProperties2(adapter_, &device_info_.properties);

  // Features
  device_info_.features = {VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2};
  device_info_.shader_float16_int8_features = {
      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_FLOAT16_INT8_FEATURES};
  device_info_.shader_16bit_storage_features = {
      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_16BIT_STORAGE_FEATURES};
  device_info_.subgroup_size_control_features = {
      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SUBGROUP_SIZE_CONTROL_FEATURES};

  NextChainBuilder features_chain_builder(&device_info_.features);
  features_chain_builder.Add(&device_info_.shader_16bit_storage_features);

  if (extensions_[DeviceExtension::kShaderFloat16Int8])
    features_chain_builder.Add(&device_info_.shader_float16_int8_features);

  if (extensions_[DeviceExtension::kSubgroupSizeControl])
    features_chain_builder.Add(&device_info_.subgroup_size_control_features);

  if (extensions_[DeviceExtension::kShaderSubgroupExtendedTypes])
    features_chain_builder.Add(
        &device_info_.shader_subgroup_extended_types_features);

  if (extensions_[DeviceExtension::kShaderDemoteToHelperInvocation])
    features_chain_builder.Add(
        &device_info_.shader_demote_to_helper_invocation_features);

  if (extensions_[DeviceExtension::kShaderIntegerDotProduct])
    features_chain_builder.Add(
        &device_info_.shader_integer_dot_product_features);

  vkGetPhysicalDeviceFeatures2(adapter_, &device_info_.features);
}

GFXAdapter::~GFXAdapter() = default;

void GFXAdapter::GetFeatures(WGPUSupportedFeatures* features) {
  if (!features) {
    GFX_ERROR() << __FUNCTION__ << ": features is null.";
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
    GFX_ERROR() << __FUNCTION__ << ": info is null.";
    return WGPUStatus_Error;
  }

  if (info->nextInChain) {
    GFX_ERROR() << __FUNCTION__ << ": nextInChain is not null.";
    return WGPUStatus_Error;
  }

  info->vendor = MakeStringView("GPU Vendor");
  info->architecture = MakeStringView("GPU Architecture");
  info->device = MakeStringView(device_info_.properties.properties.deviceName);
  info->description = MakeStringView("GPU Description");

  info->backendType = WGPUBackendType_Vulkan;

  switch (device_info_.properties.properties.deviceType) {
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

  info->vendorID = device_info_.properties.properties.vendorID;
  info->deviceID = device_info_.properties.properties.deviceID;

  if (extensions_[kSubgroupSizeControl]) {
    info->subgroupMinSize =
        device_info_.subgroup_size_control_properties.minSubgroupSize;
    info->subgroupMaxSize =
        device_info_.subgroup_size_control_properties.maxSubgroupSize;
  } else {
    info->subgroupMinSize = device_info_.subgroup_properties.subgroupSize;
    info->subgroupMaxSize = device_info_.subgroup_properties.subgroupSize;
  }

  return WGPUStatus_Success;
}

WGPUStatus GFXAdapter::GetLimits(WGPULimits* limits) {
  if (!limits) {
    GFX_ERROR() << __FUNCTION__ << ": limits is null.";
    return WGPUStatus_Error;
  }

  if (limits->nextInChain) {
    GFX_ERROR() << __FUNCTION__ << ": nextInChain is not null.";
    return WGPUStatus_Error;
  }

  VkPhysicalDeviceProperties device_properties =
      device_info_.properties.properties;

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
  const auto feature_names = GetAdapterFeatures();
  return std::find(feature_names.begin(), feature_names.end(), feature) !=
         feature_names.end();
}

WGPUFuture GFXAdapter::RequestDevice(
    WGPU_NULLABLE WGPUDeviceDescriptor const* descriptor,
    WGPURequestDeviceCallbackInfo callbackInfo) {
  if (!callbackInfo.callback) {
    GFX_ERROR() << __FUNCTION__ << ": callbackInfo.callback is null.";
    return GFXInstance::kInvalidFuture;
  }

  if (descriptor && descriptor->nextInChain) {
    GFX_ERROR() << __FUNCTION__ << ": descriptor->nextInChain is not null.";
    return GFXInstance::kInvalidFuture;
  }

  // Required limits
  if (descriptor && descriptor->requiredLimits) {
    WGPULimits supported_limits = {};
    GetLimits(&supported_limits);

#define CHECK_MAX_LIMIT(field)                                      \
  if (descriptor->requiredLimits->field > supported_limits.field) { \
    GFX_ERROR() << __FUNCTION__ << ": Required limit " #field       \
                << " not supported.";                               \
    return GFXInstance::kInvalidFuture;                             \
  }

#define CHECK_ALIGN_LIMIT(field)                                    \
  if (descriptor->requiredLimits->field > supported_limits.field) { \
    GFX_ERROR() << __FUNCTION__ << ": Required limit " #field       \
                << " not supported.";                               \
    return GFXInstance::kInvalidFuture;                             \
  }                                                                 \
  if ((descriptor->requiredLimits->field &                          \
       (descriptor->requiredLimits->field - 1)) != 0) {             \
    GFX_ERROR() << __FUNCTION__ << ": Required limit " #field       \
                << " must be a power of two.";                      \
    return GFXInstance::kInvalidFuture;                             \
  }

    CHECK_MAX_LIMIT(maxTextureDimension1D);
    CHECK_MAX_LIMIT(maxTextureDimension2D);
    CHECK_MAX_LIMIT(maxTextureDimension3D);
    CHECK_MAX_LIMIT(maxTextureArrayLayers);
    CHECK_MAX_LIMIT(maxBindGroups);
    CHECK_MAX_LIMIT(maxBindGroupsPlusVertexBuffers);
    CHECK_MAX_LIMIT(maxBindingsPerBindGroup);
    CHECK_MAX_LIMIT(maxDynamicUniformBuffersPerPipelineLayout);
    CHECK_MAX_LIMIT(maxDynamicStorageBuffersPerPipelineLayout);
    CHECK_MAX_LIMIT(maxSampledTexturesPerShaderStage);
    CHECK_MAX_LIMIT(maxSamplersPerShaderStage);
    CHECK_MAX_LIMIT(maxStorageBuffersPerShaderStage);
    CHECK_MAX_LIMIT(maxStorageTexturesPerShaderStage);
    CHECK_MAX_LIMIT(maxUniformBuffersPerShaderStage);
    CHECK_MAX_LIMIT(maxUniformBufferBindingSize);
    CHECK_MAX_LIMIT(maxStorageBufferBindingSize);
    CHECK_ALIGN_LIMIT(minUniformBufferOffsetAlignment);
    CHECK_ALIGN_LIMIT(minStorageBufferOffsetAlignment);
    CHECK_MAX_LIMIT(maxVertexBuffers);
    CHECK_MAX_LIMIT(maxBufferSize);
    CHECK_MAX_LIMIT(maxVertexAttributes);
    CHECK_MAX_LIMIT(maxVertexBufferArrayStride);
    CHECK_MAX_LIMIT(maxInterStageShaderVariables);
    CHECK_MAX_LIMIT(maxColorAttachments);
    CHECK_MAX_LIMIT(maxColorAttachmentBytesPerSample);
    CHECK_MAX_LIMIT(maxComputeWorkgroupStorageSize);
    CHECK_MAX_LIMIT(maxComputeInvocationsPerWorkgroup);
    CHECK_MAX_LIMIT(maxComputeWorkgroupSizeX);
    CHECK_MAX_LIMIT(maxComputeWorkgroupSizeY);
    CHECK_MAX_LIMIT(maxComputeWorkgroupSizeZ);
    CHECK_MAX_LIMIT(maxComputeWorkgroupsPerDimension);
    CHECK_MAX_LIMIT(maxImmediateSize);

#undef CHECK_MAX_LIMIT
#undef CHECK_ALIGN_LIMIT
  }

  // Required features
  DeviceFeatures features_knobs = {};
  VkPhysicalDeviceFeatures2 enabled_features = {
      VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2};
  NextChainBuilder features_chain(&enabled_features);

  if (descriptor && descriptor->requiredFeatures) {
    std::span<const WGPUFeatureName> required_features(
        descriptor->requiredFeatures, descriptor->requiredFeatureCount);

#define CHECK_FEATURE(name)                                     \
  std::find(required_features.begin(), required_features.end(), \
            WGPUFeatureName_##name) != required_features.end()

    // Required for core and compat WebGPU features.
    enabled_features.features.fullDrawIndexUint32 = VK_TRUE;
    enabled_features.features.fragmentStoresAndAtomics = VK_TRUE;
    enabled_features.features.shaderUniformBufferArrayDynamicIndexing = VK_TRUE;
    enabled_features.features.shaderStorageBufferArrayDynamicIndexing = VK_TRUE;
    enabled_features.features.shaderSampledImageArrayDynamicIndexing = VK_TRUE;
    enabled_features.features.shaderStorageImageArrayDynamicIndexing = VK_TRUE;

    if (device_info_.features.features.samplerAnisotropy)
      enabled_features.features.samplerAnisotropy = VK_TRUE;

    // Required for core WebGPU features.
    if (CHECK_FEATURE(CoreFeaturesAndLimits)) {
      enabled_features.features.depthBiasClamp = VK_TRUE;
      enabled_features.features.imageCubeArray = VK_TRUE;
      enabled_features.features.independentBlend = VK_TRUE;
      enabled_features.features.sampleRateShading = VK_TRUE;
    }

    if (CHECK_FEATURE(DepthClipControl)) {
      enabled_features.features.depthClamp = VK_TRUE;
    }

    if (CHECK_FEATURE(TextureCompressionBC)) {
      enabled_features.features.textureCompressionBC = VK_TRUE;
    }

    if (CHECK_FEATURE(TextureCompressionETC2)) {
      enabled_features.features.textureCompressionETC2 = VK_TRUE;
    }

    if (CHECK_FEATURE(TextureCompressionASTC)) {
      enabled_features.features.textureCompressionASTC_LDR = VK_TRUE;
    }

    if (CHECK_FEATURE(IndirectFirstInstance)) {
      enabled_features.features.drawIndirectFirstInstance = VK_TRUE;
    }

    if (CHECK_FEATURE(ShaderF16)) {
      features_knobs.shader_16bit_storage_features =
          device_info_.shader_16bit_storage_features;
      features_knobs.shader_float16_int8_features =
          device_info_.shader_float16_int8_features;
      features_chain.Add(&features_knobs.shader_16bit_storage_features);
      features_chain.Add(&features_knobs.shader_float16_int8_features);
    }

    if (CHECK_FEATURE(ClipDistances)) {
      enabled_features.features.shaderClipDistance = VK_TRUE;
    }

    if (CHECK_FEATURE(DualSourceBlending)) {
      enabled_features.features.dualSrcBlend = VK_TRUE;
    }

    if (CHECK_FEATURE(ShaderF16) && CHECK_FEATURE(Subgroups)) {
      features_knobs.shader_subgroup_extended_types_features =
          device_info_.shader_subgroup_extended_types_features;
      features_chain.Add(
          &features_knobs.shader_subgroup_extended_types_features);
    }

    if (CHECK_FEATURE(TextureFormatsTier1)) {
      enabled_features.features.shaderStorageImageExtendedFormats = VK_TRUE;
    }

    if (CHECK_FEATURE(PrimitiveIndex)) {
      enabled_features.features.geometryShader = VK_TRUE;
    }

    if (extensions_[kSubgroupSizeControl]) {
      features_knobs.subgroup_size_control_features =
          device_info_.subgroup_size_control_features;
      features_chain.Add(&features_knobs.subgroup_size_control_features);
    }

    if (extensions_[kShaderDemoteToHelperInvocation]) {
      features_knobs.shader_demote_to_helper_invocation_features =
          device_info_.shader_demote_to_helper_invocation_features;
      features_chain.Add(
          &features_knobs.shader_demote_to_helper_invocation_features);
    }

    if (extensions_[kShaderIntegerDotProduct]) {
      features_knobs.shader_integer_dot_product_features =
          device_info_.shader_integer_dot_product_features;
      features_chain.Add(&features_knobs.shader_integer_dot_product_features);
    }

#undef CHECK_FEATURE
  }

  // Required extensions
  std::vector<const char*> enabled_extension_names;
  for (const auto& ext : kDeviceExtensions)
    enabled_extension_names.push_back(ext.name);

  // Queue family select
  uint32_t main_queue_family = UINT32_MAX;
  constexpr uint32_t kUniversalFlags =
      VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_COMPUTE_BIT;
  for (size_t i = 0; i < queue_families_.size(); ++i) {
    if ((queue_families_[i].queueFlags & kUniversalFlags) == kUniversalFlags) {
      main_queue_family = static_cast<uint32_t>(i);
      break;
    }
  }

  if (main_queue_family == UINT32_MAX) {
    GFX_ERROR() << __FUNCTION__ << ": No suitable queue family.";
    return GFXInstance::kInvalidFuture;
  }

  // Queue family create info
  std::vector<VkDeviceQueueCreateInfo> queues_to_request;
  float zero = 0.0f;

  {
    VkDeviceQueueCreateInfo queue_create_info = {
        VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO};
    queue_create_info.queueFamilyIndex = main_queue_family;
    queue_create_info.queueCount = 1;
    queue_create_info.pQueuePriorities = &zero;

    queues_to_request.push_back(queue_create_info);
  }

  // Create device
  VkDeviceCreateInfo create_info = {VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO};
  create_info.pNext = &enabled_features;
  create_info.queueCreateInfoCount = queues_to_request.size();
  create_info.pQueueCreateInfos = queues_to_request.data();
  create_info.enabledExtensionCount = enabled_extension_names.size();
  create_info.ppEnabledExtensionNames = enabled_extension_names.data();

  VkDevice device;
  vkCreateDevice(adapter_, &create_info, nullptr, &device);

  GFXDevice* device_impl = nullptr;
  if (device)
    device_impl = new GFXDevice(device);

  WGPUStringView message = {};
  callbackInfo.callback(WGPURequestDeviceStatus_Success, device_impl, message,
                        callbackInfo.userdata1, callbackInfo.userdata2);

  return GFXInstance::kImmediateFuture;
}

std::vector<WGPUFeatureName> GFXAdapter::GetAdapterFeatures() {
  VkFormatProperties format_properties;
  std::vector<WGPUFeatureName> feature_names;

  // WGPUFeatureName_CoreFeaturesAndLimits
  feature_names.push_back(WGPUFeatureName_CoreFeaturesAndLimits);

  // WGPUFeatureName_DepthClipControl
  if (device_info_.features.features.depthClamp) {
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
  if (device_info_.features.features.textureCompressionBC) {
    feature_names.push_back(WGPUFeatureName_TextureCompressionBC);
    feature_names.push_back(WGPUFeatureName_TextureCompressionBCSliced3D);
  }

  // WGPUFeatureName_TextureCompressionETC2
  if (device_info_.features.features.textureCompressionETC2) {
    feature_names.push_back(WGPUFeatureName_TextureCompressionETC2);
  }

  // WGPUFeatureName_TextureCompressionASTC
  // WGPUFeatureName_TextureCompressionASTCSliced3D
  if (device_info_.features.features.textureCompressionASTC_LDR) {
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
  if (device_info_.properties.properties.limits.timestampComputeAndGraphics) {
    feature_names.push_back(WGPUFeatureName_TimestampQuery);
  }

  // WGPUFeatureName_IndirectFirstInstance
  if (device_info_.features.features.drawIndirectFirstInstance) {
    feature_names.push_back(WGPUFeatureName_IndirectFirstInstance);
  }

  // WGPUFeatureName_ShaderF16
  if (extensions_[DeviceExtension::kShaderFloat16Int8] &&
      device_info_.shader_16bit_storage_features.storageBuffer16BitAccess &&
      device_info_.shader_float16_int8_features.shaderFloat16) {
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
  if (device_info_.features.features.shaderClipDistance) {
    feature_names.push_back(WGPUFeatureName_ClipDistances);
  }

  // WGPUFeatureName_DualSourceBlending
  if (device_info_.features.features.dualSrcBlend) {
    feature_names.push_back(WGPUFeatureName_DualSourceBlending);
  }

  // WGPUFeatureName_Subgroups
  if ((device_info_.subgroup_properties.supportedStages &
       VK_SHADER_STAGE_COMPUTE_BIT) &&
      (device_info_.subgroup_properties.supportedStages &
       VK_SHADER_STAGE_FRAGMENT_BIT) &&
      (device_info_.subgroup_properties.supportedOperations &
       VK_SUBGROUP_FEATURE_BASIC_BIT) &&
      (device_info_.subgroup_properties.supportedOperations &
       VK_SUBGROUP_FEATURE_BALLOT_BIT) &&
      (device_info_.subgroup_properties.supportedOperations &
       VK_SUBGROUP_FEATURE_SHUFFLE_BIT) &&
      (device_info_.subgroup_properties.supportedOperations &
       VK_SUBGROUP_FEATURE_SHUFFLE_RELATIVE_BIT) &&
      (device_info_.subgroup_properties.supportedOperations &
       VK_SUBGROUP_FEATURE_ARITHMETIC_BIT) &&
      (device_info_.subgroup_properties.supportedOperations &
       VK_SUBGROUP_FEATURE_QUAD_BIT) &&
      extensions_[kSubgroupSizeControl] &&
      device_info_.subgroup_size_control_features.subgroupSizeControl) {
    feature_names.push_back(WGPUFeatureName_Subgroups);
  }

  // WGPUFeatureName_TextureFormatsTier1
  // WGPUFeatureName_TextureFormatsTier2
  if (device_info_.features.features.shaderStorageImageExtendedFormats) {
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
  if (device_info_.features.features.geometryShader) {
    // TODO: could also potentially use tessellation or mesh shaders?
    feature_names.push_back(WGPUFeatureName_PrimitiveIndex);
  }

  // WGPUFeatureName_TextureComponentSwizzle
  feature_names.push_back(
      WGPUFeatureName_TextureComponentSwizzle);  // Vk 1.0 Core

  return feature_names;
}

void GFXAdapter::GetDeviceQueueFamilies() {
  uint32_t queue_family_count = 0;
  vkGetPhysicalDeviceQueueFamilyProperties(adapter_, &queue_family_count,
                                           nullptr);

  queue_families_.resize(queue_family_count);
  vkGetPhysicalDeviceQueueFamilyProperties(adapter_, &queue_family_count,
                                           queue_families_.data());
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
