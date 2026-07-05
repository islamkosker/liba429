#ifndef A429_BNR_H
#define A429_BNR_H

#include <stdint.h>
/**
 * @brief   Decodes a BNR (Binary) formatted ARINC 429 word into a double value.
 * @param  word         The 32-bit raw ARINC word.
 * @param  bit_count    The number of bits used for BNR data (including the sign
 * bit). Max 19.
 * @param  scale_factor The maximum value scale factor for this parameter.
 * @param error_code    The error code
 * @return double       The decoded real-world value (e.g., altitude, speed).
 */

double a429_decode_bnr(uint32_t word, uint8_t bit_count, double scale_factor,
                       int8_t *error_code);

#endif