#pragma once

#include "../DataTypes.hpp"

OPX_NAMESPACE_BEGIN

namespace Algorithm {
    template <typename BidirectionalIterator, typename T>
    OPX_NODISCARD OPX_CONSTEXPR BidirectionalIterator FindLast(BidirectionalIterator start, BidirectionalIterator end, const T& value) {
        BidirectionalIterator endTmp(end);
        while (endTmp != start) {
            if (*--endTmp == value) {
                return endTmp;
            }
        }
        return end;
    }

    template <typename BidirectionalIterator, typename Predicate>
    OPX_NODISCARD OPX_CONSTEXPR BidirectionalIterator FindLastIf(BidirectionalIterator start, BidirectionalIterator end, Predicate pred) {
        BidirectionalIterator endTmp(end);
        while (endTmp != start) {
            if (pred(*--endTmp)) {
                return endTmp;
            }
        }
        return end;
    }

    template <typename BidirectionalIterator, typename Predicate>
    OPX_NODISCARD OPX_CONSTEXPR BidirectionalIterator FindLastIfNot(BidirectionalIterator start, BidirectionalIterator end, Predicate pred) {
        BidirectionalIterator endTmp(end);
        while (endTmp != start) {
            if (!pred(*--endTmp)) {
                return endTmp;
            }
        }
        return end;
    }
}  // namespace Algorithm

OPX_NAMESPACE_END