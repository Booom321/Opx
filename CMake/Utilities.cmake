function(target_export_api target_name api_name)
    if(BUILD_SHARED_LIBS)
        if(MSVC OR WIN32 OR CYGWIN)
            target_compile_definitions(
                ${target_name} 
                PRIVATE "${api_name}=__declspec(dllexport)"
            )
        elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang|AppleClang")
            target_compile_definitions(${target_name} 
                PUBLIC "${api_name}=__attribute__((visibility(\"default\")))"
            )
            set_target_properties(
                ${target_name} PROPERTIES
                CXX_VISIBILITY_PRESET hidden
                VISIBILITY_INLINES_HIDDEN ON
            )
        else()
            message(FATAL_ERROR "Unsupported compiler!")
        endif()
    else()
        target_compile_definitions(${target_name} PUBLIC ${api_name}=)
    endif()
    set_target_properties(
        ${target_name} PROPERTIES 
        TARGET_API_NAME "${api_name}"
    )
endfunction()

function(target_import_api target_name target_to_import)
    get_target_property(api_name ${target_to_import} TARGET_API_NAME)

    if(NOT api_name)
        message(FATAL_ERROR "Target ${target_to_import} was not configured with target_export_api!")
    endif()

    if(BUILD_SHARED_LIBS)
        if(MSVC OR WIN32 OR CYGWIN)
            target_compile_definitions(
                ${target_name} 
                PRIVATE "${api_name}=__declspec(dllimport)"
            )
        elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang|AppleClang")
            target_compile_definitions(${target_name} 
                PUBLIC "${api_name}=__attribute__((visibility(\"default\")))"
            )
        else()
            message(FATAL_ERROR "Unsupported compiler!")
        endif()
    else()
        target_compile_definitions(${target_name} PUBLIC ${api_name}=)
    endif()
endfunction()

function(target_default_compile_options target_name)
    target_compile_options(
        ${target_name}
        PRIVATE
            $<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>>:
                -Wall -Wextra -Wpedantic -Wshadow -Wnon-virtual-dtor 
                -Wold-style-cast -Wsign-conversion -Wconversion
            >
            $<$<CXX_COMPILER_ID:MSVC>:/W4 /permissive- /MP>
    )

    if(MSVC)
        if(BUILD_SHARED_LIBS)
            set_property(TARGET ${target_name} PROPERTY 
                MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL"
            )
        else()
            set_property(TARGET ${target_name} PROPERTY 
                MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>"
            )
        endif()
    endif()
endfunction()

function(enable_clang_format target_name target_dir)
    file(GLOB_RECURSE ALL_SOURCE_FILES
        CONFIGURE_DEPENDS
        "${target_dir}/*.hpp"
        "${target_dir}/*.cpp"
    )

    if(ALL_SOURCE_FILES)
        add_custom_target(
            ${target_name}
            COMMAND clang-format -i ${ALL_SOURCE_FILES}
            COMMENT "Formatting source files in ${target_dir}"
            VERBATIM
        )
    else()
        message(STATUS "No source files found for clang-format in ${target_dir}")
    endif()
endfunction()

function(target_enable_clang_format target_name)
    enable_clang_format(format_${target_name} ${CMAKE_CURRENT_SOURCE_DIR})
    add_dependencies(${target_name} format_${target_name})
endfunction()