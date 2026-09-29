#include "a429_bcd.h"
#include "decode_mock.h"
#define UNITY_INCLUDE_CONFIG_H

#include "unity.h"
#include "test_utils.h"

test_word_bcd_t test_mock_word;
static unsigned int test_seed;
#define MAX_BCD_19_BIT 79999
void setUp(void) {}
void tearDown(void) {}

void test_bcd_invariant_randomized(void)
{
    for (size_t i = 0; i < RANDOM_TEST_ITERATIONS; i++)
    {
        create_random_a429_bcd_word(&test_mock_word);

        a429_error_t err_code = A429_ERR_NO;

        a429_encode_bcd(&test_mock_word.word, test_mock_word.value, test_mock_word.payload_begin,
                        test_mock_word.payload_width, test_mock_word.resolution, &err_code);

        if (err_code)
        {
            char message[256];
            snprintf(message, sizeof(message),
                     ":>> status = %d seed = %d iter = %d value = %f", err_code, test_seed, i, test_mock_word.value);
            TEST_FAIL_MESSAGE(message);
        }

        const double decoded_value = a429_decode_bcd(test_mock_word.word, test_mock_word.payload_begin,
                                                     test_mock_word.payload_width, test_mock_word.resolution, &err_code);

        if (err_code)
        {
            char message[256];
            snprintf(message, sizeof(message),
                     ":>> status = %d seed = %d iter = %d value = %f", err_code, test_seed, i, test_mock_word.value);
            TEST_FAIL_MESSAGE(message);
        }

        char message[256];
        snprintf(message, sizeof(message),
                 ":>> status = %d seed = %d iter = %d value = %f", err_code, test_seed, i, test_mock_word.value);

        TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(test_mock_word.resolution, test_mock_word.value, decoded_value, message);
    }
}

void test_bcd_boundry_values(void)
{
    const uint8_t payload_begin = UINT32_C(11);
    const uint8_t payload_width = UINT32_C(19);
    const double resolution = 0.001;
    const double max_val = 79.999;

    const double values[] = {0., resolution, (-resolution), (max_val - resolution), (-max_val + resolution)};

    for (size_t i = 0; i < ARRAY_SIZE(values); i++)
    {
        char msg[512] = {0};

        snprintf(msg, sizeof(msg), " Message: iter: %d value  = %f", i, values[i]);
        a429_word_t word = 0;
        a429_error_t encode_error = A429_ERR_NO;
        a429_error_t decode_error = A429_ERR_NO;

        a429_encode_bcd(&word, values[i], payload_begin, payload_width, resolution, &encode_error);

        TEST_ASSERT_EQUAL_MESSAGE(A429_ERR_NO, encode_error, msg);

        const double decoded = a429_decode_bcd(word, payload_begin, payload_width, resolution, &decode_error);
        TEST_ASSERT_EQUAL_MESSAGE(A429_ERR_NO, decode_error, msg);

        TEST_ASSERT_DOUBLE_WITHIN_MESSAGE(resolution, fabs(values[i]), decoded, msg);
    }
}

void test_bcd_out_of_range(void)
{
    a429_word_t word = 0;
    a429_error_t error = A429_ERR_NO;
    const uint8_t payload_begin = UINT32_C(11);
    const uint8_t payload_width = UINT32_C(19);
    const double resolution = 0.001;
    const double max_val = resolution * MAX_BCD_19_BIT;

    const double values[] = {max_val + resolution, max_val + 1, max_val * 2};

    for (size_t i = 0; i < ARRAY_SIZE(values); i++)
    {
        char msg[512] = {0};

        snprintf(msg, sizeof(msg), " Message: iter: %d value  = %f", i, values[i]);

        a429_encode_bcd(&word, values[i], payload_begin, payload_width, resolution, &error);

        TEST_ASSERT_EQUAL_MESSAGE(A429_ERR_OUT_OF_RANGE, error, msg);
    }
}

int main(void)
{
    test_seed = (unsigned)time(NULL);
    srand(test_seed);
    UNITY_BEGIN();
    RUN_TEST(test_bcd_invariant_randomized);
    RUN_TEST(test_bcd_boundry_values);
    RUN_TEST(test_bcd_out_of_range);
    return UNITY_END();
}