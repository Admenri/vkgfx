// Copyright 2025 Admenri.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#include "gfx/gfx_query_set.h"

namespace vkgfx {

///////////////////////////////////////////////////////////////////////////////
// GFXQuerySet Implement

GFXQuerySet::GFXQuerySet() {}

GFXQuerySet::~GFXQuerySet() {}

void GFXQuerySet::Destroy() {}

uint32_t GFXQuerySet::GetCount() {
  return 0;
}

WGPUQueryType GFXQuerySet::GetType() {
  return WGPUQueryType();
}

void GFXQuerySet::SetLabel(WGPUStringView label) {
  label_ = std::string(label.data, label.length);
}

}  // namespace vkgfx

///////////////////////////////////////////////////////////////////////////////
// Member Function

GFX_REFCOUNTED_EXPORT(QuerySet, vkgfx::GFXQuerySet);

GFX_EXPORT void GFX_FUNCTION(QuerySetDestroy)(WGPUQuerySet querySet) {
  auto* self = static_cast<vkgfx::GFXQuerySet*>(querySet);
  self->Destroy();
}

GFX_EXPORT uint32_t GFX_FUNCTION(QuerySetGetCount)(WGPUQuerySet querySet) {
  auto* self = static_cast<vkgfx::GFXQuerySet*>(querySet);
  return self->GetCount();
}

GFX_EXPORT WGPUQueryType GFX_FUNCTION(QuerySetGetType)(WGPUQuerySet querySet) {
  auto* self = static_cast<vkgfx::GFXQuerySet*>(querySet);
  return self->GetType();
}

GFX_EXPORT void GFX_FUNCTION(QuerySetSetLabel)(WGPUQuerySet querySet,
                                               WGPUStringView label) {
  auto* self = static_cast<vkgfx::GFXQuerySet*>(querySet);
  self->SetLabel(label);
}
