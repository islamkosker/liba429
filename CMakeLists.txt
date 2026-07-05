cmake_minimum_required(VERSION 3.20)

project(liba429 C)

set(CMAKE_C_STANDARD 11)

add_library(a429 STATIC
    src/parity/parity.c
    src/core/a429.c
)

target_include_directories(a429
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/include/a429
)

message("Build type: ${CMAKE_BUILD_TYPE}")
target_compile_options(a429 PRIVATE
    -Wall -Wextra -Wpedantic
)

# -------------------------
# TEST FLAG (FIXED NAME)
# -------------------------
option(LIBA429_ENABLE_TESTS "Enable unit tests" ON)

if(LIBA429_ENABLE_TESTS)
    include(CTest)
    enable_testing()
    add_subdirectory(tests)
endif()


if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(ACTIVE_DIR ${CMAKE_SOURCE_DIR}/build/debug)
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    set(ACTIVE_DIR ${CMAKE_SOURCE_DIR}/build/release)
else()
    set(ACTIVE_DIR ${CMAKE_SOURCE_DIR}/build/test)
endif()

add_custom_target(set_active ALL
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_SOURCE_DIR}/build/active
    COMMAND ${CMAKE_COMMAND} -E create_symlink ${ACTIVE_DIR} ${CMAKE_SOURCE_DIR}/build/active
)