#include "Benchmark.hpp"

#include <Opx/Preprocessor.hpp>

#include <stb_sprintf.h>

#include <algorithm>
#include <cstring>
#include <cassert>
#include <cmath>
#include <memory>

#if defined(_MSC_VER) && defined(OPX_DEBUG)
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif

namespace BM {
    static constexpr UInt64 kMaxIterations = 10'000'000'000;
    static constexpr UInt64 kMaxWarmUpRuns = 10'000'000;

    static constexpr Int64 kTimeUnitInSeconds[] = {
        1,             // Seconds
        1'000,         // Millisecond
        1'000'000,     // Microsecond
        1'000'000'000  // Nanosecond
    };

    static constexpr const char* kTimeUnitToString[] = {
        "s",
        "ms",
        "us",
        "ns",
    };

    double GetElapsedByTimeUnit(Microseconds duration, ETimeUnits units) {
        using namespace std::chrono;

        switch (units) {
            case ETimeUnits::Second:
                return duration_cast<Seconds>(duration).count();
            case ETimeUnits::Millisecond:
                return duration_cast<Milliseconds>(duration).count();
            case ETimeUnits::Microsecond:
                return duration_cast<Microseconds>(duration).count();
            case ETimeUnits::Nanosecond:
                return duration_cast<Nanoseconds>(duration).count();
        }
        return 0.0;
    }

    // https://github.com/agauniyal/rang/blob/22345aa4c468db3bd4a0e64a47722aad3518cc81/include/rang.hpp#L139
    // https://github.com/gabime/spdlog/blob/8806ca6509f037cf7612ea292338e3b222209dc1/include/spdlog/details/os-inl.h#L401
    bool SupportsColor() noexcept {
#if defined(WIN32) || defined(_WIN32) || defined(_WIN64)
        return true;
#elif defined(__unix__) || defined(__unix) || defined(__linux__) || defined(__APPLE__) || \
    defined(__MACH__)
        const char* EnvTERM = std::getenv("TERM");
        const char* EnvCOLORTERM = std::getenv("COLORTERM");
        if (EnvTERM || EnvCOLORTERM) {
            return true;
        }

        static constexpr std::array<const char*, 16> kTerms = {
            {"ansi", "color", "console", "cygwin", "gnome", "konsole", "kterm", "linux", "msys",
             "putty", "rxvt", "screen", "vt100", "xterm", "alacritty", "vt102"}};

        return std::any_of(kTerms.begin(), kTerms.end(),
                           [&](const Char* term) { return std::strstr(EnvTERM, term) != nullptr; });
#endif
    }

    static const bool kSupportsColor = SupportsColor();
    const char* MakeStyle(const char* value) {
        return kSupportsColor ? value : "";
    }

    template <typename T = double>
    constexpr T ConvertTimeUnit(ETimeUnits from, ETimeUnits to, T time) {
        return static_cast<T>(time) * (static_cast<T>(kTimeUnitInSeconds[static_cast<int>(to)]) /
                                       static_cast<T>(kTimeUnitInSeconds[static_cast<int>(from)]));
    }

    double ComputeTimeStddev(ETimeUnits unit, const std::vector<BenchmarkResult>& results) {
        if (results.size() < 2) {
            return 0.0;
        }

        double totalPerOp = 0.0;
        for (const auto& result : results) {
            totalPerOp += result.mElapsed / static_cast<double>(result.mIterations);
        }

        const double mean = ConvertTimeUnit(unit, ETimeUnits::Nanosecond, totalPerOp) /
                            static_cast<double>(results.size());
        double squaredDifferencesSum = 0.0;
        for (const auto& result : results) {
            double perOp = result.mElapsed / static_cast<double>(result.mIterations);
            double diff = ConvertTimeUnit(unit, ETimeUnits::Nanosecond, perOp) - mean;
            squaredDifferencesSum += diff * diff;
        }

        return std::sqrt(squaredDifferencesSum / static_cast<double>(results.size() - 1));
    }

    double ComputeFactorBaseline(const BenchmarkGroup::AnalysisResult& baseline,
                                 const BenchmarkGroup::AnalysisResult& benchmark) {
        if (benchmark.mBenchmark->IsBaseline()) {
            return 1.0f;
        }
        const auto benchmarkTimePerIter =
            ConvertTimeUnit(benchmark.mBenchmark->GetTimeUnit(), baseline.mBenchmark->GetTimeUnit(),
                            benchmark.mTimePerIteration);
        return (baseline.mTimePerIteration != 0) ? benchmarkTimePerIter / baseline.mTimePerIteration
                                                 : 1.0;
    }

    Benchmark* Benchmark::AddIterations(UInt64 iterations) {
        assert(iterations <= kMaxIterations);
        mIterations.push_back(iterations);
        return this;
    }

    Benchmark* Benchmark::AddIterations(UInt64 initial, UInt64 multiplier, UInt64 count) {
        auto iterations = initial;
        for (UInt64 i = 0; i < count; ++i) {
            assert(iterations <= kMaxIterations);
            mIterations.push_back(iterations);
            iterations *= multiplier;
        }
        return this;
    }

    Benchmark* Benchmark::SetWarmUpRuns(UInt64 runs) {
        assert(mWarmUpRuns <= kMaxWarmUpRuns);
        mWarmUpRuns = runs;
        return this;
    }

    Benchmark* Benchmark::SetTimeUnits(ETimeUnits units) {
        mTimeUnit = units;
        return this;
    }

    void Benchmark::Run() {
        for (UInt64 i = 0; i < mWarmUpRuns; ++i) {
            mFunction(1);
        }

        mResults.reserve(mIterations.size());

        for (UInt64 iterations : mIterations) {
            auto start = Clock::now();
            mFunction(iterations);
            auto end = Clock::now();
            const auto elapsed = GetElapsedByTimeUnit(end - start, mTimeUnit);
            mResults.emplace_back(BenchmarkResult{iterations, elapsed});
        }
    }

    BenchmarkGroup::~BenchmarkGroup() {
        if (mBaseline) {
            delete mBaseline;
            mBaseline = nullptr;
        }
        mGroupName.clear();
        mBenchmarks.clear();
    }

    bool BenchmarkGroup::AddBenchmark(const char* name, typename Benchmark::Function fn) {
        if (std::find_if(mBenchmarks.begin(), mBenchmarks.end(), [&](const Benchmark& benchmark) {
                return benchmark.Name() == name;
            }) != mBenchmarks.end()) {
            return false;
        }
        mBenchmarks.emplace_back(name, fn, false);
        return true;
    }

    bool BenchmarkGroup::SetBaseline(const char* name, typename Benchmark::Function fn) {
        if (mBaseline) {
            return false;
        }

        mBaseline = new Benchmark(name, fn, true);
        return mBaseline != nullptr;
    }

    void BenchmarkGroup::Run() {
        const auto numResults = mBenchmarks.size() + (mBaseline != nullptr ? 1 : 0);
        mResults.reserve(numResults);
        if (mBaseline) {
            RunBenchmarkImpl(*mBaseline);
        }
        for (auto& benchmark : mBenchmarks) {
            RunBenchmarkImpl(benchmark);
        }

        DisplayAnalysisResults();
    }

    void BenchmarkGroup::RunBenchmarkImpl(Benchmark& benchmark) {
        benchmark.Run();
        AnalyzeBenchmarkResults(benchmark);
    }

    void BenchmarkGroup::AnalyzeBenchmarkResults(const Benchmark& benchmark) {
        AnalysisResult result{};
        const std::vector<BenchmarkResult>& benchmarkResults = benchmark.Results();
        const auto benchmarkResultsSize = benchmarkResults.size();
        result.mBenchmark = &benchmark;
        result.mStddev = ComputeTimeStddev(benchmark.GetTimeUnit(), benchmarkResults);
        result.mIterationsPerTime = 0.0;
        result.mTimePerIteration = 0.0;
        for (const auto& bmResult : benchmarkResults) {
            const double numIterations = static_cast<double>(bmResult.mIterations);
            const double elapsed = static_cast<double>(bmResult.mElapsed);
            result.mIterationsPerTime += numIterations / elapsed;
            result.mTimePerIteration += elapsed / numIterations;
        }

        if (benchmarkResultsSize != 0) {
            result.mIterationsPerTime /= static_cast<double>(benchmarkResultsSize);
            result.mTimePerIteration /= static_cast<double>(benchmarkResultsSize);
            result.mTimePerIteration = ConvertTimeUnit(
                benchmark.GetTimeUnit(), ETimeUnits::Nanosecond, result.mTimePerIteration);
        }

        if (mBaseline) {
            result.mSpeedUpVsBaseline =
                result.mBenchmark == mBaseline ? 1.0 : ComputeFactorBaseline(mResults[0], result);
        } else {
            result.mSpeedUpVsBaseline = -1.0;
        }
        mResults.emplace_back(result);
    }

    static constexpr int kBenchmarkColSize = 30;
    static constexpr int kIterationsColSize = 12;
    static constexpr int kTimeColSize = 22;
    static constexpr int kOpsPerTimeColSize = 22;
    static constexpr int kTimePerOpsColSize = 22;
    static constexpr int kStddevColSize = 22;
    static constexpr int kSpeedUpColSize = 22;

    void BenchmarkGroup::GenerateTableHeader() {
        char buffer[512];
        if (mBaseline) {
            stbsp_snprintf(buffer, sizeof(buffer), "%-*s %*s %*s %*s %*s %*s %*s",
                           kBenchmarkColSize, "Benchmark", kIterationsColSize, "Iterations",
                           kTimeColSize, "Time", kOpsPerTimeColSize, "Ops/Time", kTimePerOpsColSize,
                           "Time/Ops", kStddevColSize, "Stddev", kSpeedUpColSize, "Speed up");
        } else {
            stbsp_snprintf(buffer, sizeof(buffer), "%-*s %*s %*s %*s %*s %*s", kBenchmarkColSize,
                           "Benchmark", kIterationsColSize, "Iterations", kTimeColSize, "Time",
                           kOpsPerTimeColSize, "Ops/Time", kTimePerOpsColSize, "Time/Ops",
                           kStddevColSize, "Stddev");
        }

        tableHeader = buffer;
    }

    void BenchmarkGroup::PrintLineSeparator(char c) {
        char lineSeparator[512];
        memset(lineSeparator, '\0', sizeof(lineSeparator));
        memset(lineSeparator, c, tableHeader.size());
        printf("%s\n", lineSeparator);
    }

    void BenchmarkGroup::PrintTableHeader() {
        printf("%s\n", tableHeader.c_str());
    }

    void BenchmarkGroup::PrintRow(const AnalysisResult& result) {
        const auto timeUnit = result.mBenchmark->GetTimeUnit();
        const auto* timeUnitStr = kTimeUnitToString[static_cast<int>(timeUnit)];
        const auto& benchmarkResults = result.mBenchmark->Results();
        auto totalIterations = 0ull;
        auto totalTime = 0.0;

        for (const auto& benchmarkResult : benchmarkResults) {
            totalIterations += benchmarkResult.mIterations;
            totalTime += benchmarkResult.mElapsed;
        }

        char buffer[512];
        memset(buffer, '\0', sizeof(buffer));

        int benchmarkColSize = kBenchmarkColSize;
        int iterationsColSize = kIterationsColSize;
        int timeColSize = kTimeColSize - 3;
        int opsPerTimeColSize = kOpsPerTimeColSize - 3;
        int timePerOpsColSize = kTimePerOpsColSize - 3;
        int stddevColSize = kStddevColSize - 3;
        int speedUpColSize = kSpeedUpColSize;
        if (timeUnit == ETimeUnits::Second) {
            timeColSize += 1;
            opsPerTimeColSize += 1;
            timePerOpsColSize += 1;
            stddevColSize += 1;
        }

        mBaseline != nullptr
            ? stbsp_snprintf(buffer, sizeof(buffer),
                             "%-*s %*lld %*.4f %s %*.4f/%s %*.4f ns %*.4f ns %*.4f",
                             benchmarkColSize, result.mBenchmark->Name().c_str(), iterationsColSize,
                             totalIterations, timeColSize, totalTime, timeUnitStr,
                             opsPerTimeColSize, result.mIterationsPerTime, timeUnitStr,
                             timePerOpsColSize, result.mTimePerIteration, stddevColSize,
                             result.mStddev, speedUpColSize, result.mSpeedUpVsBaseline)
            : stbsp_snprintf(buffer, sizeof(buffer),
                             "%-*s %*lld %*.4f %s %*.4f/%s %*.4f ns %*.4f ns", benchmarkColSize,
                             result.mBenchmark->Name().c_str(), iterationsColSize, totalIterations,
                             timeColSize, totalTime, timeUnitStr, opsPerTimeColSize,
                             result.mIterationsPerTime, timeUnitStr, timePerOpsColSize,
                             result.mTimePerIteration, stddevColSize, result.mStddev);
        printf("%s\n", buffer);
    }

    void BenchmarkGroup::DisplayAnalysisResults() {
        GenerateTableHeader();

        PrintLineSeparator('=');
        printf("Benchmark group: %s\n", mGroupName.c_str());
        PrintLineSeparator('-');
        PrintTableHeader();
        PrintLineSeparator('-');
        for (const auto& result : mResults) {
            PrintRow(result);
        }
        PrintLineSeparator('=');
        return;
    }

    BenchmarkRunner* BenchmarkRunner::instance = nullptr;

    BenchmarkRunner* BenchmarkRunner::Get() {
        return instance ? instance : instance = new BenchmarkRunner();
    }

    void BenchmarkRunner::Release() {
        if (instance) {
            delete instance;
        }
    }

    void BenchmarkRunner::RunBenchmarks() {
        for (BenchmarkGroup& group : mGroups) {
            group.Run();
        }
    }

    BenchmarkGroup& BenchmarkRunner::GetBenchmarkGroup(const char* groupName) {
        auto it =
            std::find_if(mGroups.begin(), mGroups.end(), [&](const BenchmarkGroup& benchmarkGroup) {
                return benchmarkGroup.Name() == groupName;
            });

        if (it == mGroups.end()) {
            mGroups.emplace_back(groupName);
            return mGroups.back();
        }
        return *it;
    }

    Benchmark* RegisterBenchmark(const char* groupName,
                                 const char* name,
                                 typename Benchmark::Function fn,
                                 bool isBaseline) {
        BenchmarkGroup& group = BenchmarkRunner::Get()->GetBenchmarkGroup(groupName);

        if (isBaseline) {
            assert(group.SetBaseline(name, fn) && "Baseline is already set");
            return group.GetBaseline();
        }
        assert(group.AddBenchmark(name, fn) && "Benchmark is already registered");
        return &(group.Benchmarks().back());
    }

#if defined(_MSC_VER) && defined(OPX_DEBUG)
    void EnableMemoryLeakDetection() {
        _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
        _CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDOUT);
        _CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_FILE);
        _CrtSetReportFile(_CRT_ERROR, _CRTDBG_FILE_STDOUT);
        _CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_FILE);
        _CrtSetReportFile(_CRT_ASSERT, _CRTDBG_FILE_STDOUT);

        _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    }

    void DumpMemoryLeaks() {
        _CrtDumpMemoryLeaks();
    }
#else
    void EnableMemoryLeakDetection() {}
    void DumpMemoryLeaks() {}
#endif

    Int32 BenchmarkMain(Int32 argc, char** argv) {
        BENCHMARK_UNUSED(argc);
        BENCHMARK_UNUSED(argv);

        BenchmarkRunner::Get()->RunBenchmarks();
        BenchmarkRunner::Release();

        return 0;
    }
}  // namespace BM
