cmake_minimum_required(VERSION 3.20)

project(liba429 C)

set(CMAKE_C_STANDARD 11)

add_library(a429 STATIC

    src/a429_bnr.c
    src/a429_bcd.c
    src/a429_codec.c
    src/a429_disc.c
    src/a429_parity.c
    src/a429_word.c
    src/a429_protocol.c

)

target_link_libraries(a429 PRIVATE m)

target_include_directories(a429
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
    PRIVATE
        ${PROJECT_SOURCE_DIR}/src/a429
)

message("Build type: ${CMAKE_BUILD_TYPE}")
target_compile_options(a429 PRIVATE
    -Wall -Wextra -Wpedantic
)

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    add_custom_command(TARGET a429 POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "$<TARGET_FILE:a429>"
            "${CMAKE_SOURCE_DIR}/example/liba429.a"
        COMMENT "Copying release liba429.a to example"
    )
endif()

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
    set(ACTIVE_DIR ${CMAKE_SOURCE_DIR}/.build/debug)
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    set(ACTIVE_DIR ${CMAKE_SOURCE_DIR}/.build/release)
    
else()
    set(ACTIVE_DIR ${CMAKE_SOURCE_DIR}/.build/test)
endif()

# Create .build directory if it doesn't exist
file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/.build)

add_custom_target(set_active ALL
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_SOURCE_DIR}/.build/active
    COMMAND ${CMAKE_COMMAND} -E create_symlink ${ACTIVE_DIR} ${CMAKE_SOURCE_DIR}/.build/active
)