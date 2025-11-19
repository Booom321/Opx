add_library(External STATIC Utils.hpp Utils.cpp)

set_target_default_macros(External PUBLIC)

target_register_clang_format(External clang_format_external)