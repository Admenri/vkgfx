// Copyright 2025 Admenri.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#ifndef GFX_GFX_COMMAND_BUFFER_H_
#define GFX_GFX_COMMAND_BUFFER_H_

#include "gfx/common/refptr.h"
#include "gfx/gfx_config.h"

struct WGPUCommandBufferImpl {};

namespace vkgfx {

// https://gpuweb.github.io/gpuweb/#gpucommandbuffer
class GFXCommandBuffer : public RefCounted<GFXCommandBuffer>,
                         public WGPUCommandBufferImpl {
 public:
  GFXCommandBuffer();
  ~GFXCommandBuffer();

  GFXCommandBuffer(const GFXCommandBuffer&) = delete;
  GFXCommandBuffer& operator=(const GFXCommandBuffer&) = delete;

  void SetLabel(WGPUStringView label);

 private:
  std::string label_;
};

}  // namespace vkgfx

#endif  // GFX_GFX_COMMAND_BUFFER_H_
