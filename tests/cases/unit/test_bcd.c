#include "a429/bcd.h"
#include "decode_mock.h"
#define UNITY_INCLUDE_CONFIG_H

#include "unity.h"
#include "test_utils.h"

test_word_bcd_t test_mock_word;
static unsigned int test_seed;

void setUp(void) {}
void tearDown(void) {}

void test_bcd_invariant_randomized(void)
{
    for (size_t i = 0; i < RANDOM_TEST_ITERATIONS; i++)
    {
        create_random_a429_bcd_word(&test_mock_word);

        int8_t err_code = 0;

        a429_encode_bcd(
            &test_mock_word.word, test_mock_word.value,
            test_mock_word.digit_count, test_mock_word.resolution, &err_code);

        if (err_code)
        {
            char message[256];
            set_invariant_randomized_error_msg(message, err_code, test_seed, i);
            TEST_FAIL_MESSAGE(message);
        }

        const double decoded_value = a429_decode_bcd(test_mock_word.word, test_mock_word.digit_count,
                                                     test_mock_word.resolution, &err_code);

        if (err_code)
        {
            char message[256];
            set_invariant_randomized_error_msg(message, err_code, test_seed, i);
            TEST_FAIL_MESSAGE(message);
        }

        const double allowed_delta =
            test_mock_word.resolution / 2.0;

        TEST_ASSERT_DOUBLE_WITHIN(allowed_delta, test_mock_word.value, decoded_value);
    }
}

int main(void)
{
    test_seed = (unsigned)time(NULL);
    srand(test_seed);
    UNITY_BEGIN();
    RUN_TEST(test_bcd_invariant_randomized);
    return UNITY_END();
}