#pragma once

#if defined(TEST_CHAR_TYPE)
#define TEST_CASE_NAME CharTraits_Char
#elif defined(TEST_WCHAR_TYPE)
#define TEST_CASE_NAME CharTraits_WChar
#elif defined(TEST_CHAR8_TYPE)
#define TEST_CASE_NAME CharTraits_Char8
#elif defined(TEST_CHAR16_TYPE)
#define TEST_CASE_NAME CharTraits_Char16
#elif defined(TEST_CHAR32_TYPE)
#define TEST_CASE_NAME CharTraits_Char32
#endif

#include "Framework/TestStringMacros.hpp"
#include "Framework/Framework.hpp"

#include <Opx/String/CharTraits.hpp>

TEST_CASE(String, TEST_CASE_NAME) {
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

    {
        const auto testFindSubstr = [](const TEST_CHAR* str, SizeT len, const TEST_CHAR* substr,
                                       SizeT substrLen, Opx::PtrOffsetT expectedIndex) -> Bool {
            const auto* found = CharTraits::FindSubstr(str, len, substr, substrLen);
            return expectedIndex < 0 ? (found == nullptr)
                                     : (found != nullptr && found == str + expectedIndex);
        };

        const auto testFindLastSubstr = [](const TEST_CHAR* str, SizeT len, const TEST_CHAR* substr,
                                           SizeT substrLen, Opx::PtrOffsetT expectedIndex) -> Bool {
            const auto* found = CharTraits::FindLastSubstr(str, len, substr, substrLen);
            return expectedIndex < 0 ? (found == nullptr)
                                     : (found != nullptr && found == str + expectedIndex);
        };

#define TEST_FIND_SUBSTR(str, len, substr, substrLen, expectedIndex) \
    TEST_EXPECT_TRUE(                                                \
        testFindSubstr(TEST_TEXT(str), len, TEST_TEXT(substr), substrLen, expectedIndex));
#define TEST_FIND_LAST_SUBSTR(str, len, substr, substrLen, expectedIndex) \
    TEST_EXPECT_TRUE(                                                     \
        testFindLastSubstr(TEST_TEXT(str), len, TEST_TEXT(substr), substrLen, expectedIndex));

        TEST_FIND_SUBSTR("aa", 2, "aaaaa", 5, -1);
        TEST_FIND_SUBSTR("abcdef", 4, "cd", 2, 2);
        TEST_FIND_SUBSTR("aaaaa", 5, "bbb", 3, -1);
        TEST_FIND_SUBSTR("aaaaa", 5, "aaaaa", 5, 0);
        TEST_FIND_SUBSTR("aaaaa", 5, "aaaaaa", 6, -1);
        TEST_FIND_SUBSTR("aaaaa", 5, "", 0, 0);
        TEST_FIND_SUBSTR("aaaaa", 5, "bbb", 0, 0);

        TEST_FIND_LAST_SUBSTR("aaa bbb aaa ccc", 15, "aaa", 3, 8);
        TEST_FIND_LAST_SUBSTR("aaa bbb aaa ccc", 15, "bbb", 3, 4);
        TEST_FIND_LAST_SUBSTR("aaa bbb aaa ccc", 15, "", 0, 15);
        TEST_FIND_LAST_SUBSTR("aaa bbb aaa ccc", 15, "ccc", 0, 15);
    }
}