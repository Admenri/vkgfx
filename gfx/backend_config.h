// Copyright 2025 Admenri.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#ifndef GFX_CONFIG_H_
#define GFX_CONFIG_H_

#include <volk.h>
#include <webgpu/webgpu.h>

#define GFX_EXPORT WGPU_EXPORT extern "C"
#define GFX_FUNCTION(name) wgpu##name

#define GFX_REFCOUNTED_EXPORT(klass, type)                         \
  GFX_EXPORT void GFX_FUNCTION(klass##AddRef)(WGPU##klass self) {  \
    static_cast<type*>(self)->AddRef();                            \
  }                                                                \
  GFX_EXPORT void GFX_FUNCTION(klass##Release)(WGPU##klass self) { \
    static_cast<type*>(self)->Release();                           \
  }

#define GFX_STRVIEW(x) WGPUStringView{x, sizeof(x) - 1}

#endif  // !GFX_CONFIG_H_
