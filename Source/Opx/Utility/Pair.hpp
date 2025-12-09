#pragma once

#include "../Utility/Utility.hpp"

OPX_NAMESPACE_BEGIN

template <typename F, typename S>
struct Pair {
    using FirstType = F;
    using SecondType = S;

    template <typename F1 = F,
              typename S1 = S,
              typename = EnableIf_T<IsDefaultConstructible_V<F1> && IsDefaultConstructible_V<S1>>>
    OPX_CONSTEXPR Pair() : first(), second() {};
    OPX_CONSTEXPR Pair(const Pair&) = default;
    OPX_CONSTEXPR Pair(Pair&&) = default;

    template <typename F1 = F,
              typename S1 = S,
              typename = EnableIf_T<IsCopyConstructible_V<F1> && IsCopyConstructible_V<S1>>>
    OPX_CONSTEXPR Pair(const F1& first, const S1& second) : first(first), second(second) {}

    template <typename F1 = F,
              typename S1 = S,
              typename = EnableIf_T<IsConstructible_V<F, F1> && IsConstructible_V<S, S1>>>
    OPX_CONSTEXPR Pair(F1&& first, S1&& second)
        : first(Forward<F1>(first)), second(Forward<S1>(second)) {}

    template <typename S1 = S, typename = EnableIf_T<IsConstructible_V<S, S1>>>
    OPX_CONSTEXPR Pair(const F& first, S1&& second) : first(first), second(Forward<S1>(second)) {}

    template <typename F1 = F, typename = EnableIf_T<IsConstructible_V<F, F1>>>
    OPX_CONSTEXPR Pair(F1&& first, const S& second) : first(Forward<F1>(first)), second(second) {}

    template <
        typename F1 = F,
        typename S1 = S,
        typename = EnableIf_T<IsConstructible_V<F, const F1&> && IsConstructible_V<S, const S1&>>>
    OPX_CONSTEXPR Pair(const Pair<F1, S1>& other) : first(other.first), second(other.second) {}

    template <typename F1 = F,
              typename S1 = S,
              typename = EnableIf_T<IsConstructible_V<F, F1> && IsConstructible_V<S, S1>>>
    OPX_CONSTEXPR Pair(Pair<F1, S1>&& other)
        : first(Forward<F1>(other.first)), second(Forward<S1>(other.second)) {};

    ~Pair() = default;

    Pair& operator=(const Pair& other) {
        if (this != &other) {
            first = other.first;
            second = other.second;
        }
        return *this;
    }

    Pair& operator=(Pair&& other) {
        if (this != &other) {
            first = Move(other.first);
            second = Move(other.second);
        }
        return *this;
    }

    template <typename F1,
              typename S1,
              typename = EnableIf_T<IsAssignable_V<F&, const F1&> && IsAssignable_V<S&, const S1&>>>
    OPX_INLINE Pair& operator=(const Pair<F1, S1>& other) {
        first = other.first;
        second = other.second;
        return *this;
    }

    template <typename F1,
              typename S1,
              typename = EnableIf_T<IsAssignable_V<F&, F1> && IsAssignable_V<S&, S1>>>
    OPX_INLINE Pair& operator=(Pair<F1, S1>&& other) {
        first = Forward<F1>(other.first);
        second = Forward<S1>(other.second);
        return *this;
    }

    FirstType first;
    SecondType second;
};

template <typename F, typename S>
OPX_INLINE Bool operator==(const Pair<F, S>& lhs, const Pair<F, S>& rhs) {
    return lhs.first == rhs.first && lhs.second == rhs.second;
}

template <typename F, typename S>
OPX_INLINE Bool operator!=(const Pair<F, S>& lhs, const Pair<F, S>& rhs) {
    return lhs.first != rhs.first || lhs.second != rhs.second;
}

template <typename F, typename S>
Pair<F, S> MakePair(F&& first, S&& second) {
    return Pair<F, S>(Forward<F>(first), Forward<S>(second));
}

OPX_NAMESPACE_END