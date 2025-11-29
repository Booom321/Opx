#pragma once

#include "../DataTypes.hpp"

OPX_NAMESPACE_BEGIN

namespace Algorithm {
    template <typename InputIterator, typename T>
    OPX_NODISCARD OPX_CONSTEXPR InputIterator Find(InputIterator first, InputIterator last, const T& value) {
        for (; first != last; ++first) {
            if (*first == value) {
                return first;
            }
        }
        return last;
    }

    template <typename InputIterator, typename Predicate>
    OPX_NODISCARD InputIterator FindIf(InputIterator first, InputIterator last, Predicate pred) {
        for (; first != last; ++first) {
            if (pred(*first)) {
                return first;
            }
        }
        return last;
    }

    template <typename InputIterator, typename Predicate>
    OPX_NODISCARD InputIterator FindIfNot(InputIterator first, InputIterator last, Predicate pred) {
        for (; first != last; ++first) {
            if (!pred(*first)) {
                return first;
            }
        }
        return last;
    }
}  // namespace Algorithm

OPX_NAMESPACE_END