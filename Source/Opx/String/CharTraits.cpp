#include "CharTraits.hpp"

OPX_NAMESPACE_BEGIN

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

OPX_NAMESPACE_END