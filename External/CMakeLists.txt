add_library(External STATIC stb/stb_sprintf.h stb/stb_sprintf.cpp)

set(FETCHCONTENT_BASE_DIR "${ROOT_PATH}/External/benchmark")

include(FetchContent)

FetchContent_Declare(
    GoogleBenchmark
    GIT_REPOSITORY https://github.com/google/benchmark.git
    GIT_TAG main
)

FetchContent_MakeAvailable(GoogleBenchmark)