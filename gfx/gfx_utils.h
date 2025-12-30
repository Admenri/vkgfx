// Copyright 2025 Admenri.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#ifndef GFX_GFX_UTILS_H_
#define GFX_GFX_UTILS_H_

#include <functional>
#include <string>
#include <string_view>

#include "gfx/gfx_config.h"

namespace vkgfx {

// External ref_counted adapt
template <class Ty>
inline Ty* AdaptExternalRefCounted(Ty* obj) {
  if (obj)
    obj->AddRef();
  return obj;
}

// String view utility
inline WGPUStringView MakeStringView(const std::string_view& view) {
  if (view.empty())
    return WGPUStringView();

  char* data = new char[view.size() + 1];
  std::memcpy(data, view.data(), view.size() + 1);
  return WGPUStringView{data, view.size()};
}

inline void FreeStringView(WGPUStringView& view) {
  if (view.data)
    delete[] view.data;
}

inline std::string_view FromWGPUStringView(const WGPUStringView& view) {
  if (!view.data || !view.length)
    return std::string_view();

  return std::string_view(view.data, view.length);
}

// pNext chain builder utility
class NextChainBuilder {
 public:
  template <typename Ty>
  explicit NextChainBuilder(Ty* head)
      : current_(reinterpret_cast<VkBaseOutStructure*>(head)) {
    while (current_->pNext)
      current_ = current_->pNext;
  }

  NextChainBuilder(const NextChainBuilder&) = delete;
  NextChainBuilder& operator=(const NextChainBuilder&) = delete;

  template <typename Ty>
  void Add(Ty* data) {
    current_->pNext = reinterpret_cast<VkBaseOutStructure*>(data);
    current_ = current_->pNext;
  }

 private:
  void* operator new(size_t) = delete;
  void* operator new(size_t, void*) = delete;

  VkBaseOutStructure* current_;
};

// chained struct extractor
class ChainedStructExtractor {
 public:
  explicit ChainedStructExtractor(WGPUChainedStruct* head) : root_(head) {}

  ChainedStructExtractor(const ChainedStructExtractor&) = delete;
  ChainedStructExtractor& operator=(const ChainedStructExtractor&) = delete;

  template <typename Ty>
  Ty* GetStruct(WGPUSType type) {
    for (auto* current = root_; current; current = current->next)
      if (current->sType == type)
        return reinterpret_cast<Ty*>(current);
  }

  void VisitChain(const std::function<void(WGPUChainedStruct*)>& visitor) {
    for (auto* current = root_; current; current = current->next)
      visitor(current);
  }

 private:
  void* operator new(size_t) = delete;
  void* operator new(size_t, void*) = delete;

  WGPUChainedStruct* root_;
};

}  // namespace vkgfx

#endif  // !GFX_GFX_UTILS_H_
