#include "a429_bnr.h"
#include "a429_word.h"

#include <math.h>
#include <stddef.h>
#include <stdint.h>

double a429_decode_bnr(a429_word_t word, uint8_t payload_begin, uint8_t payload_width,
                       double scale_factor, a429_error_t *error_code)
{

  if (!payload_width || payload_width > A429_MAX_PAYLOAD_WIDTH)
  {
    *error_code = A429_ERR_DECODE;
    return 0.0;
  }
  else
  {
    *error_code = A429_ERR_NO;
  }

  uint32_t bnr_value = a429_get_bits(word, payload_begin, payload_width);

  uint32_t sign_bit = (bnr_value >> (payload_width - 1)) & UINT32_C(1);

  int32_t signed_value = 0;

  if (sign_bit != 0u)
  {

    uint32_t sign_mask = UINT32_MAX << payload_width;
    signed_value = (int32_t)(bnr_value | sign_mask);
  }
  else
  {
    signed_value = (int32_t)bnr_value;
  }

  double resolution = scale_factor / (double)(UINT32_C(1) << (payload_width - 1U));

  return (double)signed_value * resolution;
}

void a429_encode_bnr(a429_word_t *word, double value, uint8_t payload_begin, uint8_t payload_width,
                     double scale_factor, a429_error_t *error_code)
{
  if ((payload_width == 0U) || (payload_width > A429_MAX_PAYLOAD_WIDTH))
  {
    *error_code = A429_ERR_ENCODE;
    return;
  }

  if ((value >= scale_factor) || (value < -scale_factor))
  {
    *error_code = A429_ERR_OUT_OF_RANGE;
    return;
  }

  *error_code = A429_ERR_NO;

  const double resolution = scale_factor / (double)(UINT32_C(1) << (payload_width - 1U));

  const int32_t scaled_int = (int32_t)round(value / resolution);

  const uint32_t mask = (UINT32_C(1) << payload_width) - UINT32_C(1);

  const uint32_t bnr_value = (uint32_t)scaled_int & mask;

  a429_set_bits(word, bnr_value, payload_begin, payload_width);
}