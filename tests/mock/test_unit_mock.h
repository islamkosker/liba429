
#ifndef TEST_DECODE_MOCK
#define TEST_DECODE_MOCK

#include "a429_word.h"
#include <stdint.h>

#include <stdlib.h>
#include <time.h>

#define CMP_DELTA 0.001f

typedef struct
{
  double scale;
  uint32_t word;
  double value;
  uint8_t payload_begin;
  uint8_t payload_width;
  double resolution;
} test_word_bnr_t;

typedef struct
{
  double scale;
  uint32_t word;
  double value;
  uint8_t payload_begin;
  uint8_t payload_width;
  double resolution;
} test_word_bcd_t;

const unsigned long long large_rand()
{
  unsigned long long r = 0;
  for (int i = 0; i < 5; i++)
  {
    r = (r << 15) | (rand() & 0x7FFF);
  }
  return r;
}

static inline void create_random_a429_bnr_word(test_word_bnr_t *word)
{
  static uint8_t possible_width[] = {21, 19, 23, 21};
  static uint8_t possible_begin[] = {9, 11};
  size_t r = rand() % 4;

  word->payload_width = possible_width[r];

  word->payload_begin = possible_begin[r % 2];

  uint32_t capacity = UINT32_C(1) << (word->payload_width - 1U);

  uint32_t max_positive = capacity - UINT32_C(1);

  word->scale = 10.0 + ((double)rand() / (double)RAND_MAX) * 99990.0;

  word->resolution = word->scale / (double)capacity;

  double max_positive_value = (double)max_positive * word->resolution;

  double random_multiplier = ((double)rand() / (double)RAND_MAX) * 2.0 - 1.0;

  word->value = random_multiplier * max_positive_value;
}

static inline void create_random_a429_bcd_word(test_word_bcd_t *word)
{
  static uint8_t poosible_choose[4][2] = {
      [0] = {9, 21},
      [1] = {9, 23},
      [2] = {11, 19},
      [3] = {11, 21},
  };

  size_t r = rand() % 4;

  word->payload_begin = poosible_choose[r][0];
  word->payload_width = poosible_choose[r][1];

  const uint8_t full_digits = word->payload_width / 4;
  const uint8_t remaining_bits = word->payload_width % 4;

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

  static const double possible_resolutions[] = {0.001, 0.01, 0.1, 1.0, 10.0};

  word->resolution =
      possible_resolutions[rand() % (sizeof(possible_resolutions) /
                                     sizeof(possible_resolutions[0]))];

  uint32_t raw_bcd_int = (uint32_t)(large_rand() % (max_bcd_int + 1U));

  word->value = (double)raw_bcd_int * word->resolution;

  word->scale = (double)max_bcd_int * word->resolution;
}

#endif