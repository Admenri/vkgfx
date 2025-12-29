// Copyright 2025 Admenri.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#ifndef GFX_GFX_QUERY_SET_H_
#define GFX_GFX_QUERY_SET_H_

#include "gfx/common/refptr.h"
#include "gfx/gfx_config.h"
#include "gfx/gfx_device.h"

struct WGPUQuerySetImpl {};

namespace vkgfx {

// https://gpuweb.github.io/gpuweb/#gpuqueryset
class GFXQuerySet : public RefCounted<GFXQuerySet>, public WGPUQuerySetImpl {
 public:
  GFXQuerySet();
  ~GFXQuerySet();

  GFXQuerySet(const GFXQuerySet&) = delete;
  GFXQuerySet& operator=(const GFXQuerySet&) = delete;

  VkQueryPool GetVkHandle() const { return query_pool_; }

  void Destroy();
  uint32_t GetCount();
  WGPUQueryType GetType();
  void SetLabel(WGPUStringView label);

 private:
  VkQueryPool query_pool_;

  RefPtr<GFXDevice> device_;

  std::string label_;
};

}  // namespace vkgfx

#endif  // GFX_GFX_QUERY_SET_H_