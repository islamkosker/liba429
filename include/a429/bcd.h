#ifndef A429_BCD_H
#define A429_BCD_H
#include <stddef.h>
#include <stdint.h>
#include "a429/types.h"

/**
 * @brief Decodes a BCD (Binary code Decimal) formatted ARINC 429 word into a double value.
 * @param word         The 32-bit raw ARINC word.
 * @param digit_count  Digit count of used for the BCD data
 * @param resolution   Value resulation.
 * @param error_code    The error code
 * @return double       The decoded real-world value (e.g., altitude, speed).
 */

double a429_decode_bcd(const a429_word_t word, const uint8_t digit_count,
                       const double resolution, int8_t *error_code);

/**
 * @brief Encodes a double value into an ARINC 429 BCD (Binary code Decimal) word format.
 * @param word          Pointer to the 32-bit ARINC word where data will be set.
 * @param value         The real-world value to encode (e.g., 268.0).
 * @param digit_count     Digit count of used for the BCD data
 * @param resolution    Value resulation.
 * @param error_code    Pointer to store the execution status.
 */
void a429_encode_bcd(a429_word_t *word, const double value, const uint8_t digit_count,
                     const double resolution, int8_t *error_code);

#endif // A429_BCD_H