#pragma once

#define OPX_CPLUSPLUS __cplusplus

#if OPX_CPLUSPLUS >= 202302L
#define OPX_CPP_23 1
#define OPX_CPP_VERSION 230
#elif __cplusplus >= 202002L
#define OPX_CPP_20 1
#define OPX_CPP_VERSION 200
#elif __cplusplus >= 201703L
#define OPX_CPP_17 1
#define OPX_CPP_VERSION 170
#elif __cplusplus >= 201402L
#define OPX_CPP_14 1
#define OPX_CPP_VERSION 140
#elif __cplusplus >= 201103L
#define OPX_CPP_11 1
#define OPX_CPP_VERSION 110
#else
#define OPX_CPP_98 1
#define OPX_CPP_VERSION 98
#endif

// ######## //
// PLATFORM //
// ######## //
#if defined(_WIN32) || defined(_WIN64)
#define OPX_PLATFORM_WINDOWS 1
#define OPX_PLATFORM_WIN32 1
#if defined(_WIN64)
#define OPX_PLATFORM_WIN64 1
#endif
#elif defined(__APPLE__)
#include <TargetConditionals.h>
#define OPX_PLATFORM_APPLE 1
#define OPX_PLATFORM_POSIX 1
#if defined(TARGET_OS_IPHONE) && TARGET_OS_IPHONE
#define OPX_PLATFORM_IOS 1
#elif defined(TARGET_OS_TV) && TARGET_OS_TV
#define OPX_PLATFORM_TVOS 1
#elif defined(TARGET_OS_MAC) && TARGET_OS_MAC
#define OPX_PLATFORM_MACOS 1
#else
#error "Unknown Apple platform: __APPLE__ defined but TARGET_OS_IPHONE/TV/MAC not detected"
#endif
#elif defined(__ANDROID__)
#define OPX_PLATFORM_ANDROID 1
#define OPX_PLATFORM_LINUX 1
#define OPX_PLATFORM_POSIX 1
#elif defined(__linux__)
#define OPX_PLATFORM_LINUX 1
#define OPX_PLATFORM_POSIX 1
#elif defined(__Fuchsia__)
#define OPX_PLATFORM_FUCHSIA 1
#elif defined(__FreeBSD__)
#define OPX_PLATFORM_FREEBSD 1
#define OPX_PLATFORM_BSD 1
#define OPX_PLATFORM_POSIX 1
#elif defined(__DragonFly__)
#define OPX_PLATFORM_DRAGONFLYBSD 1
#define OPX_PLATFORM_BSD 1
#define OPX_PLATFORM_POSIX 1
#elif defined(__NetBSD__)
#define OPX_PLATFORM_NETBSD 1
#define OPX_PLATFORM_BSD 1
#define OPX_PLATFORM_POSIX 1
#elif defined(__OpenBSD__)
#define OPX_PLATFORM_OPENBSD 1
#define OPX_PLATFORM_BSD 1
#define OPX_PLATFORM_POSIX 1
#else
#error "Unknown platform: OPX_PLATFORM_* could not be detected"
#endif

// ######## //
// COMPILER //
// ######## //
#if defined(_MSC_VER)
#define OPX_COMPILER_MSVC 1
#define OPX_COMPILER_MSVC_VERSION _MSC_VER
#elif defined(__clang__)
#define OPX_COMPILER_CLANG 1
#define OPX_COMPILER_CLANG_VERSION (__clang_major__ * 10000 + __clang_minor__ * 100 + __clang_patchlevel__)
#elif defined(__GNUC__)
#define OPX_COMPILER_GCC 1
#if defined(__GNUC_PATCHLEVEL__)
#define OPX_COMPILER_GCC_VERSION (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)
#else
#define OPX_COMPILER_GCC_VERSION (__GNUC__ * 10000 + __GNUC_MINOR__ * 100)
#endif
#else
#error "Unknown compiler: OPX_COMPILER_* could not be detected"
#endif

// ############ //
// ARCHITECTURE //
// ############ //
#if defined(__x86_64__) || defined(_M_X64) || defined(__amd64__) || defined(_M_AMD64)
#define OPX_ARCH_AMD64 1
#elif defined(__i386__) || defined(_M_IX86)
#define OPX_ARCH_INTEL_X86 1
#elif defined(__aarch64__) || defined(_M_ARM64)
#define OPX_ARCH_ARM64 1
#elif defined(__arm__) || defined(_M_ARM)
#define OPX_ARCH_ARM 1
#elif defined(__ppc64__) || defined(__powerpc64__) || defined(_ARCH_PPC64)
#define OPX_ARCH_POWERPC64 1
#elif defined(__ppc__) || defined(__powerpc__) || defined(_ARCH_PPC)
#define OPX_ARCH_POWERPC32 1
#elif defined(__mips__) || defined(__mips) || defined(__MIPS__)
#define OPX_ARCH_MIPS 1
#elif defined(__sparc__) || defined(__sparc)
#define OPX_ARCH_SPARC 1
#elif defined(__ia64__) || defined(_IA64) || defined(__IA64__) || defined(_M_IA64)
#define OPX_ARCH_IA64 1
#elif defined(__alpha__) || defined(_M_ALPHA)
#define OPX_ARCH_ALPHA 1
#elif defined(__riscv)
#define OPX_ARCH_RISCV 1
#if defined(__riscv_xlen)
#if __riscv_xlen == 64
#define OPX_ARCH_RISCV64 1
#elif __riscv_xlen == 32
#define OPX_ARCH_RISCV32 1
#endif
#endif
#else
#error "Unknown architecture: OPX_ARCH_* could not be detected"
#endif

#if defined(__has_cpp_attribute)
#define OPX_HAS_CPP_ATTRIBUTE(...) __has_cpp_attribute(__VA_ARGS__)
#else
#define OPX_HAS_CPP_ATTRIBUTE(...) 0
#endif

#if OPX_HAS_CPP_ATTRIBUTE(nodiscard)
#define OPX_NODISCARD [[nodiscard]]
#else
#define OPX_NODISCARD
#endif

#if OPX_HAS_CPP_ATTRIBUTE(deprecated)
#define OPX_DEPRECATED [[deprecated]]
#define OPX_DEPRECATED_MSG(message) [[deprecated(message)]]
#else
#define OPX_DEPRECATED
#define OPX_DEPRECATED_MSG(message)
#endif

#if OPX_HAS_CPP_ATTRIBUTE(maybe_unused)
#define OPX_MAYBE_UNUSED [[maybe_unused]]
#else
#define OPX_MAYBE_UNUSED
#endif

#if OPX_HAS_CPP_ATTRIBUTE(fallthrough)
#define OPX_FALLTHROUGH [[fallthrough]]
#else
#define OPX_FALLTHROUGH
#endif

#if OPX_COMPILER_MSVC
#define OPX_INLINE __forceinline
#define OPX_NOINLINE __declspec(noinline)
#elif OPX_COMPILER_GCC || OPX_COMPILER_CLANG
#define OPX_INLINE __attribute__((always_inline)) inline
#define OPX_NOINLINE __attribute__((noinline))
#else
#define OPX_INLINE inline
#define OPX_NOINLINE
#endif

#if OPX_COMPILER_MSVC
#define OPX_IMPORT __declspec(dllimport)
#define OPX_EXPORT __declspec(dllexport)
#elif OPX_COMPILER_GCC || OPX_COMPILER_CLANG
#define OPX_IMPORT
#define OPX_EXPORT __attribute__((visibility("default")))
#else
#define OPX_IMPORT
#define OPX_EXPORT
#endif

#if defined(OPX_BUILD_SHARED_LIBS) && OPX_BUILD_SHARED_LIBS
#if defined(OPX_EXPORT_SHARED_LIBS)
#define OPX_API OPX_EXPORT
#else
#define OPX_API OPX_IMPORT
#endif
#else
#define OPX_API
#endif

#if defined(__cpp_constexpr) && __cpp_constexpr

#define OPX_CONSTEXPR constexpr

#if __cpp_constexpr >= 202110L
#define OPX_CONSTEXPR_VERSION 23
#elif __cpp_constexpr >= 201907L
#define OPX_CONSTEXPR_VERSION 20
#elif __cpp_constexpr >= 201603L
#define OPX_CONSTEXPR_VERSION 17
#elif __cpp_constexpr >= 201304L
#define OPX_CONSTEXPR_VERSION 14
#elif __cpp_constexpr >= 200704L
#define OPX_CONSTEXPR_VERSION 11
#else
#define OPX_CONSTEXPR_VERSION 0
#endif

#else
#define OPX_CONSTEXPR
#define OPX_CONSTEXPR_VERSION 0
#endif

#if defined(__cpp_consteval) && __cpp_consteval
#define OPX_CONSTEVAL consteval
#else
#define OPX_CONSTEVAL
#endif

#if OPX_COMPILER_MSVC

#define OPX_WARNING_PUSH() __pragma(warning(push))
#define OPX_WARNING_POP() __pragma(warning(pop))
#define OPX_MSVC_DISABLE_WARNING(warningCode) __pragma(warning(disable : warningCode))
#define OPX_CLANG_DISABLE_WARNING(warning)
#define OPX_GCC_DISABLE_WARNING(warning)

#elif OPX_COMPILER_CLANG

#define OPX_DO_PRAGMA(x) _Pragma(#x)
#define OPX_WARNING_PUSH() OPX_DO_PRAGMA(clang diagnostic push)
#define OPX_WARNING_POP() OPX_DO_PRAGMA(clang diagnostic pop)
#define OPX_CLANG_DISABLE_WARNING(warning) OPX_DO_PRAGMA(clang diagnostic ignored warning)
#define OPX_MSVC_DISABLE_WARNING(warningCode)
#define OPX_GCC_DISABLE_WARNING(warning)

#elif OPX_COMPILER_GCC

#define OPX_DO_PRAGMA(x) _Pragma(#x)
#define OPX_WARNING_POP() OPX_DO_PRAGMA(GCC diagnostic pop)
#define OPX_WARNING_PUSH() OPX_DO_PRAGMA(GCC diagnostic push)
#define OPX_GCC_DISABLE_WARNING(warning) OPX_DO_PRAGMA(GCC diagnostic ignored warning)
#define OPX_MSVC_DISABLE_WARNING(warningCode)
#define OPX_CLANG_DISABLE_WARNING(warning)

#else
#define OPX_WARNING_PUSH()
#define OPX_WARNING_POP()
#define OPX_MSVC_DISABLE_WARNING(warningCode)
#define OPX_CLANG_DISABLE_WARNING(warning)
#define OPX_GCC_DISABLE_WARNING(warning)
#endif

#if defined(OPX_DEBUG)
#if OPX_COMPILER_MSVC
#define OPX_DEBUG_BREAK() __debugbreak()
#elif defined(__i386__) || defined(_M_IX86) || defined(__x86_64__) || defined(_M_X64)
#define OPX_DEBUG_BREAK()              \
    {                                  \
        __asm__ volatile("int $0x03"); \
    }
#elif defined(__thumb__)
#define OPX_DEBUG_BREAK()                 \
    {                                     \
        __asm__ volatile(".inst 0xde01"); \
    }
#elif defined(__arm__) || defined(_M_ARM)
#define OPX_DEBUG_BREAK()           \
    {                               \
        __asm__ volatile("bkpt 0"); \
    }
#elif defined(__aarch64__) || defined(_M_ARM64)
#define OPX_DEBUG_BREAK()           \
    {                               \
        __asm__ volatile("brk #0"); \
    }
#elif defined(__mips__)
#define OPX_DEBUG_BREAK()          \
    {                              \
        __asm__ volatile("break"); \
    }
#elif defined(__powerpc__) || defined(__powerpc64__)
#define OPX_DEBUG_BREAK()             \
    {                                 \
        __asm__ volatile("twge 2,2"); \
    }
#elif defined(__riscv)
#define OPX_DEBUG_BREAK()           \
    {                               \
        __asm__ volatile("ebreak"); \
    }
#else
#include <signal.h>
#define OPX_DEBUG_BREAK() raise(SIGTRAP)
#endif
#else
#define OPX_DEBUG_BREAK()
#endif

#define OPX_LINE __LINE__
#define OPX_FILE __FILE__

#if OPX_COMPILER_MSVC
#define OPX_FUNC __FUNCSIG__
#elif OPX_COMPILER_GCC || OPX_COMPILER_CLANG
#define OPX_FUNC __PRETTY_FUNCTION__
#else
#define OPX_FUNC __func__
#endif

#define OPX_STRINGIFY_IMPL(x) #x
#define OPX_STRINGIFY(x) OPX_STRINGIFY_IMPL(x)

#define OPX_CONCATENATE_IMPL(x, y) x##y
#define OPX_CONCATENATE(x, y) OPX_CONCATENATE_IMPL(x, y)

#define OPX_BIT(x) (1ULL << (x))
#define OPX_UNUSED(x) (void)(x)
#define OPX_ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))
#define OPX_INVALID_INDEX (-1)
#define OPX_LOCATION OPX_FILE ":" OPX_STRINGIFY(OPX_LINE)

#define OPX_MIN(x, y) ((x > y) ? y : x)
#define OPX_MAX(x, y) ((x > y) ? x : y)
#define OPX_CLAMP(x, min, max) ((x < min) ? min : (max < x ? max : x))

#define OPX_NAMESPACE_BEGIN namespace OPX_NAMESPACE_NAME {
#define OPX_NAMESPACE_END }