#include "Utils.hpp"

#include <iostream>

#define STRINGIFY_IMPL(x) #x
#define STRINGIFY(x) STRINGIFY_IMPL(x)

namespace NAMESPACE_NAME {
    void PrintInfo() {
        std::cout << "Root path: " << ROOT_PATH << '\n';
        std::cout << "Version: " << VERSION_MAJOR << '.' << VERSION_MINOR << '.' << VERSION_PATCH << '\n';
        std::cout << "Namespace: " << STRINGIFY(NAMESPACE_NAME) << '\n';

#if defined(DEBUG)
        std::cout << "Build type: Debug" << '\n';
#endif

#if defined(RELEASE)
        std::cout << "Build type: Release" << '\n';
#endif

#if defined(BUILD_SHARED_LIBS)
        std::cout << "Enabled BUILD_SHARED_LIBS" << '\n';
#endif

#if defined(ARCH_32)
        std::cout << "x32" << '\n';
#endif

#if defined(ARCH_64)
        std::cout << "x64" << '\n';
#endif
    }
}  // namespace NAMESPACE_NAME