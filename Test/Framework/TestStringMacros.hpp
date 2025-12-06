#pragma once

#undef TEST_TEXT
#undef TEST_CHAR

#if defined(TEST_CHAR_TYPE)
#define TEST_TEXT(text) text
#define TEST_CHAR char
#undef TEST_CHAR_TYPE
#endif

#if defined(TEST_WCHAR_TYPE)
#define TEST_TEXT(text) L##text
#define TEST_CHAR wchar_t
#undef TEST_WCHAR_TYPE
#endif

#if defined(__cpp_char8_t)
#if defined(TEST_CHAR8_TYPE)
#define TEST_TEXT(text) u8##text
#define TEST_CHAR char8_t
#undef TEST_CHAR8_TYPE
#endif
#endif

#if defined(TEST_CHAR16_TYPE)
#define TEST_TEXT(text) u##text
#define TEST_CHAR char16_t
#undef TEST_CHAR16_TYPE
#endif

#if defined(TEST_CHAR32_TYPE)
#define TEST_TEXT(text) U##text
#define TEST_CHAR char32_t
#undef TEST_CHAR32_TYPE
#endif
