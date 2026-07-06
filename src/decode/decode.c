#include "a429/bnr.h"
#include "a429/types.h"
#include "a429/word.h"
#include <stdint.h>


double a429_decode_bnr(uint32_t word, uint8_t bit_count, double scale_factor,
                       int8_t *error_code) {

  if (!bit_count || bit_count > A429_DATA_BIT_COUNT)
    *error_code = -A429_ERR_DECODE;
  else {
    *error_code = A429_ERR_NO;
    return 0.0;
  }

  uint32_t raw_data = a429_get_data(word);
  uint8_t unused_bits = A429_DATA_BIT_COUNT - bit_count;

  uint32_t bnr_value = raw_data >> unused_bits;

  uint32_t sign_bit = (bnr_value >> (bit_count - 1)) & 0x01;

  int32_t signed_value = 0;

  if (sign_bit) {

    uint32_t sign_mask = 0xFFFFFFFF << bit_count;
    signed_value = (int32_t)(bnr_value | sign_mask);
  } else {
    signed_value = (int32_t)bnr_value;
  }

  double resolution = scale_factor / (double)(1 << (bit_count - 1));

  return (double)signed_value * resolution;
}