
#include "a429/bnr.h"
#include "decode_mock.h"
#define UNITY_INCLUDE_CONFIG_H

#include "unity.h"

test_word_t test_mock_word;

void setUp(void)
{

  create_random_a429_word(&test_mock_word);
}
void tearDown(void) {}

void test_codec(void)
{

  int8_t err_code = 0;
  a429_encode_bnr(&test_mock_word.word, test_mock_word.value,
                  test_mock_word.bit_count, test_mock_word.scale, &err_code);
  if (err_code)
  {
    char message[512];
    snprintf(message, sizeof(message), "Fail to encoding word: status %d",
             err_code);
    TEST_FAIL_MESSAGE(message);
    TEST_FAIL();
  }

  double test_value =
      a429_decode_bnr(test_mock_word.word, test_mock_word.bit_count,
                      test_mock_word.scale, &err_code);
  if (err_code)
  {
    char message[512];
    snprintf(message, sizeof(message), "Fail to decoding word: status %d",
             err_code);
    TEST_FAIL_MESSAGE(message);
  }
  double resolution = test_mock_word.scale / (double)(1 << (test_mock_word.bit_count - 1));
  double allowed_delta = resolution / 2.0;

  TEST_ASSERT_DOUBLE_WITHIN(allowed_delta, test_mock_word.value, test_value);
}

int main(void)
{
  UNITY_BEGIN();

  RUN_TEST(test_codec);
  return UNITY_END();
}