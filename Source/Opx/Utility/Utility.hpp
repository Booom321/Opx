#pragma once

#include "../TypeTraits.hpp"

OPX_NAMESPACE_BEGIN

template <typename T>
OPX_CONSTEXPR T&& Forward(RemoveReference_T<T>& value) noexcept {
    return static_cast<T&&>(value);
}

template <typename T>
OPX_CONSTEXPR T&& Forward(RemoveReference_T<T>&& value) noexcept {
    return static_cast<T&&>(value);
}

template <typename T>
OPX_CONSTEXPR RemoveReference_T<T>&& Move(T&& value) noexcept {
    return static_cast<RemoveReference_T<T>&&>(value);
}

template <typename T>
OPX_CONSTEXPR AddConst_T<T>& AsConst(T& value) noexcept {
    return value;
}

template <typename T>
AddRvalueReference_T<T> Declval() noexcept {
    static_assert(false, "Declval() not allowed in an evaluated context");
}

OPX_NAMESPACE_END