#ifndef LIBA429_PARITY_H
#define LIBA429_PARITY_H

#include <stdint.h>
#include <stdbool.h>

/**
 * @brief  Computes the Odd Parity bit for a 32-bit ARINC word.
 * @param  word: The 32-bit raw ARINC word (bit 31/parity bit is ignored).
 * @return The calculated parity bit (0 or 1) that makes the total '1' count ODD.
 */
uint8_t a429_compute_parity(uint32_t word);

/**
 * @brief  Verifies if the given 32-bit word has a valid Odd Parity.
 * @param  word: The 32-bit raw ARINC word.
 * @return true if parity is valid (total count of 1s is ODD), false otherwise.
 */
bool a429_verify_parity(uint32_t word);


/**
 * @brief  Computes and sets the odd parity bit for a 32-bit ARINC 429 word.
 * @param  word  The 32-bit ARINC 429 word. The parity bit (bit 32) is ignored during computation.
 * @return The ARINC 429 word with the correct odd parity bit set.
 */
uint32_t a429_apply_parity(uint32_t word);

#endif // LIBA429_PARITY_H