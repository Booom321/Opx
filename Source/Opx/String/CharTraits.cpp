#include "CharTraits.hpp"

#include <cstring>
#include <cwchar>

OPX_NAMESPACE_BEGIN

template <typename T>
const typename CharTraits<T>::CharType* CharTraits<T>::Copy(CharType* dest,
                                                            const CharType* src,
                                                            SizeT count) {
#if defined(OPX_COMPILER_GCC) || defined(OPX_COMPILER_CLANG)
    return static_cast<const CharType*>(__builtin_memcpy(dest, src, count * sizeof(CharType)));
#else
    return static_cast<const CharType*>(memcpy(dest, src, count * sizeof(CharType)));
#endif
}

template <typename T>
const typename CharTraits<T>::CharType* CharTraits<T>::Move(CharType* dest,
                                                            const CharType* src,
                                                            SizeT count) {
    if (dest < src) {
#if defined(OPX_COMPILER_GCC) || defined(OPX_COMPILER_CLANG)
        return static_cast<const CharType*>(__builtin_memcpy(dest, src, count * sizeof(CharType)));
#else
        return static_cast<const CharType*>(memcpy(dest, src, count * sizeof(CharType)));
#endif
    }
#if defined(OPX_COMPILER_GCC) || defined(OPX_COMPILER_CLANG)
    return static_cast<const CharType*>(__builtin_memmove(dest, src, count * sizeof(CharType)));
#else
    return static_cast<const CharType*>(memmove(dest, src, count * sizeof(CharType)));
#endif
}

template <typename T>
SizeT CharTraits<T>::Length(const CharType* str) {
    const CharType* current = str;
    while (*current) {
        ++current;
    }
    return static_cast<SizeT>(current - str);
}

template <typename T>
Int32 CharTraits<T>::Compare(const CharType* lhs, const CharType* rhs, SizeT n) {
    while (n > 0) {
        if (*lhs != *rhs) {
            return (*lhs > *rhs) ? 1 : -1;
        }
        ++lhs, ++rhs, --n;
    }
    return 0;
}

template <typename T>
void CharTraits<T>::Fill(CharType* dest, CharType c, SizeT n) {
    while (n > 0) {
        *dest = c;
        ++dest, --n;
    }
}

template <typename T>
const typename CharTraits<T>::CharType* CharTraits<T>::FindChar(const CharType* str,
                                                                CharType c,
                                                                SizeT n) {
    while (n > 0) {
        if (*str == c) {
            return str;
        }
        ++str, --n;
    }
    return nullptr;
}

template <typename T>
const typename CharTraits<T>::CharType* CharTraits<T>::FindLastChar(const CharType* str,
                                                                    CharType c,
                                                                    SizeT n) {
    const CharType* end = str + n;
    while (end != str) {
        --end;
        if (*end == c) {
            return end;
        }
    }
    return nullptr;
}

template <typename T>
const typename CharTraits<T>::CharType* CharTraits<T>::FindSubstr(const CharType* str,
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

template <typename T>
const typename CharTraits<T>::CharType* CharTraits<T>::FindLastSubstr(const CharType* str,
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

/////// CHAR ///////
template <>
SizeT CharTraits<Char>::Length(const CharType* str) {
#if defined(OPX_COMPILER_GCC) || defined(OPX_COMPILER_CLANG)
    return __builtin_strlen(str);
#else
    return strlen(str);
#endif
}

template <>
Int32 CharTraits<Char>::Compare(const CharType* lhs, const CharType* rhs, SizeT n) {
#if defined(OPX_COMPILER_GCC) || defined(OPX_COMPILER_CLANG)
    return __builtin_memcmp(lhs, rhs, n);
#else
    return memcmp(lhs, rhs, n);
#endif
}

template <>
void CharTraits<Char>::Fill(CharType* dest, CharType c, SizeT n) {
#if defined(OPX_COMPILER_GCC) || defined(OPX_COMPILER_CLANG)
    __builtin_memset(dest, c, n);
#else
    memset(dest, c, n);
#endif
}

template <>
const typename CharTraits<Char>::CharType* CharTraits<Char>::FindChar(const CharType* str,
                                                                      CharType c,
                                                                      SizeT n) {
#if defined(OPX_COMPILER_GCC) || defined(OPX_COMPILER_CLANG)
    return static_cast<const CharType*>(__builtin_memchr(str, c, n));
#else
    return static_cast<const CharType*>(memchr(str, c, n));
#endif
}

/////// WCHAR ///////
template <>
SizeT CharTraits<WChar>::Length(const CharType* str) {
#if defined(OPX_COMPILER_CLANG)
    return __builtin_wcslen(str);
#else
    return wcslen(str);
#endif
}

template <>
Int32 CharTraits<WChar>::Compare(const CharType* lhs, const CharType* rhs, SizeT n) {
#if defined(OPX_COMPILER_CLANG)
    return __builtin_wmemcmp(lhs, rhs, n);
#else
    return wmemcmp(lhs, rhs, n);
#endif
}

template <>
void CharTraits<WChar>::Fill(CharType* dest, CharType c, SizeT n) {
    wmemset(dest, c, n);
}

template <>
const typename CharTraits<WChar>::CharType* CharTraits<WChar>::FindChar(const CharType* str,
                                                                        CharType c,
                                                                        SizeT n) {
#if defined(OPX_COMPILER_CLANG)
    return __builtin_wmemchr(str, c, n);
#else
    return wmemchr(str, c, n);
#endif
}

template class CharTraits<Char>;
template class CharTraits<WChar>;
#if defined(OPX_HAS_CHAR18_T)
template class CharTraits<Char8>;
#endif
template class CharTraits<Char16>;
template class CharTraits<Char32>;

OPX_NAMESPACE_END