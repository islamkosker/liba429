#ifndef A429_DISCRETE_H
#define A429_DISCRETE_H

#include <stdint.h>

/**
 * @brief Extract a discrete payload field from an ARINC 429 word.
 *
 * Extracts a bit field from the specified payload position and width.
 * The extracted field is returned right-aligned in the result.
 *
 * @param[in] word ARINC 429 word containing the discrete field.
 * @param[in] payload_begin Zero-based bit position of the payload field.
 * @param[in] payload_width Width of the payload field in bits.
 *
 * @return Extracted discrete field value.
 */
uint32_t a429_get_discrete_field(uint32_t word, uint8_t payload_begin, uint8_t payload_width);

/**
 * @brief Insert a discrete payload field into an ARINC 429 word.
 *
 * Inserts the specified value into the configured payload field.
 * Only the bits within the specified payload width are affected.
 *
 * @param[out] word ARINC 429 word to be updated.
 * @param[in] value Discrete field value to insert.
 * @param[in] payload_begin Zero-based bit position of the payload field.
 * @param[in] payload_width Width of the payload field in bits.
 */
void a429_set_discrete_field(uint32_t *word, uint32_t value, uint8_t payload_begin, uint8_t payload_width);

#endif // A429_DISCRETE_H
