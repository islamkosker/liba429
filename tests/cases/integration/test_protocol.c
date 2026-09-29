#define UNITY_INCLUDE_CONFIG_H
#include "liba429.h"
#include "a429_word.h"
#include "unity.h"
#include "test_utils.h"
#include <stdlib.h>
#include <time.h>
#define CAT_HELPER(a, b) a##b
#define CAT(a, b) CAT_HELPER(a, b)

#define STRINGIFY_HELPER(x) #x
#define STRINGIFY(x) STRINGIFY_HELPER(x)

#define NAME(x) STRINGIFY(CAT(TEST_NAME_, x))
#define UNIT(x) STRINGIFY(CAT(TEST_UNIT_, x))

#define DEFAULT_LABEL 0
#define DEFAULT_BIT_TIME 100

#define GET_LABEL(x) ((x) + DEFAULT_LABEL)
#define GET_EQID(x) ((x) + DEFAULT_LABEL)
#define TEST_RAND_MAX 0x1387F
#define STANDART_DATA_WIDTH 19

#define TEST_SCALE 1.0
#define TEST_OFFSET 0.0
#define TEST_RESOLUTION 0.01

static const a429_dictionary_table_t a429_table = {
    A429_BCD(GET_LABEL(0), GET_EQID(0), NAME(0), UNIT(0), A429_DATA_BEGIN,
             STANDART_DATA_WIDTH, DEFAULT_BIT_TIME, TEST_RESOLUTION, TEST_SCALE, TEST_OFFSET),

    A429_BNR(GET_LABEL(1), GET_EQID(1), NAME(1), UNIT(1), A429_DATA_BEGIN,
             STANDART_DATA_WIDTH, DEFAULT_BIT_TIME, TEST_RESOLUTION, TEST_SCALE, TEST_OFFSET),

    A429_DISC(GET_LABEL(2), GET_EQID(2), NAME(2), UNIT(2), A429_DATA_BEGIN,
              STANDART_DATA_WIDTH, DEFAULT_BIT_TIME, TEST_RESOLUTION, TEST_SCALE, TEST_OFFSET),

};

#define GET_TABLE_ELEM(table, idx) (*(table)[idx])

void setUp(void) {}
void tearDown(void) {}

const unsigned long long large_rand()
{
    unsigned long long r = 0;
    for (int i = 0; i < 5; i++)
    {
        r = (r << 15) | (rand() & 0x7FFF);
    }
    return r;
}

static double const random_value_bcd(uint8_t width, double resolution)
{
    const uint8_t full_digits = width / 4;
    const uint8_t remaining_bits = width % 4;

    const uint8_t digit_count = full_digits + ((remaining_bits != 0U) ? 1U : 0U);

    uint32_t max_bcd_int = 0U;
    uint32_t multiplier = 1U;

    for (uint8_t i = 0; i < digit_count; i++)
    {
        uint8_t digit_width = 4U;

        if ((i == (digit_count - 1U)) && remaining_bits != 0U)
        {
            digit_width = remaining_bits;
        }
        uint32_t max_digit = (UINT32_C(1) << digit_width) - UINT32_C(1);
        if (max_digit > 9U)
        {
            max_digit = 9U;
        }
        max_bcd_int += max_digit * multiplier;

        multiplier *= 10U;
    }
    uint32_t raw_bcd_int = (uint32_t)(large_rand() % (max_bcd_int + 1U));

    double value = (double)raw_bcd_int * resolution;
}

void test_protocol_invariant(void)
{
    a429_encode_params_t params = {
        .payload.value = random_value_bcd(GET_TABLE_ELEM(a429_table, 0).encoding.width,
                                          GET_TABLE_ELEM(a429_table, 0).resolution),

        .sdi = A429_SDI_ALL,
        .ssm.ssm_bcd = A429_SSM_BCD_PLUS_NORTH_EAST};

    a429_decode_result_t res = {0};

    a429_error_t e;
    a429_word_t word = 0;
    e = a429_encode_word(GET_TABLE_ELEM(a429_table, 0).label, &word, &params, &a429_table);
    TEST_ASSERT_EQUAL(A429_ERR_NO, e);

    e = a429_decode_word(word, &res, &a429_table);
    TEST_ASSERT_EQUAL(A429_ERR_NO, e);

    TEST_ASSERT_DOUBLE_WITHIN(GET_TABLE_ELEM(a429_table, 0).resolution, params.payload.value, res.payload.value);
}

int main(void)
{
    // test_seed = (unsigned)time(NULL);
    srand((unsigned)time(NULL));
    UNITY_BEGIN();
    RUN_TEST(test_protocol_invariant);

    return UNITY_END();
}
