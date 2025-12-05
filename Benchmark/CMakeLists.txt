set(BENCHMARK_SOURCE_DIR ${ROOT_PATH}/Benchmark)
file(GLOB_RECURSE BM_ALL_HEADER_FILES ${BENCHMARK_SOURCE_DIR}/*.hpp)
file(GLOB_RECURSE BM_ALL_SOURCE_FILES ${BENCHMARK_SOURCE_DIR}/*.cpp)

add_executable(Benchmark ${BM_ALL_HEADER_FILES} ${BM_ALL_SOURCE_FILES})

target_include_directories(Benchmark PRIVATE ${ROOT_PATH}/Source)
target_include_directories(Benchmark PRIVATE ${ROOT_PATH}/External/stb)
target_include_directories(Benchmark PRIVATE ${BENCHMARK_SOURCE_DIR})

target_link_libraries(Benchmark PRIVATE Opx)

if(BUILD_SHARED_LIBS)
    target_compile_definitions(Benchmark PRIVATE BENCHMARK_BUILD_SHARED_LIBS)
endif()

target_register_clang_format(Benchmark clang_format_benchmark)
