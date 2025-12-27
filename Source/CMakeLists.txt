add_executable(Main Main.cpp)

target_link_libraries(Main PRIVATE External)
target_include_directories(Main PRIVATE ${ROOT_PATH}/External)

target_import_api(Main External)
target_enable_clang_format(Main)