#if defined(TEST_BUILD_SHARED_LIBS)
#if defined(TEST_DLL_EXPORT)
#define TEST_API TEST_EXPORT
#else
#define TEST_API TEST_IMPORT
#endif
#else
#define TEST_API
#endif

#include <cstddef>

using Byte = unsigned char;

using UInt8 = unsigned char;
using UInt16 = unsigned short;
using UInt32 = unsigned int;
using UInt64 = unsigned long long;

using Int8 = signed char;
using Int16 = signed short;
using Int32 = signed int;
using Int64 = signed long long;

using SizeT = std::size_t;

using Float = float;
using Double = double;

using Bool = bool;

using Char = char;
using WChar = wchar_t;
#if defined(__cpp_char8_t)
using Char8 = char8_t;
#endif
using Char16 = char16_t;
using Char32 = char32_t;

using NullPtr = std::nullptr_t;