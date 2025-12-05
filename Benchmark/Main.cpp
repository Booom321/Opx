#include "Benchmark/Benchmark.hpp"

void StringCreation(uint64_t iterations) {
    for (uint64_t i = 0; i < iterations; i++) {
        std::string s{};
        BENCHMARK_DO_NOT_OPTIMIZE(s);
    }
}

void StringCopy(uint64_t iterations) {
    std::string s = "hello";
    for (uint64_t i = 0; i < iterations; i++) {
        std::string copy(s);
        BENCHMARK_DO_NOT_OPTIMIZE(copy);
    }
}

BENCHMARK_REGISTER(Test, StringCreation, StringCreation)
    ->SetWarmUpRuns(100)
    ->AddIterations(100000, 10, 4);

BENCHMARK_REGISTER(Test, StringCopy, StringCopy)->SetWarmUpRuns(100)->AddIterations(100000, 10, 4);

BENCHMARK_MAIN();