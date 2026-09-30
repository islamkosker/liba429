
#include "a429_bnr.h"
#include "test_unit_mock.h"
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
    a429_encode_bnr(&test_mock_word.word, test_mock_word.value, test_mock_word.payload_begin,
                    test_mock_word.payload_width, test_mock_word.scale, &err_code);
    if (err_code)
    {
      char message[512];
      snprintf(message, sizeof(message),
               ":>> status = %d seed = %d iter = %d value = %f", err_code, test_seed, i, test_mock_word.value);

      TEST_FAIL_MESSAGE(message);
    }

    double test_value =
        a429_decode_bnr(test_mock_word.word, test_mock_word.payload_begin, test_mock_word.payload_width,
                        test_mock_word.scale, &err_code);
    if (err_code)
    {
      char message[512];
      snprintf(message, sizeof(message),
               ":>> status = %d seed = %d iter = %d value = %f", err_code, test_seed, i, test_mock_word.value);

      TEST_FAIL_MESSAGE(message);
    }

    TEST_ASSERT_DOUBLE_WITHIN(test_mock_word.resolution, test_mock_word.value, test_value);
  }
}

void test_bnr_boundry_values(void)
{
  const uint8_t payload_begin = UINT32_C(11);
  const uint8_t payload_width = UINT32_C(19);
  const double scale = 100.f;
  const double resolution = scale / (double)(UINT32_C(1) << (payload_width - UINT32_C(1)));

  const double values[] = {0., resolution, (-resolution), (scale - resolution), (-scale + resolution)};

  for (size_t i = 0; i < ARRAY_SIZE(values); i++)
  {
    a429_word_t word = 0;
    a429_error_t encode_error = A429_ERR_NO;
    a429_error_t decode_error = A429_ERR_NO;

    a429_encode_bnr(&word, values[i], payload_begin, payload_width, scale, &encode_error);

    TEST_ASSERT_EQUAL(A429_ERR_NO, encode_error);

    const double decoded = a429_decode_bnr(word, payload_begin, payload_width, scale, &decode_error);
    TEST_ASSERT_EQUAL(A429_ERR_NO, decode_error);

    TEST_ASSERT_DOUBLE_WITHIN(resolution, values[i], decoded);
  }
}

void test_bnr_out_of_range(void)
{
  a429_word_t word = 0;
  a429_error_t error = A429_ERR_NO;
  const uint8_t payload_begin = UINT32_C(11);
  const uint8_t payload_width = UINT32_C(19);
  const double scale = 100.f;
  const double resolution = scale / (double)(UINT32_C(1) << (payload_width - UINT32_C(1)));

  const double values[] = {scale + 0.000001, scale + 1, scale * 2};

  for (size_t i = 0; i < ARRAY_SIZE(values); i++)
  {

    a429_encode_bnr(&word, values[i], payload_begin, payload_width, scale, &error);

    TEST_ASSERT_EQUAL(A429_ERR_OUT_OF_RANGE, error);
  }
}

int main(void)
{
  test_seed = (unsigned)time(NULL);
  srand(test_seed);
  UNITY_BEGIN();
  RUN_TEST(test_bnr_invariant_randomized);
  RUN_TEST(test_bnr_boundry_values);
  RUN_TEST(test_bnr_out_of_range);
  return UNITY_END();
}