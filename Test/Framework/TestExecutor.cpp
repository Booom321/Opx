#include "Framework/Framework.hpp"

#include "stb_sprintf.h"

// https://github.com/agauniyal/rang/blob/22345aa4c468db3bd4a0e64a47722aad3518cc81/include/rang.hpp#L139
// https://github.com/gabime/spdlog/blob/8806ca6509f037cf7612ea292338e3b222209dc1/include/spdlog/details/os-inl.h#L401
Bool SupportsColor() noexcept {
#if defined(WIN32) || defined(_WIN32) || defined(_WIN64)
    return true;
#elif defined(__unix__) || defined(__unix) || defined(__linux__) || defined(__APPLE__) || defined(__MACH__)
    const Char* EnvTERM = std::getenv("TERM");
    const Char* EnvCOLORTERM = std::getenv("COLORTERM");
    if (EnvTERM || EnvCOLORTERM) {
        return true;
    }

    static constexpr std::array<const char*, 16> kTerms = {{"ansi", "color", "console", "cygwin", "gnome", "konsole", "kterm", "linux", "msys",
                                                            "putty", "rxvt", "screen", "vt100", "xterm", "alacritty", "vt102"}};

    return std::any_of(kTerms.begin(), kTerms.end(), [&](const Char* term) { return std::strstr(EnvTERM, term) != nullptr; });
#endif
}

static const Bool kSupportsColor = SupportsColor();

const Char* MakeStyle(const Char* value) { return kSupportsColor ? value : ""; }

static const Char* kReset = MakeStyle("\033[m");
static const Char* kBold = MakeStyle("\033[1m");
static const Char* kDark = MakeStyle("\033[2m");

static const Char* kRed = MakeStyle("\033[31m");
static const Char* kGreen = MakeStyle("\033[32m");

static const Char* kRedBold = MakeStyle("\033[31m\033[1m");
static const Char* kGreenBold = MakeStyle("\033[32m\033[1m");
static const Char* kWhiteBold = MakeStyle("\033[37m\033[1m");

class TestTimer {
public:
    using Type = Double;
    using MsDuration = std::chrono::duration<Type, std::milli>;
    using ClockType = std::chrono::high_resolution_clock;

    TestTimer() : start(ClockType::now()) {}

    void Reset() { start = ClockType::now(); }

    Type GetElapsedMilliseconds() const { return MsDuration{ClockType::now() - start}.count(); }

private:
    std::chrono::time_point<ClockType> start;
};

TestExecutor::TestExecutor() : mTotalTestTime(0.0), mNumPassedTestCases(0), mNumFailedTestCases(0) {}

TestExecutor* TestExecutor::Get() {
    static TestExecutor executorInstance{};
    return &executorInstance;
}

TestSuite& TestExecutor::GetTestSuiteByName(const Char* suiteName) {
    for (SizeT i = 0, numSuites = mTestSuites.size(); i < numSuites; ++i) {
        if (!strcmp(mTestSuites[i].GetName(), suiteName)) {
            return mTestSuites[i];
        }
    }
    mTestSuites.emplace_back(suiteName);
    return mTestSuites.back();
}

void TestExecutor::Execute() {
    if (!mTestSuites.empty()) {
        printf("----------------------------------------------------------------------------------------------\n");
    }

    while (!mTestSuites.empty()) {
        RunTestSuite(mTestSuites.front());
        mTestSuites.pop_front();
    }

    const Int32 totalTestCases = mNumPassedTestCases + mNumFailedTestCases;
    printf("----------------------------------------------------------------------------------------------\n");
    printf("Result: %s%d%% tests passed%s\n", kGreenBold, totalTestCases == 0 ? 100 : static_cast<Int32>(mNumPassedTestCases / totalTestCases * 100),
           kReset);
    printf("        %s%d passed%s\n", kGreenBold, mNumPassedTestCases, kReset);
    printf("        %s%d failed%s\n", kRedBold, mNumFailedTestCases, kReset);
    printf("Time  : %.3lf ms\n", mTotalTestTime);
    printf("----------------------------------------------------------------------------------------------\n");
}

void TestExecutor::RunTestSuite(TestSuite& suite) {
    std::deque<TestCase>& testCases = suite.GetTestCases();
    const Int32 numTestCases = static_cast<Int32>(testCases.size());
    const Char* suiteName = suite.GetName();
    printf((numTestCases == 1) ? "%sTest suite%s %s---%s %s (%d test case)\n" : "%sTest suite%s %s---%s %s (%d test cases)\n", kWhiteBold, kReset,
           kDark, kReset, suiteName, numTestCases);

    TestTimer timer{};
    TestCaseResult result{};
    while (!testCases.empty()) {
        const TestCase& testCase = testCases.front();
        result.SetPassed(true);
        result.SetTestCaseName(testCase.GetName());

        timer.Reset();
        testCase.Run(result);
        result.SetElapsedTime(timer.GetElapsedMilliseconds());

        mTotalTestTime += result.GetElapsedTime();
        ProcessTestCaseResult(result);

        testCases.pop_front();
        result.Reset();
    }
    printf("\n");
}

void TestExecutor::ProcessTestCaseResult(const TestCaseResult& result) {
    if (result.IsPassed()) {
        printf(">> %sTest case |%s %s (%.3lf ms) ... %sPassed%s\n", kDark, kReset, result.GetTestCaseName(), result.GetElapsedTime(), kGreenBold,
               kReset);
        ++mNumPassedTestCases;
        return;
    }

    printf(">> %sTest case |%s %s (%.3lf ms) ... %sFailed%s\n", kDark, kReset, result.GetTestCaseName(), result.GetElapsedTime(), kRedBold, kReset);
    ++mNumFailedTestCases;

    const std::vector<FailedTestInfo>& infos = result.GetInfos();
    for (SizeT i = 0, numInfos = infos.size(); i < numInfos; ++i) {
        const FailedTestInfo& info = infos[i];

        switch (info.GetFailureType()) {
            case ETestFailureType::Test:
                printf("   -> %sTest failed: %s:%d%s\n", kRed, info.GetFile(), info.GetLine(), kReset);
                break;
            case ETestFailureType::Assertion:
                printf("   -> %sAssertion failed: %s:%d%s\n", kRedBold, info.GetFile(), info.GetLine(), kReset);
                break;
            default:
                break;
        }
    }
}