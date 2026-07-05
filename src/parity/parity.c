#include "a429/parity.h"
#include "a429/word.h"
#include <stdint.h>

static inline bool is_odd(uint32_t n) { return (n & 1U) != 0U; }

static inline uint8_t count_set_bits(uint32_t n) {
  uint8_t count = 0U;

  while (n != 0U) {
    n &= (n - 1U);
    count++;
  }

  return count;
}

bool a429_verify_parity(uint32_t word) { return is_odd(count_set_bits(word)); }

uint8_t a429_compute_parity(uint32_t word) {
  return !a429_verify_parity(a429_get_without_parity(word));
}

uint32_t a429_apply_parity(uint32_t word) {
  uint8_t parity = a429_compute_parity(word);
  word = a429_get_without_parity(word);
  word |= ((uint32_t)parity << 31);
  return word;
}