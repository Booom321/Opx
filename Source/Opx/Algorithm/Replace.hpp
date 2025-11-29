#pragma once

#include "../DataTypes.hpp"

OPX_NAMESPACE_BEGIN

namespace Algorithm {
    template <typename InputIterator, typename T>
    void Replace(InputIterator start, InputIterator end, const T& oldValue, const T& newValue) {
        for (; start != end; ++start) {
            if (*start == oldValue) {
                *start = newValue;
            }
        }
    }

    template <typename InputIterator, typename Predicate, typename T>
    void ReplaceIf(InputIterator start, InputIterator end, Predicate pred, const T& newValue) {
        for (; start != end; ++start) {
            if (pred(*start)) {
                *start = newValue;
            }
        }
    }
}  // namespace Algorithm

OPX_NAMESPACE_END