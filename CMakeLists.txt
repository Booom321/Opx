cmake_minimum_required(VERSION 3.10)

include(CMake/Settings.cmake)
include(CMake/Utils.cmake)

project(
    ${PROJECT_NAME}
    VERSION ${PROJECT_VERSION}
    LANGUAGES CXX C
)

set(CMAKE_CXX_STANDARD ${PROJECT_CXX_STANDARD})
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_C_STANDARD ${PROJECT_C_STANDARD})
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

if(MSVC)
    add_compile_options(/MP)
endif()

message(STATUS "===============================================================")
message(STATUS "PROJECT_NAME: \"${PROJECT_NAME}\"")
message(STATUS "PROJECT_VERSION: ${PROJECT_VERSION}")
message(STATUS "MACRO_PREFIX: \"${PROJECT_NAME}\"")
message(STATUS "NAMESPACE_NAME: \"${NAMESPACE_NAME}\"")
message(STATUS "CXX_STANDARD: ${CXX_STANDARD}")
message(STATUS "C_STANDARD: ${C_STANDARD}")
message(STATUS "ROOT_PATH: \"${ROOT_PATH}\"")
message(STATUS "===============================================================")

add_subdirectory(Source)
add_subdirectory(External)