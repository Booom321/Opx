#pragma once

#include "../DataTypes.hpp"

OPX_NAMESPACE_BEGIN

namespace Algorithm {
    template <typename InputIterator, typename T>
    void Replace(InputIterator first, InputIterator last, const T& oldValue, const T& newValue) {
        for (; first != last; ++first) {
            if (*first == oldValue) {
                *first = newValue;
            }
        }
    }

    template <typename InputIterator, typename Predicate, typename T>
    void ReplaceIf(InputIterator first, InputIterator last, Predicate pred, const T& newValue) {
        for (; first != last; ++first) {
            if (pred(*first)) {
                *first = newValue;
            }
        }
    }
}  // namespace Algorithm

OPX_NAMESPACE_END