#ifndef A429_BNR_H
#define A429_BNR_H

#include "a429_types.h"
#include "a429_error.h"
#include <stdint.h>

/**
 * @brief   Decodes a BNR (Binary) formatted ARINC 429 word into a double value.
 * @param  word         The 32-bit raw ARINC word.
 * @param  payload_width    The number of bits used for BNR data (including the sign
 * bit). Max 19.
 * @param  resolution The maximum value scale factor for this parameter.
 * @param error_code    The error code
 * @return double       The decoded real-world value (e.g., altitude, speed).
 */
double a429_decode_bnr(a429_word_t word, uint8_t payload_begin, uint8_t payload_width,
                       double resolution, a429_error_t *error_code);

/**
 * @brief Encodes a double value into an ARINC 429 BNR word format.
 * @param word Pointer to the 32-bit ARINC word where data will be set.
 * @param value The real-world value to encode (e.g., 268.0).
 * @param payload_width Number of bits used for the BNR data (including sign).
 * Max 19.
 * @param scale_factor The scale factor for this parameter.
 * @param error_code Pointer to store the execution status.
 */
void a429_encode_bnr(a429_word_t *word, double value, uint8_t payload_begin, uint8_t payload_width,
                     double scale_factor, a429_error_t *error_code);

#endif