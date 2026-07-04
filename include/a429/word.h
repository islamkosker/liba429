#ifndef A429_WORD_H
#define A429_WORD_H

/**
 * @brief  Extracts the Label field (ARINC 429 bits 1-8 / C bits 0-7).
 * @param  word: The 32-bit raw ARINC word.
 * @return 8-bit unsigned label value.
 * @note   Label is the first 8 bits. Octal representation is standard for this
 * field.
 */
#define A429_GET_LABEL(word) ((uint8_t)((word) & 0x000000FFU))

/**
 * @brief  Extracts the SDI (Source/Destination Identifier) field (ARINC 429
 * bits 9-10 / C bits 8-9).
 * @param  word: The 32-bit raw ARINC word.
 * @return 2-bit value (0-3).
 * @note   Used to identify the source or destination of the data.
 */
#define A429_GET_SDI(word) ((uint8_t)(((word) >> 8) & 0x00000003U))

/**
 * @brief  Extracts the DATA field (ARINC 429 bits 11-29 / C bits 10-28).
 * @param  word: The 32-bit raw ARINC word.
 * @return 19-bit data value.
 * @note   This field's interpretation depends on the coding type (BNR, BCD, or
 * Discrete).
 */
#define A429_GET_DATA(word) ((uint32_t)(((word) >> 10) & 0x0007FFFFU))

/**
 * @brief  Extracts the SSM (Sign/Status Matrix) field (ARINC 429 bits 30-31 / C
 * bits 29-30).
 * @param  word: The 32-bit raw ARINC word.
 * @return 2-bit value.
 * @note   Indicates hardware condition, operational mode, or validity of data.
 */
#define A429_GET_SSM(word) ((uint8_t)(((word) >> 29) & 0x00000003U))

/**
 * @brief  Extracts the PARITY field (ARINC 429 bit 32 / C bit 31).
 * @param  word: The 32-bit raw ARINC word.
 * @return 1-bit value.
 * @note   ARINC 429 typically uses Odd Parity for error detection.
 */
#define A429_GET_PARITY(word) ((uint8_t)(((word) >> 31) & 0x00000001U))

/**
 * @brief  Extracts all bits except the parity bit (ARINC 429 bits 1-31).
 * @param  word: The 32-bit raw ARINC word.
 * @return 31-bit value used for parity calculation or data integrity checks.
 * @note   Masks out the 32nd bit (MSB / Parity bit).
 */
#define A429_GET_WITHOUT_PARITY(word) ((uint32_t)((word) & 0x7FFFFFFFU))

#endif
