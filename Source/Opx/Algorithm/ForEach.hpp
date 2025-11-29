#pragma once

#include "../DataTypes.hpp"

OPX_NAMESPACE_BEGIN

namespace Algorithm {
    template <typename InputIterator, typename ModifierFn>
    void ForEach(InputIterator first, InputIterator last, ModifierFn fn) {
        for (; first != last; ++first) {
            fn(*first);
        }
    }
}  // namespace Algorithm

OPX_NAMESPACE_END