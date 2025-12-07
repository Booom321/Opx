#pragma once

#if defined(TEST_CHAR_TYPE)
#define TEST_CASE_NAME String_Char
#elif defined(TEST_WCHAR_TYPE)
#define TEST_CASE_NAME String_WChar
#elif defined(TEST_CHAR8_TYPE)
#define TEST_CASE_NAME String_Char8
#elif defined(TEST_CHAR16_TYPE)
#define TEST_CASE_NAME String_Char16
#elif defined(TEST_CHAR32_TYPE)
#define TEST_CASE_NAME String_Char32
#endif

#include "Framework/TestStringMacros.hpp"
#include "Framework/Framework.hpp"

#include <Opx/String/String.hpp>

TEST_CASE(String, TEST_CASE_NAME) {
    TEST_EXPECT_TRUE(true);
}