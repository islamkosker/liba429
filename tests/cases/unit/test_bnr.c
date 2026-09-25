
#include "a429/a429_bnr.h"
#include "decode_mock.h"
#define UNITY_INCLUDE_CONFIG_H

#include "unity.h"
#include "test_utils.h"
test_word_bnr_t test_mock_word;
static unsigned int test_seed;

void setUp(void) {}
void tearDown(void) {}

void test_bnr_invariant_randomized(void)
{

  for (size_t i = 0; i < RANDOM_TEST_ITERATIONS; i++)
  {
    create_random_a429_bnr_word(&test_mock_word);

    a429_error_t err_code = A429_ERR_NO;
    a429_encode_bnr(&test_mock_word.word, test_mock_word.value,
                    test_mock_word.bit_count, test_mock_word.scale, &err_code);
    if (err_code)
    {
      char message[512];
      set_invariant_randomized_error_msg(message, err_code, test_seed, i);

      TEST_FAIL_MESSAGE(message);
    }

    double test_value =
        a429_decode_bnr(test_mock_word.word, test_mock_word.bit_count,
                        test_mock_word.scale, &err_code);
    if (err_code)
    {
      char message[512];
      set_invariant_randomized_error_msg(message, err_code, test_seed, i);

      TEST_FAIL_MESSAGE(message);
    }
    const double allowed_delta =
        test_mock_word.resolution / 2.0;

    TEST_ASSERT_DOUBLE_WITHIN(allowed_delta, test_mock_word.value, test_value);
  }
}

int main(void)
{
  test_seed = (unsigned)time(NULL);
  srand(test_seed);
  UNITY_BEGIN();
  RUN_TEST(test_bnr_invariant_randomized);

  return UNITY_END();
}