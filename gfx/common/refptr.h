// Copyright 2025 Admenri.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#ifndef GFX_COMMON_REFPTR_H_
#define GFX_COMMON_REFPTR_H_

#include <atomic>
#include <cstddef>
#include <cstdint>
#include <utility>

namespace vkgfx {

template <typename T>
struct DefaultRefCountedTraits {
  static void Destruct(const T* x) { delete x; }
};

template <class T,
          typename Traits = DefaultRefCountedTraits<T>,
          typename CountTy = uint64_t>
class RefCounted {
 public:
  RefCounted() : ref_count_(1) {}

  RefCounted(const RefCounted&) = delete;
  RefCounted& operator=(const RefCounted&) = delete;

  void AddRef() const { ref_count_.fetch_add(1, std::memory_order_relaxed); }

  bool Release() const {
    if (ref_count_.fetch_sub(1, std::memory_order_release) == 1) {
      // Acquire fence ensures that the destructor sees all changes
      // made by other threads before they released.
      std::atomic_thread_fence(std::memory_order_acquire);
      // Delete resource
      Traits::Destruct(static_cast<const T*>(this));
      // Released
      return true;
    }
    return false;
  }

  // Returns the current reference count (mostly for debugging).
  CountTy RefCount() const {
    return ref_count_.load(std::memory_order_relaxed);
  }

 protected:
  ~RefCounted() = default;

 private:
  mutable std::atomic<CountTy> ref_count_;
};

template <typename T>
class RefPtr {
 public:
  using element_type = T;

  constexpr RefPtr() noexcept = default;
  constexpr RefPtr(std::nullptr_t) noexcept {}
  constexpr RefPtr(T* p) noexcept : ptr_(p) {}

  RefPtr(RefPtr&& other) noexcept : ptr_(other.ptr_) { other.ptr_ = nullptr; }
  RefPtr(const RefPtr& other) noexcept : ptr_(other.ptr_) {
    if (ptr_)
      ptr_->AddRef();
  }

  template <typename U>
  RefPtr(const RefPtr<U>& other) noexcept : ptr_(other.get()) {
    if (ptr_)
      ptr_->AddRef();
  }

  template <typename U>
  RefPtr(RefPtr<U>&& other) noexcept : ptr_(other.ptr_) {
    other.ptr_ = nullptr;
  }

  ~RefPtr() {
    if (ptr_)
      ptr_->Release();
  }

  RefPtr& operator=(T* p) { return *this = RefPtr(p); }

  RefPtr& operator=(std::nullptr_t) {
    reset();
    return *this;
  }

  RefPtr& operator=(RefPtr r) noexcept {
    swap(r);
    return *this;
  }

  T* get() const { return ptr_; }
  T& operator*() const { return *ptr_; }
  T* operator->() const { return ptr_; }
  operator T*() const { return ptr_; }
  explicit operator bool() const { return ptr_ != nullptr; }

  void reset() { RefPtr().swap(*this); }
  void swap(RefPtr& other) noexcept { std::swap(ptr_, other.ptr_); }

  [[nodiscard]] T* release() {
    T* ptr = ptr_;
    ptr_ = nullptr;
    return ptr;
  }

 private:
  template <typename U>
  friend class RefPtr;

  T* ptr_ = nullptr;
};

// Comparison operators
template <typename T, typename U>
inline bool operator==(const RefPtr<T>& lhs, const RefPtr<U>& rhs) {
  return lhs.get() == rhs.get();
}

template <typename T, typename U>
inline bool operator!=(const RefPtr<T>& lhs, const RefPtr<U>& rhs) {
  return lhs.get() != rhs.get();
}

template <typename T>
inline bool operator==(const RefPtr<T>& lhs, std::nullptr_t) {
  return lhs.get() == nullptr;
}

template <typename T>
inline bool operator==(std::nullptr_t, const RefPtr<T>& rhs) {
  return rhs.get() == nullptr;
}

// Constructs an instance of T, which is a ref counted type, and wraps the
// object into a scoped_refptr<T>.
template <typename T, typename... Args>
RefPtr<T> MakeRefCounted(Args&&... args) {
  T* obj = new T(std::forward<Args>(args)...);
  return RefPtr<T>(obj);
}

}  // namespace vkgfx

#endif  // GFX_COMMON_REFPTR_H_