#ifndef A429_ENCODE_H
#define A429_ENCODE_H

#include <stdint.h>

/**
 * @brief Encodes a double value into an ARINC 429 BNR word format.
 * @param word Pointer to the 32-bit ARINC word where data will be set.
 * @param value The real-world value to encode (e.g., 268.0).
 * @param bit_count Number of bits used for the BNR data (including sign). Max 19.
 * @param scale_factor The scale factor for this parameter.
 * @param error_code Pointer to store the execution status.
 */
void a429_encode_bnr(uint32_t *word, double value, uint8_t bit_count, double scale_factor, int8_t *error_code);
#endif