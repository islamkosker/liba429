if(NOT LIBA429_ENABLE_TESTS)
    return()
endif()

set(UNITY_DIR "${CMAKE_CURRENT_SOURCE_DIR}/unity")
set(MOCK_DIR "${CMAKE_CURRENT_SOURCE_DIR}/mock")
set(TEST_CASES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/cases")


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
        "${PROJECT_SOURCE_DIR}/src/a429"
        "${UNITY_DIR}"
        "${MOCK_DIR}"

    )

    add_test(NAME ${name} COMMAND ${name})
endfunction()

# -------------------------
# Tests
# -------------------------

add_liba429_test(test_codec_parity "${TEST_CASES_DIR}/unit/test_parity.c")
add_liba429_test(test_codec_bnr "${TEST_CASES_DIR}/unit/test_bnr.c")
add_liba429_test(test_codec_bcd "${TEST_CASES_DIR}/unit/test_bcd.c")
add_liba429_test(test_codec_disc "${TEST_CASES_DIR}/unit/test_disc.c")
add_liba429_test(test_integration_protocol_invariant_bcd "${TEST_CASES_DIR}/integration/test_protocol_invariant_bcd.c")
add_liba429_test(test_integration_protocol_invariant_bnr "${TEST_CASES_DIR}/integration/test_protocol_invariant_bnr.c")
add_liba429_test(test_integration_protocol_invariant_disc "${TEST_CASES_DIR}/integration/test_protocol_invariant_disc.c")
add_liba429_test(test_integration_protocol_wire_bcd "${TEST_CASES_DIR}/integration/test_wire_bcd.c")
add_liba429_test(test_integration_protocol_wire_bnr "${TEST_CASES_DIR}/integration/test_wire_bnr.c")
add_liba429_test(test_integration_protocol_wire_disc "${TEST_CASES_DIR}/integration/test_wire_disc.c")
