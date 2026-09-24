if(NOT LIBA429_ENABLE_TESTS)
    return()
endif()

set(UNITY_DIR "${CMAKE_CURRENT_SOURCE_DIR}/unity")
set(UTILS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/utils")

# -------------------------
# Unity 
# -------------------------
add_library(unity STATIC
    "${UNITY_DIR}/unity.c"
)

target_include_directories(unity PUBLIC
    "${UNITY_DIR}"
    

)

# -------------------------
# Test helper
# -------------------------
function(add_liba429_test name src)
    add_executable(${name} ${src})
    message("-- Executable added: ${name}")

    target_link_libraries(${name} PRIVATE
        a429
        unity
    )

    target_include_directories(${name} PRIVATE
        "${PROJECT_SOURCE_DIR}/include"
        "${UNITY_DIR}"
        "${UTILS_DIR}"


    )

    add_test(NAME ${name} COMMAND ${name})
endfunction()

# -------------------------
# Tests
# -------------------------
set(TEST_CASES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/cases")

add_liba429_test(test_parity "${TEST_CASES_DIR}/unit/test_parity.c")
add_liba429_test(test_bnr "${TEST_CASES_DIR}/unit/test_bnr.c")
add_liba429_test(test_bcd "${TEST_CASES_DIR}/unit/test_bcd.c")
