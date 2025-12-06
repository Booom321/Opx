#pragma once

#include "../DataTypes.hpp"

#include <cstring>
#include <cwchar>

OPX_NAMESPACE_BEGIN

namespace Details {
    template <typename T>
    class CharTraitsBase {
    public:
        using CharType = T;

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
    };
}  // namespace Details

template <typename T>
class CharTraits : public Details::CharTraitsBase<T> {
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
};

template <>
class CharTraits<Char> : public Details::CharTraitsBase<Char> {
public:
    using CharType = Char;

    static SizeT Length(const CharType* str) {
#if defined(OPX_COMPILER_GCC) || defined(OPX_COMPILER_CLANG)
        return __builtin_strlen(str);
#else
        return strlen(str);
#endif
    }

    static Int32 Compare(const CharType* lhs, const CharType* rhs, SizeT n) {
#if defined(OPX_COMPILER_GCC) || defined(OPX_COMPILER_CLANG)
        return __builtin_memcmp(lhs, rhs, n);
#else
        return memcmp(lhs, rhs, n);
#endif
    }

    static void Fill(CharType* dest, CharType c, SizeT n) {
#if defined(OPX_COMPILER_GCC) || defined(OPX_COMPILER_CLANG)
        __builtin_memset(dest, c, n);
#else
        memset(dest, c, n);
#endif
    }

    static const CharType* FindChar(const CharType* str, CharType c, SizeT n) {
#if defined(OPX_COMPILER_GCC) || defined(OPX_COMPILER_CLANG)
        return static_cast<const CharType*>(__builtin_memchr(str, c, n));
#else
        return static_cast<const CharType*>(memchr(str, c, n));
#endif
    }
};

template <>
class CharTraits<WChar> : public Details::CharTraitsBase<WChar> {
public:
    using CharType = WChar;

    static SizeT Length(const CharType* str) {
#if defined(OPX_COMPILER_CLANG)
        return __builtin_wcslen(str);
#else
        return wcslen(str);
#endif
    }

    static Int32 Compare(const CharType* lhs, const CharType* rhs, SizeT n) {
#if defined(OPX_COMPILER_CLANG)
        return __builtin_wmemcmp(lhs, rhs, n);
#else
        return wmemcmp(lhs, rhs, n);
#endif
    }

    static void Fill(CharType* dest, CharType c, SizeT n) { wmemset(dest, c, n); }

    static const CharType* FindChar(const CharType* str, CharType c, SizeT n) {
#if defined(OPX_COMPILER_CLANG)
        return __builtin_wmemchr(str, c, n);
#else
        return wmemchr(str, c, n);
#endif
    }
};

OPX_NAMESPACE_END