// Copyright 2025 Admenri.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#include "gfx/gfx_surface.h"

#include "gfx/common/log.h"
#include "gfx/gfx_adapter.h"
#include "gfx/gfx_instance.h"

namespace vkgfx {

///////////////////////////////////////////////////////////////////////////////
// GFXSurface Implement

GFXSurface::GFXSurface(VkSurfaceKHR surface,
                       RefPtr<GFXInstance> instance,
                       WGPUStringView label)
    : surface_(surface), instance_(instance) {
  if (label.data && label.length)
    label_ = std::string(label.data, label.length);
}

GFXSurface::~GFXSurface() {
  if (surface_ && instance_)
    vkDestroySurfaceKHR(instance_->GetVkHandle(), surface_, nullptr);
}

void GFXSurface::Configure(WGPUSurfaceConfiguration const* config) {}

WGPUStatus GFXSurface::GetCapabilities(WGPUAdapter adapter,
                                       WGPUSurfaceCapabilities* capabilities) {
  auto* adapter_impl = static_cast<GFXAdapter*>(adapter);
  if (!adapter_impl)
    return WGPUStatus_Error;

  if (!capabilities)
    return WGPUStatus_Error;

  // Capabilities
  VkSurfaceCapabilitiesKHR surface_capabilities;
  vkGetPhysicalDeviceSurfaceCapabilitiesKHR(adapter_impl->GetVkHandle(),
                                            surface_, &surface_capabilities);

  // Formats
  uint32_t format_count = 0;
  vkGetPhysicalDeviceSurfaceFormatsKHR(adapter_impl->GetVkHandle(), surface_,
                                       &format_count, nullptr);
  std::vector<VkSurfaceFormatKHR> surface_formats(format_count);
  vkGetPhysicalDeviceSurfaceFormatsKHR(adapter_impl->GetVkHandle(), surface_,
                                       &format_count, surface_formats.data());

  // PresentModes
  uint32_t present_mode_count = 0;
  vkGetPhysicalDeviceSurfacePresentModesKHR(
      adapter_impl->GetVkHandle(), surface_, &present_mode_count, nullptr);
  std::vector<VkPresentModeKHR> present_modes(present_mode_count);
  vkGetPhysicalDeviceSurfacePresentModesKHR(adapter_impl->GetVkHandle(),
                                            surface_, &present_mode_count,
                                            present_modes.data());

  // To WGPUSurfaceCapabilities::usages
  capabilities->usages = WGPUBufferUsage_None;
  if (surface_capabilities.supportedUsageFlags &
      VK_IMAGE_USAGE_TRANSFER_SRC_BIT)
    capabilities->usages |= WGPUTextureUsage_CopySrc;
  if (surface_capabilities.supportedUsageFlags &
      VK_IMAGE_USAGE_TRANSFER_DST_BIT)
    capabilities->usages |= WGPUTextureUsage_CopyDst;
  if (surface_capabilities.supportedUsageFlags & VK_IMAGE_USAGE_SAMPLED_BIT)
    capabilities->usages |= WGPUTextureUsage_TextureBinding;
  if (surface_capabilities.supportedUsageFlags & VK_IMAGE_USAGE_STORAGE_BIT)
    capabilities->usages |= WGPUTextureUsage_StorageBinding;
  if (surface_capabilities.supportedUsageFlags &
      VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT)
    capabilities->usages |= WGPUTextureUsage_RenderAttachment;

  // To WGPUSurfaceCapabilities::formats
  auto to_wgpu_surface_format = [](VkFormat format) -> WGPUTextureFormat {
    switch (format) {
      case VK_FORMAT_R8G8B8A8_UNORM:
        return WGPUTextureFormat_RGBA8Unorm;
      case VK_FORMAT_R8G8B8A8_SRGB:
        return WGPUTextureFormat_RGBA8UnormSrgb;
      case VK_FORMAT_B8G8R8A8_UNORM:
        return WGPUTextureFormat_BGRA8Unorm;
      case VK_FORMAT_B8G8R8A8_SRGB:
        return WGPUTextureFormat_BGRA8UnormSrgb;
      case VK_FORMAT_A2B10G10R10_UNORM_PACK32:
        return WGPUTextureFormat_RGB10A2Unorm;
      case VK_FORMAT_R16G16B16A16_SFLOAT:
        return WGPUTextureFormat_RGBA16Float;
      default:
        return WGPUTextureFormat_Undefined;
    }
  };

  WGPUTextureFormat* surface_formats_out = nullptr;
  if (!surface_formats.empty()) {
    surface_formats_out = new WGPUTextureFormat[surface_formats.size()];
    for (size_t i = 0; i < surface_formats.size(); ++i)
      surface_formats_out[i] =
          to_wgpu_surface_format(surface_formats[i].format);
  }

  capabilities->formatCount = surface_formats.size();
  capabilities->formats = surface_formats_out;

  // To WGPUSurfaceCapabilities::presentModes
  auto to_wgpu_present_mode = [](VkPresentModeKHR mode) -> WGPUPresentMode {
    switch (mode) {
      case VK_PRESENT_MODE_FIFO_KHR:
        return WGPUPresentMode_Fifo;
      case VK_PRESENT_MODE_FIFO_RELAXED_KHR:
        return WGPUPresentMode_FifoRelaxed;
      case VK_PRESENT_MODE_IMMEDIATE_KHR:
        return WGPUPresentMode_Immediate;
      case VK_PRESENT_MODE_MAILBOX_KHR:
        return WGPUPresentMode_Mailbox;
      default:
        return WGPUPresentMode_Undefined;
    }
  };

  WGPUPresentMode* present_modes_out = nullptr;
  if (!present_modes.empty()) {
    present_modes_out = new WGPUPresentMode[present_modes.size()];
    for (size_t i = 0; i < present_modes.size(); ++i)
      present_modes_out[i] = to_wgpu_present_mode(present_modes[i]);
  }

  capabilities->presentModeCount = present_modes.size();
  capabilities->presentModes = present_modes_out;

  // To WGPUSurfaceCapabilities::alphaModes
  std::vector<WGPUCompositeAlphaMode> alpha_modes;
  if (surface_capabilities.supportedCompositeAlpha &
      VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR)
    alpha_modes.push_back(WGPUCompositeAlphaMode_Opaque);
  if (surface_capabilities.supportedCompositeAlpha &
      VK_COMPOSITE_ALPHA_PRE_MULTIPLIED_BIT_KHR)
    alpha_modes.push_back(WGPUCompositeAlphaMode_Premultiplied);
  if (surface_capabilities.supportedCompositeAlpha &
      VK_COMPOSITE_ALPHA_POST_MULTIPLIED_BIT_KHR)
    alpha_modes.push_back(WGPUCompositeAlphaMode_Unpremultiplied);
  if (surface_capabilities.supportedCompositeAlpha &
      VK_COMPOSITE_ALPHA_INHERIT_BIT_KHR)
    alpha_modes.push_back(WGPUCompositeAlphaMode_Inherit);

  WGPUCompositeAlphaMode* alpha_modes_out = nullptr;
  if (!alpha_modes.empty()) {
    alpha_modes_out = new WGPUCompositeAlphaMode[alpha_modes.size()];
    std::memcpy(alpha_modes_out, alpha_modes.data(),
                alpha_modes.size() * sizeof(WGPUCompositeAlphaMode));
  }

  capabilities->alphaModeCount = alpha_modes.size();
  capabilities->alphaModes = alpha_modes_out;

  return WGPUStatus_Success;
}

void GFXSurface::GetCurrentTexture(WGPUSurfaceTexture* surfaceTexture) {}

WGPUStatus GFXSurface::Present() {
  return WGPUStatus();
}

void GFXSurface::SetLabel(WGPUStringView label) {
  label_ = std::string(label.data, label.length);
}

void GFXSurface::Unconfigure() {}

}  // namespace vkgfx

///////////////////////////////////////////////////////////////////////////////
// Member Function

GFX_REFCOUNTED_EXPORT(Surface, vkgfx::GFXSurface);

GFX_EXPORT void GFX_FUNCTION(SurfaceConfigure)(
    WGPUSurface surface,
    WGPUSurfaceConfiguration const* config) {
  auto* self = static_cast<vkgfx::GFXSurface*>(surface);
  self->Configure(config);
}

GFX_EXPORT WGPUStatus
GFX_FUNCTION(SurfaceGetCapabilities)(WGPUSurface surface,
                                     WGPUAdapter adapter,
                                     WGPUSurfaceCapabilities* capabilities) {
  auto* self = static_cast<vkgfx::GFXSurface*>(surface);
  return self->GetCapabilities(adapter, capabilities);
}

GFX_EXPORT void GFX_FUNCTION(SurfaceGetCurrentTexture)(
    WGPUSurface surface,
    WGPUSurfaceTexture* surfaceTexture) {
  auto* self = static_cast<vkgfx::GFXSurface*>(surface);
  self->GetCurrentTexture(surfaceTexture);
}

GFX_EXPORT WGPUStatus GFX_FUNCTION(SurfacePresent)(WGPUSurface surface) {
  auto* self = static_cast<vkgfx::GFXSurface*>(surface);
  return self->Present();
}

GFX_EXPORT void GFX_FUNCTION(SurfaceSetLabel)(WGPUSurface surface,
                                              WGPUStringView label) {
  auto* self = static_cast<vkgfx::GFXSurface*>(surface);
  self->SetLabel(label);
}

GFX_EXPORT void GFX_FUNCTION(SurfaceUnconfigure)(WGPUSurface surface) {
  auto* self = static_cast<vkgfx::GFXSurface*>(surface);
  self->Unconfigure();
}
