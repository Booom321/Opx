#pragma once

#include "../DataTypes.hpp"

OPX_NAMESPACE_BEGIN

namespace Algorithm {
    template<typename InputIterator, typename Predicate>
    Bool AllOf(InputIterator first, InputIterator last, Predicate pred) {
        for (; first != last; ++first) {
            if (!pred(*first)) {
                return false;
            }
        }
        return true;
    }    
} // namespace Algorithm

OPX_NAMESPACE_END