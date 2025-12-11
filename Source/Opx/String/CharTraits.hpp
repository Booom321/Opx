#pragma once

#include "../DataTypes.hpp"
#include "../TypeTraits.hpp"

OPX_NAMESPACE_BEGIN

template <typename T>
class CharTraits {
public:
    static_assert(IsCharacter_V<T>, "T must be a character type.");

    using CharType = T;

    static OPX_CONSTEXPR Bool IsWhitespace(CharType c) {
        return c == static_cast<T>(0x20) || c == static_cast<T>(0x0c) ||
               c == static_cast<T>(0x0a) || c == static_cast<T>(0x0d) ||
               c == static_cast<T>(0x09) || c == static_cast<T>(0x0b);
    }

    static const CharType* Copy(CharType* dest, const CharType* src, SizeT count);
    static const CharType* Move(CharType* dest, const CharType* src, SizeT count);
    static SizeT Length(const CharType* str);
    static Int32 Compare(const CharType* lhs, const CharType* rhs, SizeT n);
    static void Fill(CharType* dest, CharType c, SizeT n);
    static const CharType* FindChar(const CharType* str, CharType c, SizeT n);
    static const CharType* FindLastChar(const CharType* str, CharType c, SizeT n);
    static const CharType* FindSubstr(const CharType* str,
                                      SizeT len,
                                      const CharType* substr,
                                      SizeT substrLen);
    static const CharType* FindLastSubstr(const CharType* str,
                                          SizeT len,
                                          const CharType* substr,
                                          SizeT substrLen);
};

OPX_NAMESPACE_END