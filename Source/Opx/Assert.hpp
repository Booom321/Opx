#pragma once

#include "DataTypes.hpp"

OPX_NAMESPACE_BEGIN

using AssertHandler = void (*)(const Char* expr, const Char* file, const Char* function, Int32 line, const Char* message);

OPX_API void SetAssertHandler(AssertHandler handler);
OPX_API void ReportAssertionFailure(const Char* expr, const Char* file, const Char* function, Int32 line, const Char* message, ...);

OPX_NAMESPACE_END

#define OPX_ALWAYS_ASSERT(expr)                                                          \
    do {                                                                                 \
        if (!(expr)) {                                                                   \
            ::Opx::ReportAssertionFailure(#expr, OPX_FILE, OPX_FUNC, OPX_LINE, nullptr); \
            OPX_DEBUG_BREAK();                                                           \
        }                                                                                \
    } while (0)

#define OPX_ALWAYS_ASSERT_MSG(expr, ...)                                                     \
    do {                                                                                     \
        if (!(expr)) {                                                                       \
            ::Opx::ReportAssertionFailure(#expr, OPX_FILE, OPX_FUNC, OPX_LINE, __VA_ARGS__); \
            OPX_DEBUG_BREAK();                                                               \
        }                                                                                    \
    } while (0)

#define OPX_ENABLE_ASSERTIONS 1

#if defined(OPX_ENABLE_ASSERTIONS) && OPX_ENABLE_ASSERTIONS
#define OPX_ASSERT(expr) OPX_ALWAYS_ASSERT(expr)
#define OPX_ASSERT_MSG(expr, ...) OPX_ALWAYS_ASSERT_MSG(expr, __VA_ARGS__)
#else
#define OPX_ASSERT(expr)
#define OPX_ASSERT_MSG(expr, ...)
#endif