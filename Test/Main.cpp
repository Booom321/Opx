#include "Framework/Framework.hpp"
#include <Opx/Preprocessor.hpp>

#if defined(OPX_COMPILER_MSVC) && defined(OPX_DEBUG)
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
inline void EnableMemoryLeakDetection() {
    _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
    _CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDOUT);
    _CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_FILE);
    _CrtSetReportFile(_CRT_ERROR, _CRTDBG_FILE_STDOUT);
    _CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_FILE);
    _CrtSetReportFile(_CRT_ASSERT, _CRTDBG_FILE_STDOUT);

    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
}

inline void DumpMemoryLeaks() {
    _CrtDumpMemoryLeaks();
}
#else
inline void EnableMemoryLeakDetection() {}
inline void DumpMemoryLeaks() {}
#endif

void RunAllTests() {
    TestExecutor::Get()->Execute();
}

int main() {
    EnableMemoryLeakDetection();
    RunAllTests();
    return 0;
}
