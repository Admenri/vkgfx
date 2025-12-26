// Copyright 2025 Admenri.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#ifndef GFX_GFX_WGPU_H_
#define GFX_GFX_WGPU_H_

#include "gfx/common/refptr.h"
#include "gfx/gfx_config.h"

namespace vkgfx {

class GFXInstance;

// Global functions for instance
GFXInstance* CreateInstance(WGPUInstanceDescriptor const* descriptor);
void GetInstanceFeatures(WGPUSupportedInstanceFeatures* features);
WGPUStatus GetInstanceLimits(WGPUInstanceLimits* limits);
WGPUBool HasInstanceFeature(WGPUInstanceFeatureName feature);

// Free functions for structs with dynamic members
void AdapterInfoFreeMembers(WGPUAdapterInfo adapterInfo);
void SupportedFeaturesFreeMembers(WGPUSupportedFeatures supportedFeatures);
void SupportedInstanceFeaturesFreeMembers(
    WGPUSupportedInstanceFeatures supportedInstanceFeatures);
void SupportedWGSLLanguageFeaturesFreeMembers(
    WGPUSupportedWGSLLanguageFeatures supportedWGSLLanguageFeatures);
void SurfaceCapabilitiesFreeMembers(
    WGPUSurfaceCapabilities surfaceCapabilities);

}  // namespace vkgfx

#endif  // !GFX_GFX_WGPU_H_
