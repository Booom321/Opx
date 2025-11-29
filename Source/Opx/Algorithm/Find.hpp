#pragma once

#include "../DataTypes.hpp"

OPX_NAMESPACE_BEGIN

namespace Algorithm {
    template <typename InputIterator, typename T>
    OPX_NODISCARD OPX_CONSTEXPR InputIterator Find(InputIterator start, InputIterator end, const T& value) {
        for (; start != end; ++start) {
            if (*start == value) {
                return start;
            }
        }
        return end;
    }

    template <typename InputIterator, typename Predicate>
    OPX_NODISCARD InputIterator FindIf(InputIterator start, InputIterator end, Predicate pred) {
        for (; start != end; ++start) {
            if (pred(*start)) {
                return start;
            }
        }
        return end;
    }

    template <typename InputIterator, typename Predicate>
    OPX_NODISCARD InputIterator FindIfNot(InputIterator start, InputIterator end, Predicate pred) {
        for (; start != end; ++start) {
            if (!pred(*start)) {
                return start;
            }
        }
        return end;
    }
}  // namespace Algorithm

OPX_NAMESPACE_END