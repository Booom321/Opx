add_executable(Benchmark Main.cpp)

target_include_directories(Benchmark PRIVATE ${ROOT_PATH}/Source)
target_include_directories(Benchmark PRIVATE ${ROOT_PATH}/Benchmark)

target_link_libraries(Benchmark PRIVATE Opx)

target_register_clang_format(Benchmark clang_format_benchmark)
