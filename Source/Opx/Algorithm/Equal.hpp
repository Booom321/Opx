#pragma once

#include "../DataTypes.hpp"

OPX_NAMESPACE_BEGIN

namespace Algorithm {
    template <typename LhsIterator, typename RhsIterator>
    OPX_NODISCARD OPX_CONSTEXPR Bool Equal(LhsIterator lhsStart, LhsIterator lhsEnd, RhsIterator rhsStart) {
        for (; lhsStart != lhsEnd; ++lhsStart, ++rhsStart) {
            if (!(*lhsStart == *rhsStart)) {
                return false;
            }
        }
        return true;
    }

    template <typename LhsIterator, typename RhsIterator, typename Predicate>
    OPX_NODISCARD OPX_CONSTEXPR Bool Equal(LhsIterator lhsStart, LhsIterator lhsEnd, RhsIterator rhsStart, Predicate pred) {
        for (; lhsStart != lhsEnd; ++lhsStart, ++rhsStart) {
            if (!pred(*lhsStart, *rhsStart)) {
                return false;
            }
        }
        return true;
    }
}  // namespace Algorithm

OPX_NAMESPACE_END