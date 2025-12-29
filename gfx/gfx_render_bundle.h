// Copyright 2025 Admenri.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#ifndef GFX_GFX_RENDER_BUNDLE_H_
#define GFX_GFX_RENDER_BUNDLE_H_

#include "gfx/common/refptr.h"
#include "gfx/gfx_config.h"

struct WGPURenderBundleImpl {};

namespace vkgfx {

// https://gpuweb.github.io/gpuweb/#gpurenderbundle
class GFXRenderBundle : public RefCounted<GFXRenderBundle>,
                        public WGPURenderBundleImpl {
 public:
  GFXRenderBundle();
  ~GFXRenderBundle();

  GFXRenderBundle(const GFXRenderBundle&) = delete;
  GFXRenderBundle& operator=(const GFXRenderBundle&) = delete;

  void SetLabel(WGPUStringView label);

 private:
  std::string label_;
};

}  // namespace vkgfx

#endif
