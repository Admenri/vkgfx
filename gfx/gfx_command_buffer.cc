// Copyright 2025 Admenri.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#include "gfx/gfx_command_buffer.h"

namespace vkgfx {

///////////////////////////////////////////////////////////////////////////////
// GFXCommandBuffer Implement

GFXCommandBuffer::GFXCommandBuffer() {}

GFXCommandBuffer::~GFXCommandBuffer() {}

void GFXCommandBuffer::SetLabel(WGPUStringView label) {
  label_ = std::string(label.data, label.length);
}

}  // namespace vkgfx

///////////////////////////////////////////////////////////////////////////////
// Member Function

GFX_REFCOUNTED_EXPORT(CommandBuffer, vkgfx::GFXCommandBuffer);

GFX_EXPORT void GFX_FUNCTION(CommandBufferSetLabel)(
    WGPUCommandBuffer commandBuffer,
    WGPUStringView label) {
  auto* self = static_cast<vkgfx::GFXCommandBuffer*>(commandBuffer);
  self->SetLabel(label);
}
