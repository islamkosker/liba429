
#ifndef TEST_DECODE_MOCK
#define TEST_DECODE_MOCK

#include "a429/a429_word.h"
#include <stdint.h>

#include <stdlib.h>
#include <time.h>

#define CMP_DELTA 0.001f

typedef struct
{
  double scale;
  uint32_t word;
  double value;
  uint8_t bit_count;
  double resolution;
} test_word_bnr_t;

typedef struct
{
  double scale;
  uint32_t word;
  double value;
  uint8_t digit_count;
  double resolution;
} test_word_bcd_t;

unsigned long long large_rand()
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

  word->bit_count = (rand() % A429_DATA_BIT_COUNT - 1) + 2;

  uint32_t capacity = 1U << (word->bit_count - 1);
  uint32_t max_value = capacity - 1;

  word->scale = 10.0 + (((double)rand() / RAND_MAX) * 99990.f);
  double resolution = word->scale / (double)capacity;

  word->resolution = resolution;

  double random_multiplier = (((double)rand() / RAND_MAX) * 2.0) - 1.0;
  double max_physical_value = max_value * resolution;

  word->value = random_multiplier * max_physical_value;
}

static inline void create_random_a429_bcd_word(test_word_bcd_t *word)
{

  word->digit_count = (rand() % 5) + 1;
  static const uint32_t max_bcd_limits[] = {0, 9, 99, 999, 9999, 79999};
  uint32_t max_bcd_int = max_bcd_limits[word->digit_count];

  static const double possible_resolutions[] = {0.001, 0.01, 0.1, 1.0, 10.0};
  word->resolution = possible_resolutions[rand() % 5];

  uint32_t raw_bcd_int = (uint32_t)(large_rand() % (max_bcd_int + 1));

  word->value = (double)raw_bcd_int * word->resolution;

  word->scale = (double)max_bcd_int * word->resolution;
}

#endif