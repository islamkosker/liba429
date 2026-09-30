#define UNITY_INCLUDE_CONFIG_H
#include "unity.h"
#include "test_utils.h"
#include "test_protocol_mock.h"

void setUp(void) {}
void tearDown(void) {}

void protocol_invariant_bnr(uint8_t label_idx, a429_ssm_bnr_t ssm, a429_sdi_t sdi)
{
    a429_label_dictionary_t dictionary = GET_TABLE_ELEM(a429_table, label_idx);

    a429_encode_params_t params = {
        .payload.value = random_value_bnr(dictionary.encoding.width,
                                          dictionary.resolution),

        .sdi = sdi,
        .ssm.ssm_bnr = ssm};

    a429_decode_result_t res = {0};

    a429_error_t e;
    a429_word_t word = 0;
    e = a429_encode_word(dictionary.label, &word, &params, &a429_table);
    TEST_ASSERT_EQUAL(A429_ERR_NO, e);

    e = a429_decode_word(word, &res, &a429_table);
    TEST_ASSERT_EQUAL(A429_ERR_NO, e);

    TEST_ASSERT_DOUBLE_WITHIN(dictionary.resolution, params.payload.value, res.payload.value);
    TEST_ASSERT_EQUAL(sdi, res.sdi);
    TEST_ASSERT_EQUAL(ssm, res.ssm.ssm_bnr);
}
void test_protocol_invariant_bnr_data(void)
{
    protocol_invariant_bnr(TEST_DATA + 1, A429_SSM_DISC_NORMAL, A429_SDI_ALL);
}

void test_protocol_invariant_bnr_data_ssm(void)
{
    protocol_invariant_bnr(TEST_DATA_SSM + 1, A429_SSM_DISC_NOT_USE, A429_SDI_ALL);
}

void test_protocol_invariant_bnr_data_sdi(void)
{
    protocol_invariant_bnr(TEST_DATA_SDI + 1, A429_SSM_DISC_NORMAL, A429_SDI_NOT_USE);
}

void test_protocol_invariant_bnr_data_sdi_ssm(void)
{
    protocol_invariant_bnr(TEST_DATA_SDI_SSM + 1, A429_SSM_DISC_NOT_USE, A429_SDI_NOT_USE);
}

int main(void)
{
    // test_seed = (unsigned)time(NULL);
    srand((unsigned)time(NULL));
    UNITY_BEGIN();

    RUN_TEST(test_protocol_invariant_bnr_data);
    RUN_TEST(test_protocol_invariant_bnr_data_ssm);
    RUN_TEST(test_protocol_invariant_bnr_data_sdi);
    RUN_TEST(test_protocol_invariant_bnr_data_sdi_ssm);

    return UNITY_END();
}
