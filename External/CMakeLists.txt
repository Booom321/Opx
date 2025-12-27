add_library(
    External
    Utils/Utils.hpp
    Utils/Utils.cpp
)

target_export_api(External UTILS_API)
target_enable_clang_format(External)