if(NOT LIBA429_ENABLE_TESTS)
    return()
endif()

set(UNITY_DIR "${CMAKE_CURRENT_SOURCE_DIR}/unity")

# -------------------------
# Unity 
# -------------------------
add_library(unity STATIC
    "${UNITY_DIR}/unity.c"
)

# PUBLIC yaparak bu kütüphaneye bağlanan herkesin 
# unity.h'ı doğrudan bulabilmesini sağlıyoruz.
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

    )

    add_test(NAME ${name} COMMAND ${name})
endfunction()

# -------------------------
# Tests
# -------------------------
set(TEST_CASES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/cases")

add_liba429_test(test_parity "${TEST_CASES_DIR}/parity/test_parity.c")
