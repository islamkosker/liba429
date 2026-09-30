#define UNITY_INCLUDE_CONFIG_H
#include "unity.h"
#include "test_utils.h"
#include "test_protocol_mock.h"

void setUp(void) {}
void tearDown(void) {}

static void disc_wire_roundtrip(uint8_t label_idx, a429_ssm_disc_t ssm, a429_sdi_t sdi)
{
    a429_label_dictionary_t dictionary = GET_TABLE_ELEM(a429_table, label_idx);

    a429_encode_params_t tx_params = {
        .payload.discrete = random_value_disc(dictionary.encoding.width,
                                              false, false),

        .sdi = sdi,
        .ssm.ssm_disc = ssm};
    a429_decode_result_t rx_result = {0};
    a429_word_t tx_word = 0;
    a429_word_t rx_word = 0;
    a429_wire_data_t wire_data = 0;

    a429_error_t e = a429_encode_word(dictionary.label, &tx_word, &tx_params, &a429_table);
    TEST_ASSERT_EQUAL(A429_ERR_NO, e);

    wire_data = a429_pack_word(tx_word);
    rx_word = a429_unpack_word(wire_data);

    e = a429_decode_word(rx_word, &rx_result, &a429_table);
    TEST_ASSERT_EQUAL(A429_ERR_NO, e);

    TEST_ASSERT_EQUAL_UINT32(tx_params.payload.discrete, rx_result.payload.discrete);
    TEST_ASSERT_EQUAL(sdi, rx_result.sdi);
    TEST_ASSERT_EQUAL(ssm, rx_result.ssm.ssm_disc);
}

void test_disc_wire_roundtrip_data(void)
{
    disc_wire_roundtrip(TEST_DATA + 2, A429_SSM_DISC_NORMAL, A429_SDI_ALL);
}

void test_disc_wire_roundtrip_data_ssm(void)
{
    disc_wire_roundtrip(TEST_DATA_SSM + 2, A429_SSM_DISC_NOT_USE, A429_SDI_ALL);
}

void test_disc_wire_roundtrip_data_sdi(void)
{
    disc_wire_roundtrip(TEST_DATA_SDI + 2, A429_SSM_DISC_NORMAL, A429_SDI_NOT_USE);
}

void test_disc_wire_roundtrip_data_ssm_sdi(void)
{
    disc_wire_roundtrip(TEST_DATA_SDI_SSM + 2, A429_SSM_DISC_NOT_USE, A429_SDI_NOT_USE);
}

int main(void)
{

    UNITY_BEGIN();
    RUN_TEST(test_disc_wire_roundtrip_data);
    RUN_TEST(test_disc_wire_roundtrip_data_ssm);
    RUN_TEST(test_disc_wire_roundtrip_data_sdi);
    RUN_TEST(test_disc_wire_roundtrip_data_ssm_sdi);

    return UNITY_END();
}