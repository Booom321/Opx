set(OPX_SOURCE_DIR ${ROOT_PATH}/Source)
file(GLOB_RECURSE OPX_ALL_HEADER_FILES ${OPX_SOURCE_DIR}/Opx/*.hpp)
file(GLOB_RECURSE OPX_ALL_SOURCE_FILES ${OPX_SOURCE_DIR}/Opx/*.cpp)

add_library(Opx ${OPX_ALL_HEADER_FILES} ${OPX_ALL_SOURCE_FILES})

target_link_libraries(Opx PUBLIC External)
target_include_directories(Opx PRIVATE ${ROOT_PATH}/External)

set_target_default_macros(Opx PUBLIC)
set_target_default_compile_options(Opx PUBLIC)

target_register_clang_format(Opx clang_format_opx)