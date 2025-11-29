#pragma once

#include "../DataTypes.hpp"

OPX_NAMESPACE_BEGIN

namespace Algorithm {
    template <typename InputIterator, typename ModifierFn>
    void ForEach(InputIterator start, InputIterator end, ModifierFn fn) {
        for (; start != end; ++start) {
            fn(*start);
        }
    }
}  // namespace Algorithm

OPX_NAMESPACE_END