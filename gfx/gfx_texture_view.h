// Copyright 2025 Admenri.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#ifndef GFX_GFX_TEXTURE_VIEW_H_
#define GFX_GFX_TEXTURE_VIEW_H_

#include "gfx/common/refptr.h"
#include "gfx/gfx_config.h"

struct WGPUTextureViewImpl {};

namespace vkgfx {

// https://gpuweb.github.io/gpuweb/#gputextureview
class GFXTextureView : public RefCounted<GFXTextureView>,
                       public WGPUTextureViewImpl {
 public:
  GFXTextureView();
  ~GFXTextureView();

  GFXTextureView(const GFXTextureView&) = delete;
  GFXTextureView& operator=(const GFXTextureView&) = delete;

  void SetLabel(WGPUStringView label);

 private:
  VkImageView view_;

  std::string label_;
};

}  // namespace vkgfx

#endif  // GFX_GFX_TEXTURE_VIEW_H_
