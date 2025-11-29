#pragma once

#include "../DataTypes.hpp"

OPX_NAMESPACE_BEGIN

namespace Algorithm {
    template <typename InputIterator, typename T>
    OPX_NODISCARD OPX_CONSTEXPR Bool Contains(InputIterator first, InputIterator last,
                                              const T& value) {
        for (; first != last; ++first) {
            if (*first == value) {
                return true;
            }
        }
        return false;
    }

    template <typename InputIterator, typename Predicate>
    OPX_NODISCARD Bool ContainsIf(InputIterator first, InputIterator last, Predicate pred) {
        for (; first != last; ++first) {
            if (pred(*first)) {
                return true;
            }
        }
        return false;
    }

    template <typename InputIterator, typename Predicate>
    OPX_NODISCARD Bool ContainsIfNot(InputIterator first, InputIterator last, Predicate pred) {
        for (; first != last; ++first) {
            if (!pred(*first)) {
                return true;
            }
        }
        return false;
    }
}  // namespace Algorithm

OPX_NAMESPACE_END