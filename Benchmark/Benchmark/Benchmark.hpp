#pragma once

#include <chrono>
#include <vector>
#include <string>
#include <atomic>

#if defined(_MSC_VER)
#define BENCHMARK_IMPORT __declspec(dllimport)
#define BENCHMARK_EXPORT __declspec(dllexport)
#elif defined(__GNUC__) || defined(__clang__)
#define BENCHMARK_IMPORT
#define BENCHMARK_EXPORT __attribute__((visibility("default")))
#else
#define BENCHMARK_IMPORT
#define BENCHMARK_EXPORT
#endif

#if defined(BENCHMARK_BUILD_SHARED_LIBS) && BENCHMARK_BUILD_SHARED_LIBS
#if defined(BENCHMARK_EXPORT_SHARED_LIBS)
#define BENCHMARK_API BENCHMARK_EXPORT
#else
#define BENCHMARK_API BENCHMARK_IMPORT
#endif
#else
#define BENCHMARK_API
#endif

#if defined(__GNUC__) || defined(__clang__)
#define BENCHMARK_INLINE __attribute__((always_inline)) inline
#elif defined(_MSC_VER) && !defined(__clang__)
#define BENCHMARK_INLINE __forceinline
#else
#define BENCHMARK_INLINE inline
#endif

#define BENCHMARK_UNUSED(x) (void)(x)

namespace BM {
    using Int8 = int8_t;
    using UInt8 = uint8_t;
    using Int16 = int16_t;
    using UInt16 = uint16_t;
    using Int32 = int32_t;
    using UInt32 = uint32_t;
    using Int64 = int64_t;
    using UInt64 = uint64_t;

    using Clock = std::chrono::steady_clock;
    using TimePoint = std::chrono::time_point<Clock>;

    using Seconds = std::chrono::duration<double>;
    using Milliseconds = std::chrono::duration<double, std::milli>;
    using Microseconds = std::chrono::duration<double, std::micro>;
    using Nanoseconds = std::chrono::duration<double, std::nano>;

#if defined(_MSC_VER)
    template <typename T>
    BENCHMARK_INLINE void DoNotOptimize(const T& value) {
        volatile const T* volatile ptr = &value;
        (void)*ptr;
    }

    template <typename T>
    BENCHMARK_INLINE void DoNotOptimize(T& value) {
        volatile T* volatile ptr = &value;
        (void)*ptr;
    }
#else
    template <typename T>
    BENCHMARK_INLINE void DoNotOptimize(const T& value) {
        asm volatile("" : : "r,m"(value) : "memory");
    }

    template <typename T>
    BENCHMARK_INLINE void DoNotOptimize(T& value) {
        asm volatile("" : "+r,m"(value) : : "memory");
    }
#endif

    // https://github.com/google/benchmark/blob/main/include/benchmark/benchmark.h#L519
    BENCHMARK_INLINE void ClobberMemory() {
        std::atomic_signal_fence(std::memory_order_acq_rel);
    }

    enum class ETimeUnits : Int8 {
        Second = 0,
        Millisecond = 1,
        Microsecond = 2,
        Nanosecond = 3,
    };

    class BenchmarkResult {
    public:
        UInt64 mIterations;
        double mElapsed;
    };

    class BENCHMARK_API Benchmark {
    public:
        using Function = void (*)(UInt64 iterations);

        Benchmark(const char* name, Function fn, bool isBaseline)
            : mName(name), mFunction(fn), mIsBaseline(isBaseline) {}

        ~Benchmark() = default;

        [[nodiscard]] BENCHMARK_INLINE ETimeUnits GetTimeUnit() const { return mTimeUnit; }
        [[nodiscard]] BENCHMARK_INLINE bool IsBaseline() const { return mIsBaseline; }
        [[nodiscard]] BENCHMARK_INLINE const std::string& Name() const { return mName; }
        [[nodiscard]] BENCHMARK_INLINE const std::vector<BenchmarkResult>& Results() const {
            return mResults;
        }

        template <typename... Iterations>
        Benchmark* AddMultipleIterations(Iterations&&... iterations) {
            UInt64 values[] = {static_cast<UInt64>(iterations)...};
            for (auto x : values) {
                mIterations.push_back(x);
            }
            return this;
        }

        Benchmark* AddIterations(UInt64 iterations);
        Benchmark* AddIterations(UInt64 initial, UInt64 multiplier, UInt64 count);
        Benchmark* SetWarmUpRuns(UInt64 runs);
        Benchmark* SetTimeUnits(ETimeUnits units);

        void Run();

    private:
        std::string mName;
        std::vector<UInt64> mIterations;
        std::vector<BenchmarkResult> mResults;
        Function mFunction;
        UInt64 mWarmUpRuns = 0;
        ETimeUnits mTimeUnit = ETimeUnits::Millisecond;
        bool mIsBaseline;
    };

    class BENCHMARK_API BenchmarkGroup {
    public:
        struct AnalysisResult {
            const Benchmark* mBenchmark = nullptr;
            double mStddev = 0.0;
            double mIterationsPerTime = 0.0;
            double mTimePerIteration = 0.0;
            double mSpeedUpVsBaseline = 0.0;
        };

        BenchmarkGroup(const char* name) : mBaseline(nullptr), mGroupName(name) {};
        ~BenchmarkGroup();

        bool AddBenchmark(const char* name, typename Benchmark::Function fn);
        bool SetBaseline(const char* name, typename Benchmark::Function fn);

        BENCHMARK_INLINE Benchmark* GetBaseline() { return mBaseline; }
        BENCHMARK_INLINE std::vector<Benchmark>& Benchmarks() { return mBenchmarks; }
        BENCHMARK_INLINE const std::vector<Benchmark>& Benchmarks() const { return mBenchmarks; }
        BENCHMARK_INLINE const std::string& Name() const { return mGroupName; }

        void Run();

    private:
        void RunBenchmarkImpl(Benchmark& benchmark);
        void AnalyzeBenchmarkResults(const Benchmark& benchmark);
        void DisplayAnalysisResults();

        void GenerateTableHeader();

        void PrintLineSeparator(char c);
        void PrintTableHeader();
        void PrintRow(const AnalysisResult& result);

        Benchmark* mBaseline;

        std::string mGroupName;
        std::string tableHeader;
        std::string tableRowFmt;

        std::vector<Benchmark> mBenchmarks;
        std::vector<AnalysisResult> mResults{};
    };

    class BENCHMARK_API BenchmarkRunner {
        BenchmarkRunner(const BenchmarkRunner&) = delete;
        BenchmarkRunner& operator=(const BenchmarkRunner&) = delete;
        BenchmarkRunner(BenchmarkRunner&&) = delete;
        BenchmarkRunner& operator=(BenchmarkRunner&&) = delete;

    public:
        BenchmarkRunner() = default;

        static BenchmarkRunner* Get();
        static void Release();

        void RunBenchmarks();

        BenchmarkGroup& GetBenchmarkGroup(const char* groupName);

    private:
        static BenchmarkRunner* instance;

        std::vector<BenchmarkGroup> mGroups;
    };

    BENCHMARK_API Benchmark* RegisterBenchmark(const char* groupName,
                                               const char* name,
                                               typename Benchmark::Function fn,
                                               bool isBaseline = false);

    BENCHMARK_API void EnableMemoryLeakDetection();
    BENCHMARK_API void DumpMemoryLeaks();
    BENCHMARK_API Int32 BenchmarkMain(Int32 argc, char** argv);
}  // namespace BM

#define BENCHMARK_REGISTER(groupName, name, func) \
    static ::BM::Benchmark* BM##groupName##name = ::BM::RegisterBenchmark(#groupName, #name, func)

#define BENCHMARK_BASELINE(groupName, name, func)         \
    static ::BM::Benchmark* BMBaseline##groupName##name = \
        ::BM::RegisterBenchmark(#groupName, #name, func, true)

#define BENCHMARK_MAIN()                            \
    int main(int argc, char** argv) {               \
        ::BM::EnableMemoryLeakDetection();          \
        int code = ::BM::BenchmarkMain(argc, argv); \
        return code;                                \
    }

#define BENCHMARK_DO_NOT_OPTIMIZE(...) ::BM::DoNotOptimize(__VA_ARGS__)
#define BENCHMARK_CLOBBER_MEMORY(...) ::BM::ClobberMemory()
