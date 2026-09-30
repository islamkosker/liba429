#define UNITY_INCLUDE_CONFIG_H
#include "unity.h"
#include "test_utils.h"
#include "test_protocol_mock.h"

void setUp(void) {}
void tearDown(void) {}

void test_protocol_invariant_disc_data(void)
{
    a429_label_dictionary_t dictionary = GET_TABLE_ELEM(a429_table, TEST_DATA + 2);

    a429_encode_params_t params = {
        .payload.discrete = random_value_disc(dictionary.encoding.width,
                                              false, false),

        .sdi = A429_SDI_ALL,
        .ssm.ssm_disc = A429_SSM_DISC_NORMAL};

    a429_decode_result_t res = {0};

    a429_error_t e;
    a429_word_t word = 0;
    e = a429_encode_word(dictionary.label, &word, &params, &a429_table);
    TEST_ASSERT_EQUAL(A429_ERR_NO, e);

    e = a429_decode_word(word, &res, &a429_table);
    TEST_ASSERT_EQUAL(A429_ERR_NO, e);

    TEST_ASSERT_DOUBLE_WITHIN(dictionary.resolution, params.payload.discrete, res.payload.discrete);
    TEST_ASSERT_EQUAL(params.sdi, res.sdi);
    TEST_ASSERT_EQUAL(A429_SSM_DISC_NORMAL, res.ssm.ssm_disc);
}

void test_protocol_invariant_disc_data_ssm(void)
{

    a429_label_dictionary_t dictionary = GET_TABLE_ELEM(a429_table, TEST_DATA_SSM + 2);

    a429_encode_params_t params = {
        .payload.discrete = random_value_disc(dictionary.encoding.width,
                                              false, false),

        .sdi = A429_SDI_ALL,
        .ssm.ssm_disc = A429_SSM_DISC_NOT_USE};

    a429_decode_result_t res = {0};

    a429_error_t e;
    a429_word_t word = 0;
    e = a429_encode_word(dictionary.label, &word, &params, &a429_table);
    TEST_ASSERT_EQUAL(A429_ERR_NO, e);

    e = a429_decode_word(word, &res, &a429_table);
    TEST_ASSERT_EQUAL(A429_ERR_NO, e);

    TEST_ASSERT_DOUBLE_WITHIN(dictionary.resolution, params.payload.discrete, res.payload.discrete);
    TEST_ASSERT_EQUAL(params.sdi, res.sdi);
    TEST_ASSERT_EQUAL(A429_SSM_DISC_NOT_USE, res.ssm.ssm_disc);
}

void test_protocol_invariant_disc_data_sdi(void)
{

    a429_label_dictionary_t dictionary = GET_TABLE_ELEM(a429_table, TEST_DATA_SDI + 2);

    a429_encode_params_t params = {
        .payload.discrete = random_value_disc(dictionary.encoding.width,
                                              false, false),

        .sdi = A429_SDI_NOT_USE,
        .ssm.ssm_disc = A429_SSM_DISC_NORMAL};

    a429_decode_result_t res = {0};

    a429_error_t e;
    a429_word_t word = 0;
    e = a429_encode_word(dictionary.label, &word, &params, &a429_table);
    TEST_ASSERT_EQUAL(A429_ERR_NO, e);

    e = a429_decode_word(word, &res, &a429_table);
    TEST_ASSERT_EQUAL(A429_ERR_NO, e);

    TEST_ASSERT_DOUBLE_WITHIN(dictionary.resolution, params.payload.discrete, res.payload.discrete);
    TEST_ASSERT_EQUAL(res.sdi, A429_SDI_NOT_USE);
    TEST_ASSERT_EQUAL(params.ssm.ssm_disc, res.ssm.ssm_disc);
}

void test_protocol_invariant_disc_data_sdi_ssm(void)
{

    a429_label_dictionary_t dictionary = GET_TABLE_ELEM(a429_table, TEST_DATA_SDI_SSM + 2);

    a429_encode_params_t params = {
        .payload.discrete = random_value_disc(dictionary.encoding.width,
                                              false, false),

        .sdi = A429_SDI_NOT_USE,
        .ssm.ssm_disc = A429_SSM_DISC_NOT_USE};

    a429_decode_result_t res = {0};

    a429_error_t e;
    a429_word_t word = 0;
    e = a429_encode_word(dictionary.label, &word, &params, &a429_table);
    TEST_ASSERT_EQUAL(A429_ERR_NO, e);

    e = a429_decode_word(word, &res, &a429_table);
    TEST_ASSERT_EQUAL(A429_ERR_NO, e);

    TEST_ASSERT_DOUBLE_WITHIN(dictionary.resolution, res.payload.discrete, params.payload.discrete);
    TEST_ASSERT_EQUAL(res.sdi, A429_SDI_NOT_USE);
    TEST_ASSERT_EQUAL(A429_SSM_DISC_NOT_USE, res.ssm.ssm_disc);
}

int main(void)
{
    // test_seed = (unsigned)time(NULL);
    srand((unsigned)time(NULL));
    UNITY_BEGIN();

    RUN_TEST(test_protocol_invariant_disc_data);
    RUN_TEST(test_protocol_invariant_disc_data_ssm);
    RUN_TEST(test_protocol_invariant_disc_data_sdi);
    RUN_TEST(test_protocol_invariant_disc_data_sdi_ssm);

    return UNITY_END();
}
