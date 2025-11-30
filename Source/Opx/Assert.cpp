#include <OpxPCH.hpp>

OPX_NAMESPACE_BEGIN

void DefaultAssertHandler(const Char* expr,
                          const Char* file,
                          const Char* function,
                          Int32 line,
                          const Char* message) {
    printf("Assertion failed: %s\n    >> Location: %s (%s:%d)\n", expr,
           *function ? function : "<function>", file, line);
    if (message) {
        printf("    >> Message: %s\n", message);
    }
}

static AssertHandler globalHandler = DefaultAssertHandler;

void SetAssertHandler(AssertHandler handler) {
    globalHandler = handler;
}

void ReportAssertionFailure(const Char* expr,
                            const Char* file,
                            const Char* function,
                            Int32 line,
                            const Char* message,
                            ...) {
    if (message) {
        Char buffer[1024];

        va_list args;
        va_start(args, message);
        stbsp_vsnprintf(buffer, sizeof(buffer), message, args);
        va_end(args);

        globalHandler(expr, file, function, line, buffer);
    } else {
        globalHandler(expr, file, function, line, nullptr);
    }
}

OPX_NAMESPACE_END