#pragma once

#include "../DataTypes.hpp"

OPX_NAMESPACE_BEGIN

namespace Algorithm {
    template <typename BidirectionalIterator, typename T>
    OPX_NODISCARD OPX_CONSTEXPR BidirectionalIterator FindLast(BidirectionalIterator first,
                                                               BidirectionalIterator last,
                                                               const T& value) {
        BidirectionalIterator lastTmp(last);
        while (lastTmp != first) {
            if (*--lastTmp == value) {
                return lastTmp;
            }
        }
        return last;
    }

    template <typename BidirectionalIterator, typename Predicate>
    OPX_NODISCARD OPX_CONSTEXPR BidirectionalIterator FindLastIf(BidirectionalIterator first,
                                                                 BidirectionalIterator last,
                                                                 Predicate pred) {
        BidirectionalIterator lastTmp(last);
        while (lastTmp != first) {
            if (pred(*--lastTmp)) {
                return lastTmp;
            }
        }
        return last;
    }

    template <typename BidirectionalIterator, typename Predicate>
    OPX_NODISCARD OPX_CONSTEXPR BidirectionalIterator FindLastIfNot(BidirectionalIterator first,
                                                                    BidirectionalIterator last,
                                                                    Predicate pred) {
        BidirectionalIterator lastTmp(last);
        while (lastTmp != first) {
            if (!pred(*--lastTmp)) {
                return lastTmp;
            }
        }
        return last;
    }
}  // namespace Algorithm

OPX_NAMESPACE_END