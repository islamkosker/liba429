#define UNITY_INCLUDE_CONFIG_H
#include "unity.h"
#include "test_utils.h"
#include "test_protocol_mock.h"
#include <assert.h>

void setUp(void) {}
void tearDown(void) {}

void test_protocol_invariant_bcd_data(void)
{

    a429_label_dictionary_t dictionary = GET_TABLE_ELEM(a429_table, TEST_DATA);

    a429_encode_params_t params = {
        .payload.value = random_value_bcd(dictionary.encoding.width,
                                          dictionary.resolution),

        .sdi = A429_SDI_ALL,
        .ssm.ssm_bcd = A429_SSM_BCD_PLUS_NORTH_EAST};

    a429_decode_result_t res = {0};

    a429_error_t e;
    a429_word_t word = 0;
    e = a429_encode_word(dictionary.label, &word, &params, &a429_table);
    TEST_ASSERT_EQUAL(A429_ERR_NO, e);

    e = a429_decode_word(word, &res, &a429_table);
    TEST_ASSERT_EQUAL(A429_ERR_NO, e);

    TEST_ASSERT_DOUBLE_WITHIN(dictionary.resolution, params.payload.value, res.payload.value);
    TEST_ASSERT_EQUAL(params.sdi, res.sdi);
    TEST_ASSERT_EQUAL(A429_SSM_BCD_PLUS_NORTH_EAST, res.ssm.ssm_bcd);
}

void test_protocol_invariant_bcd_data_ssm(void)
{

    a429_label_dictionary_t dictionary = GET_TABLE_ELEM(a429_table, TEST_DATA_SSM);

    a429_encode_params_t params = {
        .payload.value = random_value_bcd(dictionary.encoding.width,
                                          dictionary.resolution),

        .sdi = A429_SDI_ALL,
        .ssm.ssm_bcd = A429_SSM_BCD_NOT_USE};

    a429_decode_result_t res = {0};

    a429_error_t e;
    a429_word_t word = 0;
    e = a429_encode_word(dictionary.label, &word, &params, &a429_table);
    TEST_ASSERT_EQUAL(A429_ERR_NO, e);

    e = a429_decode_word(word, &res, &a429_table);
    TEST_ASSERT_EQUAL(A429_ERR_NO, e);

    TEST_ASSERT_DOUBLE_WITHIN(dictionary.resolution, params.payload.value, res.payload.value);
    TEST_ASSERT_EQUAL(params.sdi, res.sdi);
    TEST_ASSERT_EQUAL(A429_SSM_BCD_NOT_USE, res.ssm.ssm_bcd);
}

void test_protocol_invariant_bcd_data_sdi(void)
{

    a429_label_dictionary_t dictionary = GET_TABLE_ELEM(a429_table, TEST_DATA_SDI);

    a429_encode_params_t params = {
        .payload.value = random_value_bcd(dictionary.encoding.width,
                                          dictionary.resolution),

        .sdi = A429_SDI_NOT_USE,
        .ssm.ssm_bcd = A429_SSM_BCD_PLUS_NORTH_EAST};

    a429_decode_result_t res = {0};

    a429_error_t e;
    a429_word_t word = 0;
    e = a429_encode_word(dictionary.label, &word, &params, &a429_table);
    TEST_ASSERT_EQUAL(A429_ERR_NO, e);

    e = a429_decode_word(word, &res, &a429_table);
    TEST_ASSERT_EQUAL(A429_ERR_NO, e);

    TEST_ASSERT_DOUBLE_WITHIN(dictionary.resolution, params.payload.value, res.payload.value);
    TEST_ASSERT_EQUAL(A429_SDI_NOT_USE, res.sdi);
    TEST_ASSERT_EQUAL(params.ssm.ssm_bcd, res.ssm.ssm_bcd);
}

void test_protocol_invariant_bcd_data_sdi_ssm(void)
{

    a429_label_dictionary_t dictionary = GET_TABLE_ELEM(a429_table, TEST_DATA_SDI_SSM);

    a429_encode_params_t params = {
        .payload.value = random_value_bcd(dictionary.encoding.width,
                                          dictionary.resolution),

        .sdi = A429_SDI_NOT_USE,
        .ssm.ssm_bcd = A429_SSM_BCD_NOT_USE};

    a429_decode_result_t res = {0};

    a429_error_t e;
    a429_word_t word = 0;
    e = a429_encode_word(dictionary.label, &word, &params, &a429_table);
    TEST_ASSERT_EQUAL(A429_ERR_NO, e);

    e = a429_decode_word(word, &res, &a429_table);
    TEST_ASSERT_EQUAL(A429_ERR_NO, e);

    TEST_ASSERT_DOUBLE_WITHIN(dictionary.resolution, res.payload.value, params.payload.value);
    TEST_ASSERT_EQUAL(A429_SDI_NOT_USE, res.sdi);
    TEST_ASSERT_EQUAL(A429_SSM_BCD_NOT_USE, res.ssm.ssm_bcd);
}

int main(void)
{
    // test_seed = (unsigned)time(NULL);
    srand((unsigned)time(NULL));
    UNITY_BEGIN();
    RUN_TEST(test_protocol_invariant_bcd_data);
    RUN_TEST(test_protocol_invariant_bcd_data_ssm);
    RUN_TEST(test_protocol_invariant_bcd_data_sdi);
    RUN_TEST(test_protocol_invariant_bcd_data_sdi_ssm);

    return UNITY_END();
}
