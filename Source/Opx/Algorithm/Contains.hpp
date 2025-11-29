#pragma once

#include "../DataTypes.hpp"

OPX_NAMESPACE_BEGIN

namespace Algorithm {
    template <typename InputIterator, typename T>
    OPX_NODISCARD OPX_CONSTEXPR Bool Contains(InputIterator start, InputIterator end, const T& value) {
        for (; start != end; ++start) {
            if (*start == value) {
                return true;
            }
        }
        return false;
    }

    template <typename InputIterator, typename Predicate>
    OPX_NODISCARD Bool ContainsIf(InputIterator start, InputIterator end, Predicate pred) {
        for (; start != end; ++start) {
            if (pred(*start)) {
                return true;
            }
        }
        return false;
    }

    template <typename InputIterator, typename Predicate>
    OPX_NODISCARD Bool ContainsIfNot(InputIterator start, InputIterator end, Predicate pred) {
        for (; start != end; ++start) {
            if (!pred(*start)) {
                return true;
            }
        }
        return false;
    }
}  // namespace Algorithm

OPX_NAMESPACE_END