#pragma once
#ifndef PORTABLE_LCE_ATOMIC_REF_H
#define PORTABLE_LCE_ATOMIC_REF_H

#include <atomic>

#if !defined(__cpp_lib_atomic_ref)
namespace std {
template <typename T>
struct atomic_ref {
    T& obj;
    explicit atomic_ref(T& ref) noexcept : obj(ref) {}

    bool compare_exchange_strong(T& expected, T desired,
                                 std::memory_order success = std::memory_order_seq_cst,
                                 std::memory_order failure = std::memory_order_seq_cst) noexcept {
        return __atomic_compare_exchange_n(&obj, &expected, desired, false, (int)success, (int)failure);
    }

    bool compare_exchange_weak(T& expected, T desired,
                               std::memory_order success = std::memory_order_seq_cst,
                               std::memory_order failure = std::memory_order_seq_cst) noexcept {
        return __atomic_compare_exchange_n(&obj, &expected, desired, true, (int)success, (int)failure);
    }

    T load(std::memory_order order = std::memory_order_seq_cst) const noexcept {
        return __atomic_load_n(&obj, (int)order);
    }

    void store(T desired, std::memory_order order = std::memory_order_seq_cst) noexcept {
        __atomic_store_n(&obj, desired, (int)order);
    }
};
} // namespace std
#endif

#endif // PORTABLE_LCE_ATOMIC_REF_H
