// Copyright 2025 Admenri.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#include "gfx/gfx_utils.h"

namespace vkgfx {

VkSampleCountFlagBits ToVulkanSampleCount(uint32_t samples) {
  switch (samples) {
    case 2:
      return VK_SAMPLE_COUNT_2_BIT;
    case 4:
      return VK_SAMPLE_COUNT_4_BIT;
    case 8:
      return VK_SAMPLE_COUNT_8_BIT;
    case 16:
      return VK_SAMPLE_COUNT_16_BIT;
    case 32:
      return VK_SAMPLE_COUNT_32_BIT;
    case 64:
      return VK_SAMPLE_COUNT_64_BIT;
    default:
      return VK_SAMPLE_COUNT_1_BIT;
  }
}

VkImageUsageFlags ToVulkanImageUsage(WGPUTextureUsage usage,
                                     WGPUTextureFormat format) {
  VkImageUsageFlags flags = 0;

  auto is_depth_format = [](WGPUTextureFormat format) {
    return format == WGPUTextureFormat_Depth16Unorm ||
           format == WGPUTextureFormat_Depth24Plus ||
           format == WGPUTextureFormat_Depth24PlusStencil8 ||
           format == WGPUTextureFormat_Depth32Float ||
           format == WGPUTextureFormat_Depth32FloatStencil8;
  };

  if (usage & WGPUTextureUsage_CopySrc) {
    flags |= VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
  }
  if (usage & WGPUTextureUsage_CopyDst) {
    flags |= VK_IMAGE_USAGE_TRANSFER_DST_BIT;
  }
  if (usage & WGPUTextureUsage_TextureBinding) {
    flags |= VK_IMAGE_USAGE_SAMPLED_BIT;
    // If the sampled texture is a depth/stencil texture, its image layout will
    // be set to DEPTH_STENCIL_READ_ONLY_OPTIMAL in order to support readonly
    // depth/stencil attachment. That layout requires
    // DEPTH_STENCIL_ATTACHMENT_BIT image usage.
    if (is_depth_format(format)) {
      flags |= VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
    }
  }
  if (usage & WGPUTextureUsage_StorageBinding) {
    flags |= VK_IMAGE_USAGE_STORAGE_BIT;
  }
  if (usage & WGPUTextureUsage_RenderAttachment) {
    if (is_depth_format(format)) {
      flags |= VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
    } else {
      flags |= VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    }
  }

  return flags;
}

VkCompareOp ToVulkanCompareOp(WGPUCompareFunction op) {
  switch (op) {
    case WGPUCompareFunction_Never:
      return VK_COMPARE_OP_NEVER;
    case WGPUCompareFunction_Less:
      return VK_COMPARE_OP_LESS;
    case WGPUCompareFunction_LessEqual:
      return VK_COMPARE_OP_LESS_OR_EQUAL;
    case WGPUCompareFunction_Greater:
      return VK_COMPARE_OP_GREATER;
    case WGPUCompareFunction_GreaterEqual:
      return VK_COMPARE_OP_GREATER_OR_EQUAL;
    case WGPUCompareFunction_Equal:
      return VK_COMPARE_OP_EQUAL;
    case WGPUCompareFunction_NotEqual:
      return VK_COMPARE_OP_NOT_EQUAL;
    case WGPUCompareFunction_Always:
      return VK_COMPARE_OP_ALWAYS;

    case WGPUCompareFunction_Undefined:
      break;
  }
  return VkCompareOp();
}

VkSamplerAddressMode ToVulkanSamplerAddressMode(WGPUAddressMode mode) {
  switch (mode) {
    case WGPUAddressMode_Repeat:
      return VK_SAMPLER_ADDRESS_MODE_REPEAT;
    case WGPUAddressMode_MirrorRepeat:
      return VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT;
    case WGPUAddressMode_ClampToEdge:
      return VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
    case WGPUAddressMode_Undefined:
      break;
  }
  return VkSamplerAddressMode();
}

VkFilter ToVulkanSamplerFilter(WGPUFilterMode filter) {
  switch (filter) {
    case WGPUFilterMode_Linear:
      return VK_FILTER_LINEAR;
    case WGPUFilterMode_Nearest:
      return VK_FILTER_NEAREST;
    case WGPUFilterMode_Undefined:
      break;
  }
  return VkFilter();
}

VkSamplerMipmapMode ToVulkanMipMapMode(WGPUMipmapFilterMode filter) {
  switch (filter) {
    case WGPUMipmapFilterMode_Linear:
      return VK_SAMPLER_MIPMAP_MODE_LINEAR;
    case WGPUMipmapFilterMode_Nearest:
      return VK_SAMPLER_MIPMAP_MODE_NEAREST;
    case WGPUMipmapFilterMode_Undefined:
      break;
  }
  return VkSamplerMipmapMode();
}

VkDescriptorType ToVulkanDescriptorType(const WGPUBindGroupLayoutEntry& entry) {
  if (entry.buffer.type != WGPUBufferBindingType_BindingNotUsed) {
    switch (entry.buffer.type) {
      case WGPUBufferBindingType_Uniform:
        if (entry.buffer.hasDynamicOffset)
          return VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
        return VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
      case WGPUBufferBindingType_Storage:
      case WGPUBufferBindingType_ReadOnlyStorage:
        if (entry.buffer.hasDynamicOffset)
          return VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC;
        return VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
      default:
        return VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
    }
  }
  if (entry.sampler.type != WGPUSamplerBindingType_BindingNotUsed) {
    return VK_DESCRIPTOR_TYPE_SAMPLER;
  }
  if (entry.texture.sampleType != WGPUTextureSampleType_BindingNotUsed) {
    return VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
  }
  if (entry.storageTexture.access != WGPUStorageTextureAccess_BindingNotUsed) {
    return VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
  }
  return VkDescriptorType();
}

VkShaderStageFlags ToVulkanShaderStages(WGPUShaderStage stages) {
  VkShaderStageFlags flags = 0;

  if (stages & WGPUShaderStage_Vertex) {
    flags |= VK_SHADER_STAGE_VERTEX_BIT;
  }
  if (stages & WGPUShaderStage_Fragment) {
    flags |= VK_SHADER_STAGE_FRAGMENT_BIT;
  }
  if (stages & WGPUShaderStage_Compute) {
    flags |= VK_SHADER_STAGE_COMPUTE_BIT;
  }

  return flags;
}

VkBufferUsageFlags ToVulkanBufferUsage(WGPUBufferUsage usage) {
  VkBufferUsageFlags flags = 0;

  if (usage & WGPUBufferUsage_CopySrc)
    flags |= VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
  if (usage & WGPUBufferUsage_CopyDst)
    flags |= VK_BUFFER_USAGE_TRANSFER_DST_BIT;
  if (usage & WGPUBufferUsage_Index)
    flags |= VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
  if (usage & WGPUBufferUsage_Vertex)
    flags |= VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
  if (usage & WGPUBufferUsage_Uniform)
    flags |= VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
  if (usage & WGPUBufferUsage_Storage)
    flags |= VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;
  if (usage & WGPUBufferUsage_Indirect)
    flags |= VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT;
  if (usage & WGPUBufferUsage_QueryResolve)
    flags |= VK_BUFFER_USAGE_TRANSFER_DST_BIT;

  return flags;
}

VkFormat ToVulkanPixelFormat(WGPUTextureFormat format) {
  switch (format) {
    case WGPUTextureFormat_Undefined:
      return VK_FORMAT_UNDEFINED;
    case WGPUTextureFormat_R8Unorm:
      return VK_FORMAT_R8_UNORM;
    case WGPUTextureFormat_R8Snorm:
      return VK_FORMAT_R8_SNORM;
    case WGPUTextureFormat_R8Uint:
      return VK_FORMAT_R8_UINT;
    case WGPUTextureFormat_R8Sint:
      return VK_FORMAT_R8_SINT;
    case WGPUTextureFormat_R16Uint:
      return VK_FORMAT_R16_UINT;
    case WGPUTextureFormat_R16Sint:
      return VK_FORMAT_R16_SINT;
    case WGPUTextureFormat_R16Unorm:
      return VK_FORMAT_R16_UNORM;
    case WGPUTextureFormat_R16Snorm:
      return VK_FORMAT_R16_SNORM;
    case WGPUTextureFormat_R16Float:
      return VK_FORMAT_R16_SFLOAT;
    case WGPUTextureFormat_RG8Unorm:
      return VK_FORMAT_R8G8_UNORM;
    case WGPUTextureFormat_RG8Snorm:
      return VK_FORMAT_R8G8_SNORM;
    case WGPUTextureFormat_RG8Uint:
      return VK_FORMAT_R8G8_UINT;
    case WGPUTextureFormat_RG8Sint:
      return VK_FORMAT_R8G8_SINT;
    case WGPUTextureFormat_R32Float:
      return VK_FORMAT_R32_SFLOAT;
    case WGPUTextureFormat_R32Uint:
      return VK_FORMAT_R32_UINT;
    case WGPUTextureFormat_R32Sint:
      return VK_FORMAT_R32_SINT;
    case WGPUTextureFormat_RG16Uint:
      return VK_FORMAT_R16G16_UINT;
    case WGPUTextureFormat_RG16Sint:
      return VK_FORMAT_R16G16_SINT;
    case WGPUTextureFormat_RG16Unorm:
      return VK_FORMAT_R16G16_UNORM;
    case WGPUTextureFormat_RG16Snorm:
      return VK_FORMAT_R16G16_SNORM;
    case WGPUTextureFormat_RG16Float:
      return VK_FORMAT_R16G16_SFLOAT;
    case WGPUTextureFormat_RGBA8Unorm:
      return VK_FORMAT_R8G8B8A8_UNORM;
    case WGPUTextureFormat_RGBA8UnormSrgb:
      return VK_FORMAT_R8G8B8A8_SRGB;
    case WGPUTextureFormat_RGBA8Snorm:
      return VK_FORMAT_R8G8B8A8_SNORM;
    case WGPUTextureFormat_RGBA8Uint:
      return VK_FORMAT_R8G8B8A8_UINT;
    case WGPUTextureFormat_RGBA8Sint:
      return VK_FORMAT_R8G8B8A8_SINT;
    case WGPUTextureFormat_BGRA8Unorm:
      return VK_FORMAT_B8G8R8A8_UNORM;
    case WGPUTextureFormat_BGRA8UnormSrgb:
      return VK_FORMAT_B8G8R8A8_SRGB;
    case WGPUTextureFormat_RGB10A2Uint:
      // VK_FORMAT_A2B10G10R10_UINT_PACK32
      return VK_FORMAT_A2R10G10B10_UINT_PACK32;
    case WGPUTextureFormat_RGB10A2Unorm:
      // VK_FORMAT_A2B10G10R10_UNORM_PACK32
      return VK_FORMAT_A2R10G10B10_UNORM_PACK32;
    case WGPUTextureFormat_RG11B10Ufloat:
      return VK_FORMAT_B10G11R11_UFLOAT_PACK32;
    case WGPUTextureFormat_RGB9E5Ufloat:
      return VK_FORMAT_E5B9G9R9_UFLOAT_PACK32;
    case WGPUTextureFormat_RG32Float:
      return VK_FORMAT_R32G32_SFLOAT;
    case WGPUTextureFormat_RG32Uint:
      return VK_FORMAT_R32G32_UINT;
    case WGPUTextureFormat_RG32Sint:
      return VK_FORMAT_R32G32_SINT;
    case WGPUTextureFormat_RGBA16Uint:
      return VK_FORMAT_R16G16B16A16_UINT;
    case WGPUTextureFormat_RGBA16Sint:
      return VK_FORMAT_R16G16B16A16_SINT;
    case WGPUTextureFormat_RGBA16Unorm:
      return VK_FORMAT_R16G16B16A16_UNORM;
    case WGPUTextureFormat_RGBA16Snorm:
      return VK_FORMAT_R16G16B16A16_SNORM;
    case WGPUTextureFormat_RGBA16Float:
      return VK_FORMAT_R16G16B16A16_SFLOAT;
    case WGPUTextureFormat_RGBA32Float:
      return VK_FORMAT_R32G32B32A32_SFLOAT;
    case WGPUTextureFormat_RGBA32Uint:
      return VK_FORMAT_R32G32B32A32_UINT;
    case WGPUTextureFormat_RGBA32Sint:
      return VK_FORMAT_R32G32B32A32_SINT;
    case WGPUTextureFormat_Stencil8:
      return VK_FORMAT_S8_UINT;
    case WGPUTextureFormat_Depth16Unorm:
      return VK_FORMAT_D16_UNORM;
    case WGPUTextureFormat_Depth24Plus:
      // VK_FORMAT_D24_UNORM_S8_UINT
      return VK_FORMAT_X8_D24_UNORM_PACK32;
    case WGPUTextureFormat_Depth24PlusStencil8:
      return VK_FORMAT_D24_UNORM_S8_UINT;
    case WGPUTextureFormat_Depth32Float:
      return VK_FORMAT_D32_SFLOAT;
    case WGPUTextureFormat_Depth32FloatStencil8:
      return VK_FORMAT_D32_SFLOAT_S8_UINT;
    case WGPUTextureFormat_BC1RGBAUnorm:
      return VK_FORMAT_BC1_RGBA_UNORM_BLOCK;
    case WGPUTextureFormat_BC1RGBAUnormSrgb:
      return VK_FORMAT_BC1_RGBA_SRGB_BLOCK;
    case WGPUTextureFormat_BC2RGBAUnorm:
      return VK_FORMAT_BC2_UNORM_BLOCK;
    case WGPUTextureFormat_BC2RGBAUnormSrgb:
      return VK_FORMAT_BC2_SRGB_BLOCK;
    case WGPUTextureFormat_BC3RGBAUnorm:
      return VK_FORMAT_BC3_UNORM_BLOCK;
    case WGPUTextureFormat_BC3RGBAUnormSrgb:
      return VK_FORMAT_BC3_SRGB_BLOCK;
    case WGPUTextureFormat_BC4RUnorm:
      return VK_FORMAT_BC4_UNORM_BLOCK;
    case WGPUTextureFormat_BC4RSnorm:
      return VK_FORMAT_BC4_SNORM_BLOCK;
    case WGPUTextureFormat_BC5RGUnorm:
      return VK_FORMAT_BC5_UNORM_BLOCK;
    case WGPUTextureFormat_BC5RGSnorm:
      return VK_FORMAT_BC5_SNORM_BLOCK;
    case WGPUTextureFormat_BC6HRGBUfloat:
      return VK_FORMAT_BC6H_UFLOAT_BLOCK;
    case WGPUTextureFormat_BC6HRGBFloat:
      return VK_FORMAT_BC6H_SFLOAT_BLOCK;
    case WGPUTextureFormat_BC7RGBAUnorm:
      return VK_FORMAT_BC7_UNORM_BLOCK;
    case WGPUTextureFormat_BC7RGBAUnormSrgb:
      return VK_FORMAT_BC7_SRGB_BLOCK;
    case WGPUTextureFormat_ETC2RGB8Unorm:
      return VK_FORMAT_ETC2_R8G8B8_UNORM_BLOCK;
    case WGPUTextureFormat_ETC2RGB8UnormSrgb:
      return VK_FORMAT_ETC2_R8G8B8_SRGB_BLOCK;
    case WGPUTextureFormat_ETC2RGB8A1Unorm:
      return VK_FORMAT_ETC2_R8G8B8A1_UNORM_BLOCK;
    case WGPUTextureFormat_ETC2RGB8A1UnormSrgb:
      return VK_FORMAT_ETC2_R8G8B8A1_SRGB_BLOCK;
    case WGPUTextureFormat_ETC2RGBA8Unorm:
      return VK_FORMAT_ETC2_R8G8B8A8_UNORM_BLOCK;
    case WGPUTextureFormat_ETC2RGBA8UnormSrgb:
      return VK_FORMAT_ETC2_R8G8B8A8_SRGB_BLOCK;
    case WGPUTextureFormat_EACR11Unorm:
      return VK_FORMAT_EAC_R11_UNORM_BLOCK;
    case WGPUTextureFormat_EACR11Snorm:
      return VK_FORMAT_EAC_R11_SNORM_BLOCK;
    case WGPUTextureFormat_EACRG11Unorm:
      return VK_FORMAT_EAC_R11G11_UNORM_BLOCK;
    case WGPUTextureFormat_EACRG11Snorm:
      return VK_FORMAT_EAC_R11G11_SNORM_BLOCK;
    case WGPUTextureFormat_ASTC4x4Unorm:
      return VK_FORMAT_ASTC_4x4_UNORM_BLOCK;
    case WGPUTextureFormat_ASTC4x4UnormSrgb:
      return VK_FORMAT_ASTC_4x4_SRGB_BLOCK;
    case WGPUTextureFormat_ASTC5x4Unorm:
      return VK_FORMAT_ASTC_5x4_UNORM_BLOCK;
    case WGPUTextureFormat_ASTC5x4UnormSrgb:
      return VK_FORMAT_ASTC_5x4_SRGB_BLOCK;
    case WGPUTextureFormat_ASTC5x5Unorm:
      return VK_FORMAT_ASTC_5x5_UNORM_BLOCK;
    case WGPUTextureFormat_ASTC5x5UnormSrgb:
      return VK_FORMAT_ASTC_5x5_SRGB_BLOCK;
    case WGPUTextureFormat_ASTC6x5Unorm:
      return VK_FORMAT_ASTC_6x5_UNORM_BLOCK;
    case WGPUTextureFormat_ASTC6x5UnormSrgb:
      return VK_FORMAT_ASTC_6x5_SRGB_BLOCK;
    case WGPUTextureFormat_ASTC6x6Unorm:
      return VK_FORMAT_ASTC_6x6_UNORM_BLOCK;
    case WGPUTextureFormat_ASTC6x6UnormSrgb:
      return VK_FORMAT_ASTC_6x6_SRGB_BLOCK;
    case WGPUTextureFormat_ASTC8x5Unorm:
      return VK_FORMAT_ASTC_8x5_UNORM_BLOCK;
    case WGPUTextureFormat_ASTC8x5UnormSrgb:
      return VK_FORMAT_ASTC_8x5_SRGB_BLOCK;
    case WGPUTextureFormat_ASTC8x6Unorm:
      return VK_FORMAT_ASTC_8x6_UNORM_BLOCK;
    case WGPUTextureFormat_ASTC8x6UnormSrgb:
      return VK_FORMAT_ASTC_8x6_SRGB_BLOCK;
    case WGPUTextureFormat_ASTC8x8Unorm:
      return VK_FORMAT_ASTC_8x8_UNORM_BLOCK;
    case WGPUTextureFormat_ASTC8x8UnormSrgb:
      return VK_FORMAT_ASTC_8x8_SRGB_BLOCK;
    case WGPUTextureFormat_ASTC10x5Unorm:
      return VK_FORMAT_ASTC_10x5_UNORM_BLOCK;
    case WGPUTextureFormat_ASTC10x5UnormSrgb:
      return VK_FORMAT_ASTC_10x5_SRGB_BLOCK;
    case WGPUTextureFormat_ASTC10x6Unorm:
      return VK_FORMAT_ASTC_10x6_UNORM_BLOCK;
    case WGPUTextureFormat_ASTC10x6UnormSrgb:
      return VK_FORMAT_ASTC_10x6_SRGB_BLOCK;
    case WGPUTextureFormat_ASTC10x8Unorm:
      return VK_FORMAT_ASTC_10x8_UNORM_BLOCK;
    case WGPUTextureFormat_ASTC10x8UnormSrgb:
      return VK_FORMAT_ASTC_10x8_SRGB_BLOCK;
    case WGPUTextureFormat_ASTC10x10Unorm:
      return VK_FORMAT_ASTC_10x10_UNORM_BLOCK;
    case WGPUTextureFormat_ASTC10x10UnormSrgb:
      return VK_FORMAT_ASTC_10x10_SRGB_BLOCK;
    case WGPUTextureFormat_ASTC12x10Unorm:
      return VK_FORMAT_ASTC_12x10_UNORM_BLOCK;
    case WGPUTextureFormat_ASTC12x10UnormSrgb:
      return VK_FORMAT_ASTC_12x10_SRGB_BLOCK;
    case WGPUTextureFormat_ASTC12x12Unorm:
      return VK_FORMAT_ASTC_12x12_UNORM_BLOCK;
    case WGPUTextureFormat_ASTC12x12UnormSrgb:
      return VK_FORMAT_ASTC_12x12_SRGB_BLOCK;
    default:
      return VK_FORMAT_UNDEFINED;
  }
}

VkImageViewType ToVulkanTextureViewDimension(
    WGPUTextureViewDimension dimension) {
  switch (dimension) {
    default:
    case WGPUTextureViewDimension_1D:
      return VK_IMAGE_VIEW_TYPE_1D;
    case WGPUTextureViewDimension_2D:
      return VK_IMAGE_VIEW_TYPE_2D;
    case WGPUTextureViewDimension_3D:
      return VK_IMAGE_VIEW_TYPE_3D;
    case WGPUTextureViewDimension_2DArray:
      return VK_IMAGE_VIEW_TYPE_2D_ARRAY;
  }
}

}  // namespace vkgfx
