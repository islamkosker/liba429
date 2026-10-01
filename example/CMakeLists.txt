cmake_minimum_required(VERSION 3.20)

project(liba429_example C)

add_executable(liba429_example
    main.c
)

target_include_directories(liba429_example
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(liba429_example
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/liba429.a
        m
)