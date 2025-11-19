function(set_target_default_macros target scope)
    target_compile_definitions(
        ${target} 
        ${scope} 
            "${MACRO_PREFIX}NAMESPACE_NAME=${NAMESPACE_NAME}"
            "${MACRO_PREFIX}ROOT_PATH=\"${ROOT_PATH}\""
            "${MACRO_PREFIX}VERSION_MAJOR=${VERSION_MAJOR}"
            "${MACRO_PREFIX}VERSION_MINOR=${VERSION_MINOR}"
            "${MACRO_PREFIX}VERSION_PATCH=${VERSION_PATCH}"
            $<$<CONFIG:Debug>:${MACRO_PREFIX}DEBUG>
            $<$<CONFIG:Release>:${MACRO_PREFIX}RELEASE>
            $<$<BOOL:${BUILD_SHARED_LIBS}>:"${MACRO_PREFIX}BUILD_SHARED_LIBS">

            $<$<EQUAL:${CMAKE_SIZEOF_VOID_P},4>:${MACRO_PREFIX}ARCH_32>
            $<$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>:${MACRO_PREFIX}ARCH_64>

            $<$<PLATFORM_ID:Windows>:WIN32_LEAN_AND_MEAN NOMINMAX>
            $<$<CXX_COMPILER_ID:MSVC>:_SCL_SECURE_NO_WARNINGS _CRT_SECURE_NO_WARNINGS>
    )
endfunction()

function(set_target_default_compile_options target scope)
    target_compile_options(
        ${target}
        ${scope}
            # GCC
            $<$<CXX_COMPILER_ID:GNU>:-Wall -Wextra -Wpedantic -Wshadow -Wnon-virtual-dtor -Wold-style-cast -Wsign-conversion -Wconversion>
            # Clang
            $<$<CXX_COMPILER_ID:Clang>:-Wall -Wextra -Wpedantic -Wshadow -Wnon-virtual-dtor -Wold-style-cast -Wsign-conversion -Wconversion>
            # MSVC
            $<$<CXX_COMPILER_ID:MSVC>:/W4 /permissive- /MP>
    )

    if(MSVC)
        target_compile_options(
            ${target}
            ${scope}
                $<$<AND:$<CONFIG:Debug>,$<BOOL:${BUILD_SHARED_LIBS}>>:/MDd>
                $<$<AND:$<CONFIG:Release>,$<BOOL:${BUILD_SHARED_LIBS}>>:/MD>
                $<$<AND:$<CONFIG:Debug>,$<NOT:$<BOOL:${BUILD_SHARED_LIBS}>>>:/MTd>
                $<$<AND:$<CONFIG:Release>,$<NOT:$<BOOL:${BUILD_SHARED_LIBS}>>>:/MT>
        )
    endif()
endfunction()

function(add_clang_format target_name TARGET_DIR)
    file(GLOB_RECURSE ALL_SOURCE_FILES
        "${TARGET_DIR}/*.hpp"
        "${TARGET_DIR}/*.cpp"
        # "${TARGET_DIR}/*.h"
        # "${TARGET_DIR}/*.c"
    )
    if(ALL_SOURCE_FILES)
        add_custom_target(
            ${target_name}
            COMMAND clang-format -i ${ALL_SOURCE_FILES}
            COMMENT "Running clang-format on ${TARGET_DIR}"
            VERBATIM
        )
    endif()
endfunction()

macro(target_register_clang_format dest_target target_name)
    add_clang_format(${target_name} ${CMAKE_CURRENT_SOURCE_DIR})
    add_dependencies(${dest_target} ${target_name})
endmacro()