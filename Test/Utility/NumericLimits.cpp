#include "Framework/Framework.hpp"

#include <Opx/Utility/NumericLimits.hpp>

TEST_CASE(Utility, NumericLimits) {
    using namespace Opx;

#if defined(__cpp_char8_t)
    TEST_EXPECT_TRUE(NumericLimits<char8_t>::kIsInteger);
    TEST_EXPECT_TRUE(!NumericLimits<char8_t>::kHasInfinity);
    TEST_EXPECT_TRUE(!NumericLimits<char8_t>::kHasNaN);
#endif

    TEST_EXPECT_TRUE(
        NumericLimits<bool>::kIsInteger && NumericLimits<char>::kIsInteger &&
        NumericLimits<signed char>::kIsInteger && NumericLimits<unsigned char>::kIsInteger &&
        NumericLimits<wchar_t>::kIsInteger && NumericLimits<char16_t>::kIsInteger &&
        NumericLimits<char32_t>::kIsInteger && NumericLimits<unsigned short>::kIsInteger &&
        NumericLimits<int>::kIsInteger && NumericLimits<unsigned int>::kIsInteger &&
        NumericLimits<long>::kIsInteger && NumericLimits<unsigned long>::kIsInteger &&
        NumericLimits<long>::kIsInteger && NumericLimits<unsigned long long>::kIsInteger &&
        !NumericLimits<float>::kIsInteger && !NumericLimits<double>::kIsInteger &&
        !NumericLimits<long double>::kIsInteger);

    TEST_EXPECT_TRUE(
        !NumericLimits<bool>::kHasInfinity && !NumericLimits<char>::kHasInfinity &&
        !NumericLimits<signed char>::kHasInfinity && !NumericLimits<unsigned char>::kHasInfinity &&
        !NumericLimits<wchar_t>::kHasInfinity && !NumericLimits<char16_t>::kHasInfinity &&
        !NumericLimits<char32_t>::kHasInfinity && !NumericLimits<unsigned short>::kHasInfinity &&
        !NumericLimits<int>::kHasInfinity && !NumericLimits<unsigned int>::kHasInfinity &&
        !NumericLimits<long>::kHasInfinity && !NumericLimits<unsigned long>::kHasInfinity &&
        !NumericLimits<long>::kHasInfinity && !NumericLimits<unsigned long long>::kHasInfinity &&
        NumericLimits<float>::kHasInfinity && NumericLimits<double>::kHasInfinity &&
        NumericLimits<long double>::kHasInfinity);

    TEST_EXPECT_TRUE(!NumericLimits<bool>::kHasNaN && !NumericLimits<char>::kHasNaN &&
                     !NumericLimits<signed char>::kHasNaN &&
                     !NumericLimits<unsigned char>::kHasNaN && !NumericLimits<wchar_t>::kHasNaN &&
                     !NumericLimits<char16_t>::kHasNaN && !NumericLimits<char32_t>::kHasNaN &&
                     !NumericLimits<unsigned short>::kHasNaN && !NumericLimits<int>::kHasNaN &&
                     !NumericLimits<unsigned int>::kHasNaN && !NumericLimits<long>::kHasNaN &&
                     !NumericLimits<unsigned long>::kHasNaN && !NumericLimits<long>::kHasNaN &&
                     !NumericLimits<unsigned long long>::kHasNaN && NumericLimits<float>::kHasNaN &&
                     NumericLimits<double>::kHasNaN && NumericLimits<long double>::kHasNaN);
}
