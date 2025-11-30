set(TEST_SOURCE_DIR ${ROOT_PATH}/Test)
file(GLOB_RECURSE TEST_ALL_HEADER_FILES ${TEST_SOURCE_DIR}/*.hpp)
file(GLOB_RECURSE TEST_ALL_SOURCE_FILES ${TEST_SOURCE_DIR}/*.cpp)

add_executable(Test ${TEST_ALL_HEADER_FILES} ${TEST_ALL_SOURCE_FILES})

target_precompile_headers(Test PRIVATE Framework/Framework.hpp)
if (MSVC)
    target_compile_options(Test PRIVATE /Yu"Framework/Framework.hpp")
endif()

target_include_directories(Test PRIVATE ${ROOT_PATH}/External/stb)
target_include_directories(Test PRIVATE ${ROOT_PATH}/Source)
target_include_directories(Test PRIVATE ${TEST_SOURCE_DIR})
target_link_libraries(Test PRIVATE Opx)

if(BUILD_SHARED_LIBS)
    target_compile_definitions(Test PRIVATE TEST_BUILD_SHARED_LIBS)
endif()

target_register_clang_format(Test clang_format_test)