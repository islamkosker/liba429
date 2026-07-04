cmake_minimum_required(VERSION 3.20)

project(liba429
    VERSION 0.1.0
    LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

add_library(a429 STATIC
    src/parity/parity.c)

target_sources(a429 PRIVATE
    src/core/a429.c
)

target_include_directories(a429
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/include/a429
)


target_compile_options(a429 PRIVATE
    -Wall
    -Wextra
    -Wpedantic
)

add_executable(a429_test
    examples/main.c
)

target_link_libraries(a429_test
    PRIVATE a429
)