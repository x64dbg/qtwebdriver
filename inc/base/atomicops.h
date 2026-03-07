// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
//
// Modernized: replaced platform-specific implementations with C++17
// std::atomic. All the original Chromium base::subtle:: function
// signatures are preserved as inline wrappers.

#ifndef BASE_ATOMICOPS_H_
#define BASE_ATOMICOPS_H_

#include <atomic>
#include <cstdint>

#include "base/basictypes.h"
#include "build/build_config.h"

namespace base {
namespace subtle {

typedef int32_t Atomic32;
#ifdef ARCH_CPU_64_BITS
typedef intptr_t Atomic64;
#endif
typedef intptr_t AtomicWord;

inline Atomic32 NoBarrier_CompareAndSwap(volatile Atomic32* ptr,
                                         Atomic32 old_value,
                                         Atomic32 new_value) {
    auto* ap = reinterpret_cast<std::atomic<Atomic32>*>(const_cast<Atomic32*>(ptr));
    ap->compare_exchange_strong(old_value, new_value, std::memory_order_relaxed);
    return old_value;
}

inline Atomic32 NoBarrier_AtomicExchange(volatile Atomic32* ptr,
                                         Atomic32 new_value) {
    auto* ap = reinterpret_cast<std::atomic<Atomic32>*>(const_cast<Atomic32*>(ptr));
    return ap->exchange(new_value, std::memory_order_relaxed);
}

inline Atomic32 NoBarrier_AtomicIncrement(volatile Atomic32* ptr,
                                          Atomic32 increment) {
    auto* ap = reinterpret_cast<std::atomic<Atomic32>*>(const_cast<Atomic32*>(ptr));
    return ap->fetch_add(increment, std::memory_order_relaxed) + increment;
}

inline Atomic32 Barrier_AtomicIncrement(volatile Atomic32* ptr,
                                        Atomic32 increment) {
    auto* ap = reinterpret_cast<std::atomic<Atomic32>*>(const_cast<Atomic32*>(ptr));
    return ap->fetch_add(increment, std::memory_order_seq_cst) + increment;
}

inline Atomic32 Acquire_CompareAndSwap(volatile Atomic32* ptr,
                                       Atomic32 old_value,
                                       Atomic32 new_value) {
    auto* ap = reinterpret_cast<std::atomic<Atomic32>*>(const_cast<Atomic32*>(ptr));
    ap->compare_exchange_strong(old_value, new_value,
                                std::memory_order_acquire,
                                std::memory_order_acquire);
    return old_value;
}

inline Atomic32 Release_CompareAndSwap(volatile Atomic32* ptr,
                                       Atomic32 old_value,
                                       Atomic32 new_value) {
    auto* ap = reinterpret_cast<std::atomic<Atomic32>*>(const_cast<Atomic32*>(ptr));
    ap->compare_exchange_strong(old_value, new_value,
                                std::memory_order_release,
                                std::memory_order_relaxed);
    return old_value;
}

inline void MemoryBarrier() {
    std::atomic_thread_fence(std::memory_order_seq_cst);
}

inline void NoBarrier_Store(volatile Atomic32* ptr, Atomic32 value) {
    auto* ap = reinterpret_cast<std::atomic<Atomic32>*>(const_cast<Atomic32*>(ptr));
    ap->store(value, std::memory_order_relaxed);
}

inline void Acquire_Store(volatile Atomic32* ptr, Atomic32 value) {
    auto* ap = reinterpret_cast<std::atomic<Atomic32>*>(const_cast<Atomic32*>(ptr));
    ap->store(value, std::memory_order_seq_cst);
}

inline void Release_Store(volatile Atomic32* ptr, Atomic32 value) {
    auto* ap = reinterpret_cast<std::atomic<Atomic32>*>(const_cast<Atomic32*>(ptr));
    ap->store(value, std::memory_order_release);
}

inline Atomic32 NoBarrier_Load(volatile const Atomic32* ptr) {
    auto* ap = reinterpret_cast<const std::atomic<Atomic32>*>(const_cast<const Atomic32*>(ptr));
    return ap->load(std::memory_order_relaxed);
}

inline Atomic32 Acquire_Load(volatile const Atomic32* ptr) {
    auto* ap = reinterpret_cast<const std::atomic<Atomic32>*>(const_cast<const Atomic32*>(ptr));
    return ap->load(std::memory_order_acquire);
}

inline Atomic32 Release_Load(volatile const Atomic32* ptr) {
    auto* ap = reinterpret_cast<const std::atomic<Atomic32>*>(const_cast<const Atomic32*>(ptr));
    std::atomic_thread_fence(std::memory_order_seq_cst);
    return ap->load(std::memory_order_relaxed);
}

#ifdef ARCH_CPU_64_BITS

inline Atomic64 NoBarrier_CompareAndSwap(volatile Atomic64* ptr,
                                         Atomic64 old_value,
                                         Atomic64 new_value) {
    auto* ap = reinterpret_cast<std::atomic<Atomic64>*>(const_cast<Atomic64*>(ptr));
    ap->compare_exchange_strong(old_value, new_value, std::memory_order_relaxed);
    return old_value;
}

inline Atomic64 NoBarrier_AtomicExchange(volatile Atomic64* ptr,
                                         Atomic64 new_value) {
    auto* ap = reinterpret_cast<std::atomic<Atomic64>*>(const_cast<Atomic64*>(ptr));
    return ap->exchange(new_value, std::memory_order_relaxed);
}

inline Atomic64 NoBarrier_AtomicIncrement(volatile Atomic64* ptr,
                                          Atomic64 increment) {
    auto* ap = reinterpret_cast<std::atomic<Atomic64>*>(const_cast<Atomic64*>(ptr));
    return ap->fetch_add(increment, std::memory_order_relaxed) + increment;
}

inline Atomic64 Barrier_AtomicIncrement(volatile Atomic64* ptr,
                                        Atomic64 increment) {
    auto* ap = reinterpret_cast<std::atomic<Atomic64>*>(const_cast<Atomic64*>(ptr));
    return ap->fetch_add(increment, std::memory_order_seq_cst) + increment;
}

inline Atomic64 Acquire_CompareAndSwap(volatile Atomic64* ptr,
                                       Atomic64 old_value,
                                       Atomic64 new_value) {
    auto* ap = reinterpret_cast<std::atomic<Atomic64>*>(const_cast<Atomic64*>(ptr));
    ap->compare_exchange_strong(old_value, new_value,
                                std::memory_order_acquire,
                                std::memory_order_acquire);
    return old_value;
}

inline Atomic64 Release_CompareAndSwap(volatile Atomic64* ptr,
                                       Atomic64 old_value,
                                       Atomic64 new_value) {
    auto* ap = reinterpret_cast<std::atomic<Atomic64>*>(const_cast<Atomic64*>(ptr));
    ap->compare_exchange_strong(old_value, new_value,
                                std::memory_order_release,
                                std::memory_order_relaxed);
    return old_value;
}

inline void NoBarrier_Store(volatile Atomic64* ptr, Atomic64 value) {
    auto* ap = reinterpret_cast<std::atomic<Atomic64>*>(const_cast<Atomic64*>(ptr));
    ap->store(value, std::memory_order_relaxed);
}

inline void Acquire_Store(volatile Atomic64* ptr, Atomic64 value) {
    auto* ap = reinterpret_cast<std::atomic<Atomic64>*>(const_cast<Atomic64*>(ptr));
    ap->store(value, std::memory_order_seq_cst);
}

inline void Release_Store(volatile Atomic64* ptr, Atomic64 value) {
    auto* ap = reinterpret_cast<std::atomic<Atomic64>*>(const_cast<Atomic64*>(ptr));
    ap->store(value, std::memory_order_release);
}

inline Atomic64 NoBarrier_Load(volatile const Atomic64* ptr) {
    auto* ap = reinterpret_cast<const std::atomic<Atomic64>*>(const_cast<const Atomic64*>(ptr));
    return ap->load(std::memory_order_relaxed);
}

inline Atomic64 Acquire_Load(volatile const Atomic64* ptr) {
    auto* ap = reinterpret_cast<const std::atomic<Atomic64>*>(const_cast<const Atomic64*>(ptr));
    return ap->load(std::memory_order_acquire);
}

inline Atomic64 Release_Load(volatile const Atomic64* ptr) {
    auto* ap = reinterpret_cast<const std::atomic<Atomic64>*>(const_cast<const Atomic64*>(ptr));
    std::atomic_thread_fence(std::memory_order_seq_cst);
    return ap->load(std::memory_order_relaxed);
}

#endif  // ARCH_CPU_64_BITS

}  // namespace subtle
}  // namespace base

#endif  // BASE_ATOMICOPS_H_
