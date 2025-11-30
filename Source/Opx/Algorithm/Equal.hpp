#pragma once

#include "../DataTypes.hpp"

OPX_NAMESPACE_BEGIN

namespace Algorithm {
    template <typename LhsIterator, typename RhsIterator>
    OPX_NODISCARD OPX_CONSTEXPR Bool Equal(LhsIterator lhsFirst,
                                           LhsIterator lhsLast,
                                           RhsIterator rhsFirst) {
        for (; lhsFirst != lhsLast; ++lhsFirst, ++rhsFirst) {
            if (!(*lhsFirst == *rhsFirst)) {
                return false;
            }
        }
        return true;
    }

    template <typename LhsIterator, typename RhsIterator, typename Predicate>
    OPX_NODISCARD Bool
    Equal(LhsIterator lhsFirst, LhsIterator lhsLast, RhsIterator rhsFirst, Predicate pred) {
        for (; lhsFirst != lhsLast; ++lhsFirst, ++rhsFirst) {
            if (!pred(*lhsFirst, *rhsFirst)) {
                return false;
            }
        }
        return true;
    }
}  // namespace Algorithm

OPX_NAMESPACE_END