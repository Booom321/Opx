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

Opx::Bool Expect(const Opx::BasicString<TEST_CHAR>& str,
                 const TEST_CHAR* expectedStr,
                 typename Opx::BasicString<TEST_CHAR>::SizeType expectedLen) {
    return str == expectedStr && str.GetSize() == expectedLen && str.GetCapacity() >= expectedLen;
}

#define TEST_STRING(str, expectedStr, expectedLen) \
    TEST_EXPECT_TRUE(Expect(str, TEST_TEXT(expectedStr), expectedLen))

TEST_CASE(String, TEST_CASE_NAME) {
    using String = Opx::BasicString<TEST_CHAR>;

    TEST_COMMENT("Constructor");
    {
        String s1{};
        TEST_STRING(s1, "", 0);
        TEST_EXPECT_EQ(s1.GetCapacity(), String::kInitialCapacity);

        String s2{TEST_TEXT("hello")};
        TEST_STRING(s2, "hello", 5);

        String s3{TEST_TEXT('a'), TEST_TEXT('b'), TEST_TEXT('c'), TEST_TEXT('d'), TEST_TEXT('e')};
        TEST_STRING(s3, "abcde", 5);

        String s4{TEST_TEXT("hello world"), 5};
        TEST_STRING(s4, "hello", 5);

        String s5(5, TEST_TEXT('a'));
        TEST_STRING(s5, "aaaaa", 5);

        String s6(String{TEST_TEXT("string")});
        TEST_STRING(s6, "string", 6);

        String s7(s6);
        TEST_STRING(s7, "string", 6);

        String s8(Move(s6));
        TEST_STRING(s8, "string", 6);
        TEST_STRING(s6, "", 0);

        String s9({});
        TEST_STRING(s9, "", 0);

        String s10(TEST_TEXT(""));
        TEST_STRING(s10, "", 0);

        // String s9(nullptr); // error!!
    }

    TEST_COMMENT("Assign");
    {
        String s{};
        String empty;
        String other{TEST_TEXT("hello world")};

        s = TEST_TEXT("this is a string");
        TEST_STRING(s, "this is a string", 16);

        s = TEST_TEXT("foo");
        TEST_STRING(s, "foo", 3);

        s = TEST_TEXT("");
        TEST_STRING(s, "", 0);

        s = TEST_TEXT("this is a very very very very very very very very very very long string");
        TEST_STRING(s, "this is a very very very very very very very very very very long string",
                    71);

        s = TEST_TEXT('a');
        TEST_STRING(s, "a", 1);

        s = other;
        TEST_STRING(s, "hello world", 11);

        s = Move(other);
        TEST_STRING(s, "hello world", 11);
        TEST_STRING(other, "", 0);

        s = String(TEST_TEXT("a string"));
        TEST_STRING(s, "a string", 8);

        s = empty;
        TEST_STRING(s, "", 0);

        // s = nullptr; // error!!
    }

    TEST_COMMENT("String data");
    {
        {
            String s{TEST_TEXT("0123456789")};

            TEST_STRING(s, "0123456789", 10);
            TEST_EXPECT_EQ(s.GetSize(), 10);
            TEST_EXPECT_FALSE(s.IsEmpty());

            TEST_EXPECT_EQ(s[0], TEST_TEXT('0'));
            TEST_EXPECT_EQ(s[4], TEST_TEXT('4'));
            TEST_EXPECT_EQ(s[7], TEST_TEXT('7'));

            TEST_EXPECT_EQ(s.At(0), TEST_TEXT('0'));
            TEST_EXPECT_EQ(s.At(4), TEST_TEXT('4'));
            TEST_EXPECT_EQ(s.At(7), TEST_TEXT('7'));

            TEST_EXPECT_NEQ(s.GetData(), nullptr);
            TEST_EXPECT_EQ(s.First(), TEST_TEXT('0'));
            TEST_EXPECT_EQ(s.Last(), TEST_TEXT('9'));

            s[0] = TEST_TEXT('a');
            s.At(1) = TEST_TEXT('b');
            TEST_STRING(s, "ab23456789", 10);
        }

        {
            const String s{TEST_TEXT("0123456789")};

            TEST_STRING(s, "0123456789", 10);
            TEST_EXPECT_EQ(s.GetSize(), 10);
            TEST_EXPECT_FALSE(s.IsEmpty());

            TEST_EXPECT_EQ(s[0], TEST_TEXT('0'));
            TEST_EXPECT_EQ(s[4], TEST_TEXT('4'));
            TEST_EXPECT_EQ(s[7], TEST_TEXT('7'));

            TEST_EXPECT_EQ(s.At(0), TEST_TEXT('0'));
            TEST_EXPECT_EQ(s.At(4), TEST_TEXT('4'));
            TEST_EXPECT_EQ(s.At(7), TEST_TEXT('7'));

            TEST_EXPECT_NEQ(s.GetData(), nullptr);
            TEST_EXPECT_EQ(s.First(), TEST_TEXT('0'));
            TEST_EXPECT_EQ(s.Last(), TEST_TEXT('9'));
        }

        String empty;
        TEST_EXPECT_TRUE(empty.IsEmpty());
    }

    TEST_COMMENT("Clear, Reserve, Shrink, Resize");
    {
        String s;
        TEST_EXPECT_EQ(s.GetCapacity(), String::kInitialCapacity);

        s.Reserve(16);
        TEST_EXPECT_EQ(s.GetCapacity(), String::kInitialCapacity);

        s.Reserve(48);
        TEST_EXPECT_EQ(s.GetCapacity(), 48);

        s.Resize(10, TEST_TEXT('a'));
        TEST_STRING(s, "aaaaaaaaaa", 10);

        s.Shrink();
        TEST_STRING(s, "aaaaaaaaaa", 10);
        TEST_EXPECT_EQ(s.GetCapacity(), s.GetSize());

        s.Clear();
        TEST_STRING(s, "", 0);

        s.Clear();
        TEST_STRING(s, "", 0);

        String empty;
        empty.Clear();
        TEST_STRING(empty, "", 0);
    }

    TEST_COMMENT("Comparision");
    {
        const String s1{TEST_TEXT("abcdef")};
        const String s2{TEST_TEXT("abcdef")};
        const String s3{TEST_TEXT("xyzxyz")};
        const String s4{TEST_TEXT("string")};
        const String empty;

        TEST_COMMENT("operators");
        {
            TEST_EXPECT_EQ(s1, s2);
            TEST_EXPECT_NEQ(s1, s3);

            TEST_EXPECT_EQ(s1, TEST_TEXT("abcdef"));
            TEST_EXPECT_NEQ(s1, TEST_TEXT("0123456"));

            TEST_EXPECT_EQ(TEST_TEXT("string"), s4);
            TEST_EXPECT_NEQ(TEST_TEXT("abcdef"), s4);

            TEST_EXPECT_NEQ(s1, empty);
            TEST_EXPECT_NEQ(empty, s1);
        }

        TEST_COMMENT("Compare");
        {
            TEST_EXPECT_EQ(s1.Compare(s2), 0);
            TEST_EXPECT_EQ(s1.Compare(s3), -1);
            TEST_EXPECT_EQ(s3.Compare(s1), 1);

            TEST_EXPECT_EQ(s1.Compare(TEST_TEXT("zzzzzzzzzz")), -1);
            TEST_EXPECT_EQ(s1.Compare(TEST_TEXT("aaaaaaa")), 1);
            TEST_EXPECT_EQ(s3.Compare(TEST_TEXT("xyzxyz")), 0);

            const String def{TEST_TEXT("def")};
            TEST_EXPECT_EQ(s1.Compare(3, 3, def), 0);
            TEST_EXPECT_EQ(s3.Compare(3, 3, TEST_TEXT("def")), 1);

            TEST_EXPECT_EQ(s1.Compare(empty), 1);
            TEST_EXPECT_EQ(empty.Compare(s1), -1);
            TEST_EXPECT_EQ(empty.Compare(TEST_TEXT("")), 0);
            TEST_EXPECT_EQ(empty.Compare(TEST_TEXT("abcdef")), -1);

            const String s01{TEST_TEXT("def abc def")};
            const String s02{TEST_TEXT("abc def abc def abc")};
            TEST_EXPECT_EQ(s01.Compare(4, 3, s02, 0, 3), 0);
            TEST_EXPECT_EQ(s01.Compare(4, 7, s02, 8, 9), -1);
            TEST_EXPECT_EQ(s01.Compare(4, 7, s02, 8, 4), 1);

            TEST_EXPECT_EQ(s01.Compare(0, 3, TEST_TEXT("abc def abc"), 4, 3), 0);
        }
    }

    TEST_COMMENT("StartsWith, EndsWith");
    {
        const String s{TEST_TEXT("abcdef abcdef")};
        const String prefix{TEST_TEXT("abc")};
        const String suffix{TEST_TEXT("def")};

        TEST_EXPECT_TRUE(s.StartsWith(prefix));
        TEST_EXPECT_TRUE(s.StartsWith(prefix, 7));
        TEST_EXPECT_TRUE(s.StartsWith(TEST_TEXT("")));
        TEST_EXPECT_TRUE(s.StartsWith(TEST_TEXT(""), 4));
        TEST_EXPECT_TRUE(s.StartsWith(TEST_TEXT("abc")));
        TEST_EXPECT_TRUE(s.StartsWith(TEST_TEXT("abc"), 7));
        TEST_EXPECT_TRUE(s.StartsWith(TEST_TEXT("abcdef"), 7));
        TEST_EXPECT_TRUE(s.StartsWith(TEST_TEXT('a')));
        TEST_EXPECT_TRUE(s.StartsWith(TEST_TEXT('c'), 2));
        TEST_EXPECT_FALSE(s.StartsWith(TEST_TEXT('$'), 2));
        TEST_EXPECT_FALSE(s.StartsWith(TEST_TEXT("abcdef"), 9));

        TEST_EXPECT_TRUE(s.EndsWith(suffix));
        TEST_EXPECT_TRUE(s.EndsWith(suffix, 5));
        TEST_EXPECT_TRUE(s.EndsWith(TEST_TEXT("cdef")));
        TEST_EXPECT_TRUE(s.EndsWith(TEST_TEXT("cdef"), 5));
        TEST_EXPECT_TRUE(s.EndsWith(TEST_TEXT('f')));
        TEST_EXPECT_TRUE(s.EndsWith(TEST_TEXT('f'), 5));
        TEST_EXPECT_TRUE(s.EndsWith(TEST_TEXT("")));
        TEST_EXPECT_TRUE(s.EndsWith(TEST_TEXT(""), 5));
        TEST_EXPECT_FALSE(s.EndsWith(TEST_TEXT('s')));
        TEST_EXPECT_FALSE(s.EndsWith(TEST_TEXT("1234")));
        TEST_EXPECT_FALSE(s.EndsWith(TEST_TEXT("1234"), 3));

        const String empty;
        TEST_EXPECT_TRUE(empty.StartsWith(TEST_TEXT("")));
        TEST_EXPECT_TRUE(empty.EndsWith(TEST_TEXT("")));
        TEST_EXPECT_FALSE(empty.StartsWith(TEST_TEXT("abc")));
        TEST_EXPECT_FALSE(empty.EndsWith(TEST_TEXT("abc")));
    }

    TEST_COMMENT("Push");
    {
        String s;
        s.Push(TEST_TEXT('a'));
        s.Push(TEST_TEXT('b'));
        s.Push(TEST_TEXT('c'));
        s.Push(TEST_TEXT('d'));
        s.Push(TEST_TEXT('e'));
        s.Push(TEST_TEXT('f'));
        s.Shrink();
        s.Push(TEST_TEXT('0'));
        s.Push(TEST_TEXT('1'));
        s.Push(TEST_TEXT('2'));
        s.Push(TEST_TEXT('3'));
        s.Push(TEST_TEXT('4'));
        s.Push(TEST_TEXT('5'));
        s.Push(TEST_TEXT('6'));
        s.Push(TEST_TEXT('7'));
        TEST_STRING(s, "abcdef01234567", 14);
    }

    TEST_COMMENT("Append, operator+=");
    {
        String text{TEST_TEXT("a text")};
        String empty;
        {
            String s;
            s.Append(TEST_TEXT("abc"));
            s.Append(TEST_TEXT(" abc "), 5);
            s.Append(text);
            s.Append(text, 1, 5);
            s.Append(5, TEST_TEXT('a'));
            s.Append(TEST_TEXT(""));
            s.Append(empty);
            TEST_STRING(s, "abc abc a text textaaaaa", 24);
        }

        {
            String s;
            s += TEST_TEXT("abc ");
            s += text;
            s += empty;
            s += ' ';
            s += 'a';
            s += TEST_TEXT(" abc abc");
            s += TEST_TEXT("");
            TEST_STRING(s, "abc a text a abc abc", 20);
        }

        {
            String s;
            s.AppendChars(TEST_TEXT('a'), TEST_TEXT('b'), TEST_TEXT('c'), 40, TEST_TEXT(':'), 41);
            TEST_STRING(s, "abc(:)", 6);
            s.AppendChars(91, TEST_TEXT('b'), TEST_TEXT('c'), 93, TEST_TEXT('x'), 41);
            TEST_STRING(s, "abc(:)[bc]x)", 12);
        }
    }

    TEST_COMMENT("Insert");
    {
        {
            String text{TEST_TEXT("a text")};
            String empty;
            String s;
            s.Insert(0, TEST_TEXT("abc"));
            s.Insert(3, TEST_TEXT("abcdef"), 4);
            s.Insert(2, TEST_TEXT("abc"));
            s.Insert(5, text);
            s.Insert(4, text, 2, 4);
            s.Insert(7, TEST_TEXT(""));
            s.Insert(11, empty);
            TEST_STRING(s, "ababtextca textcabcd", 20);

            s.Insert(1, 6, TEST_TEXT('A'));
            s.Insert(1, 0, TEST_TEXT('A'));
            TEST_STRING(s, "aAAAAAAbabtextca textcabcd", 26);
        }
        {
            String s;
            s.InsertChars(0, TEST_TEXT('a'), TEST_TEXT('b'), TEST_TEXT('c'), 40, TEST_TEXT(':'),
                          41);
            TEST_STRING(s, "abc(:)", 6);
            s.InsertChars(3, TEST_TEXT('a'), TEST_TEXT('b'), TEST_TEXT('c'), 124, TEST_TEXT(':'),
                          124);
            TEST_STRING(s, "abcabc|:|(:)", 12);
            s.InsertChars(7, 91, TEST_TEXT('b'), TEST_TEXT('c'), 93, TEST_TEXT('x'), 41);
            TEST_STRING(s, "abcabc|[bc]x):|(:)", 18);
        }
    }

    TEST_COMMENT("operator+");
    {
        const String s1{TEST_TEXT("111")};
        const String s2{TEST_TEXT("222")};
        const String empty;

        {
            String s;

            s = s1 + s2;
            TEST_STRING(s, "111222", 6);

            s = s1 + TEST_TEXT("AAA");
            TEST_STRING(s, "111AAA", 6);

            s = s1 + String{TEST_TEXT("abc123")};
            TEST_STRING(s, "111abc123", 9);

            s = s1 + 'a';
            TEST_STRING(s, "111a", 4);

            s = s1 + TEST_TEXT("");
            TEST_STRING(s, "111", 3);

            s = s1 + empty;
            TEST_STRING(s, "111", 3);

            s = empty + s1;
            TEST_STRING(s, "111", 3);

            s = TEST_TEXT("HELLO") + s1;
            TEST_STRING(s, "HELLO111", 8);

            s = TEST_TEXT('0') + s1;
            TEST_STRING(s, "0111", 4);

            s = TEST_TEXT("") + s1;
            TEST_STRING(s, "111", 3);
        }
        {
            String s;

            s = String{TEST_TEXT("abc")} + s1;
            TEST_STRING(s, "abc111", 6);

            s = String{TEST_TEXT("abc")} + TEST_TEXT("HELLO");
            TEST_STRING(s, "abcHELLO", 8);

            s = String{TEST_TEXT("abc")} + String{TEST_TEXT(" abc")};
            TEST_STRING(s, "abc abc", 7);

            s = String{TEST_TEXT("abc")} + 'b';
            TEST_STRING(s, "abcb", 4);

            s = String{TEST_TEXT("")} + String{TEST_TEXT("abc")};
            TEST_STRING(s, "abc", 3);

            s = String{TEST_TEXT("abc")} + TEST_TEXT("");
            TEST_STRING(s, "abc", 3);

            s = String{TEST_TEXT("abc")} + empty;
            TEST_STRING(s, "abc", 3);

            s = String{TEST_TEXT("")} + String{TEST_TEXT("")};
            TEST_STRING(s, "", 0);

            s = TEST_TEXT("Foo") + String{TEST_TEXT("Bar")};
            TEST_STRING(s, "FooBar", 6);

            s = TEST_TEXT("") + String{TEST_TEXT("text")};
            TEST_STRING(s, "text", 4);

            s = TEST_TEXT('a') + String{TEST_TEXT(" text")};
            TEST_STRING(s, "a text", 6);
        }
    }

    TEST_COMMENT("Repeat, operator*, operator*=");
    {
        {
            String empty;
            empty.Repeat(0);
            TEST_STRING(empty, "", 0);
            empty.Repeat(1);
            TEST_STRING(empty, "", 0);
            empty.Repeat(4);
            TEST_STRING(empty, "", 0);
            empty.Repeat(10);
            TEST_STRING(empty, "", 0);
        }

        {
            String s{TEST_TEXT("abc!")};
            s.Repeat(3);
            TEST_STRING(s, "abc!abc!abc!", 12);

            s.Repeat(1);
            TEST_STRING(s, "abc!abc!abc!", 12);

            s.Repeat(3);
            TEST_STRING(s, "abc!abc!abc!abc!abc!abc!abc!abc!abc!", 36);

            s.Repeat(0);
            TEST_STRING(s, "", 0);
        }

        {
            String s{TEST_TEXT("abc!")};

            String s1{s * 3};
            TEST_STRING(s1, "abc!abc!abc!", 12);

            String s2{s * 1};
            TEST_STRING(s2, "abc!", 4);

            String s3{s * 0};
            TEST_STRING(s3, "", 0);

            String s4{String{} * 3};
            TEST_STRING(s4, "", 0);

            String s5{4 * s};
            TEST_STRING(s5, "abc!abc!abc!abc!", 16);

            String s6{String{TEST_TEXT("lol!")} * 3};
            TEST_STRING(s6, "lol!lol!lol!", 12);

            String s7{2 * String{TEST_TEXT("lol!")} * 3};
            TEST_STRING(s7, "lol!lol!lol!lol!lol!lol!", 24);
        }

        {
            String s{TEST_TEXT("abc!")};

            s *= 3;
            TEST_STRING(s, "abc!abc!abc!", 12);

            s *= 1;
            TEST_STRING(s, "abc!abc!abc!", 12);

            s *= 0;
            TEST_STRING(s, "", 0);
        }
    }

    TEST_COMMENT("RemoveAt, Pop");
    {
        {
            String s{TEST_TEXT("abcdef01234567")};
            s.RemoveAt(3, 3);
            TEST_STRING(s, "abc01234567", 11);
        }
        {
            String s{TEST_TEXT("abcdef01234567")};
            TEST_EXPECT_EQ(s.Pop(), TEST_TEXT('7'));
            TEST_STRING(s, "abcdef0123456", 13);
        }
    }

    TEST_COMMENT("Trim");
    {
        {
            String s{TEST_TEXT("  \t\f\n\rabc\t\v\n")};
            s.Trim();
            TEST_STRING(s, "abc", 3);
        }
        {
            String s{TEST_TEXT("abc")};
            s.Trim();
            TEST_STRING(s, "abc", 3);
        }
        {
            String s{TEST_TEXT("  \t\f\n\r\t\v\n")};
            s.Trim();
            TEST_STRING(s, "", 0);
        }
        {
            String s{TEST_TEXT("")};
            s.Trim();
            TEST_STRING(s, "", 0);
        }
    }

    TEST_COMMENT("Reverse, Slice");
    {
        String empty;
        empty.Reverse();
        TEST_STRING(empty, "", 0);

        String s{TEST_TEXT("abcdef 01234567")};
        s.Reverse();
        TEST_STRING(s, "76543210 fedcba", 15);

        {
            s = TEST_TEXT("abcdef 01234567");
            String digits{s.Slice(7, 8)};
            TEST_STRING(digits, "01234567", 8);
        }

        {
            s = TEST_TEXT("abcdef 01234567");
            String digits{s.Slice(7)};
            TEST_STRING(digits, "01234567", 8);
        }
    }
}