#define UNITY_INCLUDE_CONFIG_H
#include "unity.h"
#include "test_utils.h"
#include "test_protocol_mock.h"

void setUp(void) {}
void tearDown(void) {}

void protocol_invariant_disc(uint8_t label_idx, a429_ssm_disc_t ssm, a429_sdi_t sdi)
{
    a429_label_dictionary_t dictionary = GET_TABLE_ELEM(a429_table, label_idx);

    a429_encode_params_t params = {
        .payload.discrete = random_value_disc(dictionary.encoding.width,
                                              false, false),

        .sdi = sdi,
        .ssm.ssm_disc = ssm};

    a429_decode_result_t res = {0};

    a429_error_t e;
    a429_word_t word = 0;
    e = a429_encode_word(dictionary.label, &word, &params, &a429_table);
    TEST_ASSERT_EQUAL(A429_ERR_NO, e);
    uint8_t out_label = 0;
    e = a429_decode_word(word, &res, &a429_table, &out_label);
    TEST_ASSERT_EQUAL(A429_ERR_NO, e);

    TEST_ASSERT_EQUAL_UINT32(params.payload.discrete, res.payload.discrete);
    TEST_ASSERT_EQUAL(sdi, res.sdi);
    TEST_ASSERT_EQUAL(ssm, res.ssm.ssm_disc);
}
// 32:test_protocol_invariant_disc_data_ssm:FAIL: Expected 4 Was 0
void test_protocol_invariant_disc_data(void)
{
    protocol_invariant_disc(TEST_DATA + 2, A429_SSM_DISC_NORMAL, A429_SDI_ALL);
}

void test_protocol_invariant_disc_data_ssm(void)
{
    protocol_invariant_disc(TEST_DATA_SSM + 2, A429_SSM_DISC_NOT_USE, A429_SDI_ALL);
}

void test_protocol_invariant_disc_data_sdi(void)
{
    protocol_invariant_disc(TEST_DATA_SDI + 2, A429_SSM_DISC_NORMAL, A429_SDI_NOT_USE);
}

void test_protocol_invariant_disc_data_sdi_ssm(void)
{
    protocol_invariant_disc(TEST_DATA_SDI_SSM + 2, A429_SSM_DISC_NOT_USE, A429_SDI_NOT_USE);
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
