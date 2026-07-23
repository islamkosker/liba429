
#ifndef TEST_DECODE_MOCK
#define TEST_DECODE_MOCK

#include "a429/word.h"
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
} test_word_t;

unsigned long long large_rand()
{
  unsigned long long r = 0;
  for (int i = 0; i < 5; i++)
  {
    r = (r << 15) | (rand() & 0x7FFF);
  }
  return r;
}

static inline void create_random_a429_word(test_word_t *word)
{
  srand((unsigned)time(NULL));
  word->bit_count = (rand() % A429_DATA_BIT_COUNT) + 1;

  uint32_t capacity = 1U << (word->bit_count - 1);
  uint32_t max_value = capacity - 1;

  word->scale = 10.0 + (((double)rand() / RAND_MAX) * 99990.0);
  double resolution = word->scale / (double)capacity;

  double random_multiplier = (((double)rand() / RAND_MAX) * 2.0) - 1.0;
  double max_physical_value = max_value * resolution;

  word->value = random_multiplier * max_physical_value;
}

#endif