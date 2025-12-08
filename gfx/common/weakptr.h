// Copyright 2025 Admenri.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#ifndef GFX_COMMON_WEAKPTR_H_
#define GFX_COMMON_WEAKPTR_H_

#include "gfx/common/refptr.h"

namespace vkgfx {

template <typename T>
class WeakPtrFactory;

class WeakRefCount : public RefCounted<WeakRefCount> {
 public:
  WeakRefCount() : flag_(1) {}

  void Invalidate() { flag_.store(0, std::memory_order_release); }
  bool IsValid() const { return flag_.load(std::memory_order_acquire); }

 private:
  std::atomic<uint32_t> flag_;
};

template <typename T>
class WeakPtr {
 public:
  using element_type = T;

  // 1. Constructors
  constexpr WeakPtr() noexcept : ptr_(nullptr) {}
  constexpr WeakPtr(std::nullptr_t) noexcept : ptr_(nullptr) {}

  // Copy Constructor
  WeakPtr(const WeakPtr& other) = default;

  // Move Constructor
  WeakPtr(WeakPtr&& other) noexcept = default;

  // Conversion Copy Constructor (Derived -> Base)
  template <typename U>
  WeakPtr(const WeakPtr<U>& other)
      : ptr_(other.ptr_), ref_count_(other.ref_count_) {}

  // Conversion Move Constructor
  template <typename U>
  WeakPtr(WeakPtr<U>&& other) noexcept
      : ptr_(other.ptr_), ref_count_(std::move(other.ref_count_)) {}

  // 2. Assignment
  WeakPtr& operator=(const WeakPtr& other) = default;
  WeakPtr& operator=(WeakPtr&& other) noexcept = default;
  WeakPtr& operator=(std::nullptr_t) {
    reset();
    return *this;
  }

  // 3. Accessors

  // Returns the raw pointer if the object is still alive, otherwise nullptr.
  T* get() const {
    if (ref_count_ && ref_count_->IsValid())
      return ptr_;
    return nullptr;
  }

  // Dereference operators
  // Note: It is undefined behavior to dereference if the pointer is expired.
  // Always check usage with `if (ptr)` or `ptr.get()`.
  T& operator*() const { return *get(); }
  T* operator->() const { return get(); }

  // Validity check
  explicit operator bool() const { return get() != nullptr; }

  // Returns true if the referenced object has been deleted.
  bool expired() const { return !ref_count_ || !ref_count_->IsValid(); }

  void reset() {
    ptr_ = nullptr;
    ref_count_ = nullptr;
  }

 private:
  template <typename U>
  friend class WeakPtr;

  template <typename U>
  friend class WeakPtrFactory;

  // Private constructor used by WeakPtrFactory
  WeakPtr(T* ptr, RefPtr<WeakRefCount> ref)
      : ptr_(ptr), ref_count_(std::move(ref)) {}

  // The raw pointer to the object.
  T* ptr_;
  // The shared flag that indicates if 'ptr_' is still valid.
  RefPtr<WeakRefCount> ref_count_;
};

template <typename T>
class WeakPtrFactory {
 public:
  explicit WeakPtrFactory(T* ptr) : ptr_(ptr) {
    weak_ref_ = MakeRefCounted<WeakRefCount>();
  }

  ~WeakPtrFactory() { InvalidateWeakPtrs(); }

  // Delete copy and move to prevent accidental invalidation logic issues
  WeakPtrFactory(const WeakPtrFactory&) = delete;
  WeakPtrFactory& operator=(const WeakPtrFactory&) = delete;

  // Creates a WeakPtr pointing to the owner object.
  WeakPtr<T> GetWeakPtr() const {
    // If we've already invalidated, we can't hand out new valid pointers
    // without creating a new anchor, but generally, this shouldn't happen
    // in normal lifecycle usage.
    if (!weak_ref_->IsValid())
      return WeakPtr<T>();
    return WeakPtr<T>(ptr_, weak_ref_);
  }

  // Forces invalidation of all outstanding WeakPtrs.
  // Automatically called in destructor.
  void InvalidateWeakPtrs() {
    if (weak_ref_)
      weak_ref_->Invalidate();
  }

  // Returns true if there are active WeakPtrs referencing this object.
  // Note: This counts the Factory's reference as well.
  bool HasWeakPtrs() const {
    // RefCount is 1 if only the factory holds it.
    return weak_ref_->RefCount() > 1;
  }

 private:
  T* ptr_;
  RefPtr<WeakRefCount> weak_ref_;
};

// Comparison operators for WeakPtr
template <typename T, typename U>
inline bool operator==(const WeakPtr<T>& lhs, const WeakPtr<U>& rhs) {
  return lhs.get() == rhs.get();
}

template <typename T, typename U>
inline bool operator!=(const WeakPtr<T>& lhs, const WeakPtr<U>& rhs) {
  return lhs.get() != rhs.get();
}

template <typename T>
inline bool operator==(const WeakPtr<T>& lhs, std::nullptr_t) {
  return lhs.get() == nullptr;
}

template <typename T>
inline bool operator==(std::nullptr_t, const WeakPtr<T>& rhs) {
  return rhs.get() == nullptr;
}

}  // namespace vkgfx

#endif  //! GFX_COMMON_WEAKPTR_H_