// Copyright 2025 Admenri.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#ifndef GFX_GFX_BIND_GROUP_H_
#define GFX_GFX_BIND_GROUP_H_

#include "gfx/common/refptr.h"
#include "gfx/gfx_config.h"

struct WGPUBindGroupImpl {};

namespace vkgfx {

// https://gpuweb.github.io/gpuweb/#gpubindgroup
class GFXBindGroup : public RefCounted<GFXBindGroup>, public WGPUBindGroupImpl {
 public:
  GFXBindGroup();
  ~GFXBindGroup();

  GFXBindGroup(const GFXBindGroup&) = delete;
  GFXBindGroup& operator=(const GFXBindGroup&) = delete;

  void SetLabel(WGPUStringView label);

 private:
  std::string label_;
};

}  // namespace vkgfx

#endif  // GFX_GFX_BIND_GROUP_H_
