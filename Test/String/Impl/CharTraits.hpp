#pragma once

#if defined(TEST_CHAR_TYPE)
#define TEST_CASE_CHAR_TRAITS_NAME CharTraits_Char
#elif defined(TEST_WCHAR_TYPE)
#define TEST_CASE_CHAR_TRAITS_NAME CharTraits_WChar
#elif defined(TEST_CHAR8_TYPE)
#define TEST_CASE_CHAR_TRAITS_NAME CharTraits_Char8
#elif defined(TEST_CHAR16_TYPE)
#define TEST_CASE_CHAR_TRAITS_NAME CharTraits_Char16
#elif defined(TEST_CHAR32_TYPE)
#define TEST_CASE_CHAR_TRAITS_NAME CharTraits_Char32
#endif

#include "Framework/TestStringMacros.hpp"
#include "Framework/Framework.hpp"

#include <Opx/String/CharTraits.hpp>

TEST_CASE(String, TEST_CASE_CHAR_TRAITS_NAME) {
    using CharTraits = Opx::CharTraits<TEST_CHAR>;

    TEST_EXPECT_EQ(CharTraits::Length(TEST_TEXT("Hello")), 5);
    TEST_EXPECT_EQ(CharTraits::Length(TEST_TEXT("v")), 1);
    TEST_EXPECT_EQ(CharTraits::Length(TEST_TEXT("")), 0);

    TEST_EXPECT_EQ(CharTraits::Compare(TEST_TEXT(""), TEST_TEXT(""), 0), 0);
    TEST_EXPECT_EQ(CharTraits::Compare(TEST_TEXT("Hello"), TEST_TEXT("Hello"), 5), 0);
    TEST_EXPECT_EQ(CharTraits::Compare(TEST_TEXT("HelloXYZ"), TEST_TEXT("HelloABC"), 0), 0);
    TEST_EXPECT_EQ(CharTraits::Compare(TEST_TEXT("HelloABC"), TEST_TEXT("HelloXYZ"), 5), 0);
    TEST_EXPECT_EQ(CharTraits::Compare(TEST_TEXT("HelloXYZ"), TEST_TEXT("HelloABC"), 7), 1);
    TEST_EXPECT_EQ(CharTraits::Compare(TEST_TEXT("HelloABC"), TEST_TEXT("HelloXYZ"), 7), -1);

    {
        TEST_CHAR str[] = TEST_TEXT("string");
        CharTraits::Fill(str, TEST_TEXT('a'), CharTraits::Length(str));
        TEST_EXPECT_EQ(CharTraits::Compare(str, TEST_TEXT("aaaaaa"), 0), 0);
    }

    {
        const TEST_CHAR* s1 = TEST_TEXT("Hello");
        const TEST_CHAR* s2 = TEST_TEXT("");
        const SizeT s1n = CharTraits::Length(s1);
        const SizeT s2n = CharTraits::Length(s2);

        TEST_EXPECT_EQ(CharTraits::FindChar(s1, 'l', s1n), s1 + 2);
        TEST_EXPECT_EQ(CharTraits::FindChar(s1, 'x', s1n), nullptr);
        TEST_EXPECT_EQ(CharTraits::FindLastChar(s1, 'l', s1n), s1 + 3);
        TEST_EXPECT_EQ(CharTraits::FindLastChar(s1, 'x', s1n), nullptr);

        TEST_EXPECT_EQ(CharTraits::FindChar(s2, 'l', s2n), nullptr);
        TEST_EXPECT_EQ(CharTraits::FindChar(s2, 'x', s2n), nullptr);
        TEST_EXPECT_EQ(CharTraits::FindLastChar(s2, 'l', s2n), nullptr);
        TEST_EXPECT_EQ(CharTraits::FindLastChar(s2, 'x', s2n), nullptr);
    }
}