#pragma once

#include "../DataTypes.hpp"

#include <cstring>
#include <cwchar>

OPX_NAMESPACE_BEGIN

template <typename T>
class CharTraits {
public:
    using CharType = T;

    static SizeT Length(const CharType* str) {
        const CharType* current = str;
        while (*current) {
            ++current;
        }
        return static_cast<SizeT>(current - str);
    }

    static Int32 Compare(const CharType* lhs, const CharType* rhs, SizeT n) {
        while (n > 0) {
            if (*lhs != *rhs) {
                return (*lhs > *rhs) ? 1 : -1;
            }
            ++lhs, ++rhs, --n;
        }
        return 0;
    }

    static void Fill(CharType* dest, CharType c, SizeT n) {
        while (n > 0) {
            *dest = c;
            ++dest, --n;
        }
    }

    static const CharType* FindChar(const CharType* str, CharType c, SizeT n) {
        while (n > 0) {
            if (*str == c) {
                return str;
            }
            ++str, --n;
        }
        return nullptr;
    }

    static const CharType* FindLastChar(const CharType* str, CharType c, SizeT n) {
        const CharType* end = str + n;
        while (end != str) {
            --end;
            if (*end == c) {
                return end;
            }
        }
        return nullptr;
    }

    static const CharType* FindSubstr(const CharType* str,
                                      SizeT len,
                                      const CharType* substr,
                                      SizeT substrLen) {
        if (substrLen > len) {
            return nullptr;
        }

        if (substrLen == 0) {
            return str;
        }

        for (const CharType* end = str + (len - substrLen + 1); str != end; ++str) {
            if (*str == *substr && Compare(str, substr, substrLen) == 0) {
                return str;
            }
        }
        return nullptr;
    }

    static const CharType* FindLastSubstr(const CharType* str,
                                          SizeT len,
                                          const CharType* substr,
                                          SizeT substrLen) {
        if (substrLen > len) {
            return nullptr;
        }

        if (substrLen == 0) {
            return str + len;
        }

        for (const CharType* end = str + (len - substrLen); end != str; --end) {
            if (*end == *substr && Compare(end, substr, substrLen) == 0) {
                return end;
            }
        }
        return nullptr;
    }
};

OPX_NAMESPACE_END