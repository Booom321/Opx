cmake_minimum_required(VERSION 3.15)

include(CMake/Settings.cmake)
include(CMake/Utilities.cmake)

project(
    "Project"
    VERSION ${PROJECT_VERSION}
    LANGUAGES CXX C
)

set(OUTPUT_DIR "${CMAKE_BINARY_DIR}/Bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${OUTPUT_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${OUTPUT_DIR})

set(CMAKE_CXX_STANDARD ${CXX_STANDARD})
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_C_STANDARD ${C_STANDARD})
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

if(MSVC)
    add_compile_options(/MP)
endif()

add_subdirectory(External)
add_subdirectory(Source)