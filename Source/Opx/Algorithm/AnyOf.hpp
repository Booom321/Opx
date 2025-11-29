#pragma once

#include "../DataTypes.hpp"

OPX_NAMESPACE_BEGIN

namespace Algorithm {
    template<typename InputIterator, typename Predicate>
    Bool AnyOf(InputIterator first, InputIterator last, Predicate pred) {
        for (; first != last; ++first) {
            if (pred(*first)) {
                return true;
            }
        }
        return false;
    }    
} // namespace Algorithm

OPX_NAMESPACE_END