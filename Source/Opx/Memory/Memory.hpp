#pragma once

#include <cstring>

#include "../Utility/Utility.hpp"

OPX_NAMESPACE_BEGIN

namespace Memory {
    void* Allocate(SizeT size);
    void Free(void* pointer);

    template <typename T, typename SizeType = SizeT>
    void DestructItems(T* pointer, SizeType num) {
        if constexpr (!IsTriviallyDestructible_V<T>) {
            while (num--) {
                pointer->~T();
                ++pointer;
            }
        }
    }

    template <typename T, typename SizeType = SizeT>
    void DefaultConstructItems(T* pointer, SizeType num) {
        if constexpr (IsTriviallyCopyable_V<T>) {
            memset(pointer, 0, sizeof(T) * static_cast<SizeT>(num));
        } else {
            while (num--) {
                new (pointer) T();
                ++pointer;
            }
        }
    }

    template <typename T, typename SizeType = SizeT>
    void CopyConstructItems(T* dest, const T* source, SizeType num) {
        if constexpr (IsTriviallyCopyable_V<T>) {
            memcpy(dest, source, sizeof(T) * static_cast<SizeT>(num));
        } else {
            while (num--) {
                new (dest) T(*source);
                ++dest, ++source;
            }
        }
    }

    template <typename T, typename SizeType = SizeT>
    void MoveConstructItems(T* dest, T* source, SizeType num) {
        if constexpr (IsTriviallyCopyable_V<T>) {
            memcpy(dest, source, sizeof(T) * static_cast<SizeT>(num));
        } else {
            while (num--) {
                new (dest) T(Move(*source));
                ++dest, ++source;
            }
        }
    }

    template <typename T, typename SizeType = SizeT>
    void CopyAssignItems(T* dest, const T* source, SizeType num) {
        if constexpr (IsTriviallyCopyable_V<T>) {
            memcpy(dest, source, sizeof(T) * static_cast<SizeT>(num));
        } else {
            while (num--) {
                *dest = *source;
                ++dest, ++source;
            }
        }
    }

    template <typename T, typename SizeType = SizeT>
    void MoveAssignItems(T* dest, T* source, SizeType num) {
        if constexpr (IsTriviallyCopyable_V<T>) {
            memcpy(dest, source, sizeof(T) * static_cast<SizeT>(num));
        } else {
            while (num--) {
                *dest = Move(*source);
                ++dest, ++source;
            }
        }
    }

    template <typename T, typename SizeType = SizeT>
    void MoveBackwardAssignItems(T* dest, T* source, SizeType num) {
        if constexpr (IsTriviallyCopyable_V<T>) {
            memmove(dest - num, source - num, sizeof(T) * static_cast<SizeT>(num));
        } else {
            while (num--) {
                *--dest = Move(*--source);
            }
        }
    }

    template <typename T, typename SizeType = SizeT>
    void CopyBackwardAssignItems(T* dest, T* source, SizeType num) {
        if constexpr (IsTriviallyCopyable_V<T>) {
            memmove(dest - num, source - num, sizeof(T) * static_cast<SizeT>(num));
        } else {
            while (num--) {
                *--dest = *--source;
            }
        }
    }

    template <typename T, typename SizeType = SizeT>
    void FillConstructItems(T* dest, const T& value, SizeType num) {
        while (num--) {
            new (dest) T(value);
            ++dest;
        }
    }

    template <typename T, typename SizeType = SizeT>
    void FillAssignItems(T* dest, const T& value, SizeType num) {
        while (num--) {
            *dest = value;
            ++dest;
        }
    }
}  // namespace Memory

OPX_NAMESPACE_END