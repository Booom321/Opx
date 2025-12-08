#pragma once

#include "../DataTypes.hpp"

#include <limits>

OPX_NAMESPACE_BEGIN

namespace Details {
    template <typename T>
    class NumericLimitsImpl {
    public:
        static OPX_CONSTEXPR Bool kHasNaN = std::numeric_limits<T>::has_quiet_NaN;
        static OPX_CONSTEXPR Bool kHasInfinity = std::numeric_limits<T>::has_infinity;
        static OPX_CONSTEXPR Bool kIsInteger = std::numeric_limits<T>::is_integer;

        static OPX_CONSTEXPR T kMin = std::numeric_limits<T>::min();
        static OPX_CONSTEXPR T kMax = std::numeric_limits<T>::max();
        static OPX_CONSTEXPR T kInfinity = std::numeric_limits<T>::infinity();
        static OPX_CONSTEXPR T kNaN = std::numeric_limits<T>::quiet_NaN();
    };
}  // namespace Details

template <typename T>
class NumericLimits {
    static_assert(false, "Unsupported type");
};

template <>
class NumericLimits<bool> : public Details::NumericLimitsImpl<bool> {};
template <>
class NumericLimits<char> : public Details::NumericLimitsImpl<char> {};
template <>
class NumericLimits<signed char> : public Details::NumericLimitsImpl<signed char> {};
template <>
class NumericLimits<unsigned char> : public Details::NumericLimitsImpl<unsigned char> {};
template <>
class NumericLimits<wchar_t> : public Details::NumericLimitsImpl<wchar_t> {};
#if defined(OPX_HAS_CHAR8_T)
template <>
class NumericLimits<char8_t> : public Details::NumericLimitsImpl<char8_t> {};
#endif
template <>
class NumericLimits<char16_t> : public Details::NumericLimitsImpl<char16_t> {};
template <>
class NumericLimits<char32_t> : public Details::NumericLimitsImpl<char32_t> {};
template <>
class NumericLimits<short> : public Details::NumericLimitsImpl<short> {};
template <>
class NumericLimits<unsigned short> : public Details::NumericLimitsImpl<unsigned short> {};
template <>
class NumericLimits<int> : public Details::NumericLimitsImpl<int> {};
template <>
class NumericLimits<unsigned int> : public Details::NumericLimitsImpl<unsigned int> {};
template <>
class NumericLimits<long> : public Details::NumericLimitsImpl<long> {};
template <>
class NumericLimits<unsigned long> : public Details::NumericLimitsImpl<unsigned long> {};
template <>
class NumericLimits<long long> : public Details::NumericLimitsImpl<long long> {};
template <>
class NumericLimits<unsigned long long> : public Details::NumericLimitsImpl<unsigned long long> {};
template <>
class NumericLimits<float> : public Details::NumericLimitsImpl<float> {};
template <>
class NumericLimits<double> : public Details::NumericLimitsImpl<double> {};
template <>
class NumericLimits<long double> : public Details::NumericLimitsImpl<long double> {};

OPX_NAMESPACE_END