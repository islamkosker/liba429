#define UNITY_INCLUDE_CONFIG_H
#include "unity.h"
#include "test_utils.h"
#include "test_protocol_mock.h"

void setUp(void) {}
void tearDown(void) {}

static void bnr_wire_roundtrip(uint8_t label_idx, a429_ssm_bnr_t ssm, a429_sdi_t sdi)
{
    a429_label_dictionary_t dictionary = GET_TABLE_ELEM(a429_table, label_idx);

    a429_encode_params_t tx_params = {
        .payload.value = random_value_bnr(dictionary.encoding.width,
                                          dictionary.resolution),

        .sdi = sdi,
        .ssm.ssm_bnr = ssm};
    a429_decode_result_t rx_result = {0};
    a429_word_t tx_word = 0;
    a429_word_t rx_word = 0;
    a429_wire_data_t wire_data = 0;

    a429_error_t e = a429_encode_word(dictionary.label, &tx_word, &tx_params, &a429_table);
    TEST_ASSERT_EQUAL(A429_ERR_NO, e);

    wire_data = a429_pack_word(tx_word);
    rx_word = a429_unpack_word(wire_data);
    uint8_t out_label = 0;
    e = a429_decode_word(rx_word, &rx_result, &a429_table, &out_label);
    TEST_ASSERT_EQUAL(A429_ERR_NO, e);

    TEST_ASSERT_DOUBLE_WITHIN(dictionary.resolution, tx_params.payload.value, rx_result.payload.value);
    TEST_ASSERT_EQUAL(sdi, rx_result.sdi);
    TEST_ASSERT_EQUAL(ssm, rx_result.ssm.ssm_bnr);
}

void test_bnr_wire_roundtrip_data(void)
{
    bnr_wire_roundtrip(TEST_DATA + 1, A429_SSM_BNR_NORMAL, A429_SDI_ALL);
}

void test_bnr_wire_roundtrip_data_ssm(void)
{
    bnr_wire_roundtrip(TEST_DATA_SSM + 1, A429_SSM_BNR_NOT_USE, A429_SDI_ALL);
}

void test_bnr_wire_roundtrip_data_sdi(void)
{
    bnr_wire_roundtrip(TEST_DATA_SDI + 1, A429_SSM_BNR_NORMAL, A429_SDI_NOT_USE);
}

void test_bnr_wire_roundtrip_data_ssm_sdi(void)
{
    bnr_wire_roundtrip(TEST_DATA_SDI_SSM + 1, A429_SSM_BNR_NOT_USE, A429_SDI_NOT_USE);
}

int main(void)
{

    UNITY_BEGIN();
    RUN_TEST(test_bnr_wire_roundtrip_data);
    RUN_TEST(test_bnr_wire_roundtrip_data_ssm);
    RUN_TEST(test_bnr_wire_roundtrip_data_sdi);
    RUN_TEST(test_bnr_wire_roundtrip_data_ssm_sdi);

    return UNITY_END();
}