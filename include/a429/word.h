#ifndef A429_WORD_H
#define A429_WORD_H
#include <stdint.h>

#define A429_LABEL_SHIFT    0U
#define A429_SDI_SHIFT      8U
#define A429_DATA_SHIFT     10U
#define A429_SSM_SHIFT      29U
#define A429_PARITY_SHIFT   31U

#define A429_LABEL_MASK     0xFFU
#define A429_SDI_MASK       0x03U
#define A429_DATA_MASK      0x7FFFFU
#define A429_SSM_MASK       0x03U
#define A429_PARITY_MASK    0x01U

#define A429_WORD_MASK      0x7FFFFFFFU

/**
 * @brief  Extracts the Label field (ARINC 429 bits 1-8 / C bits 0-7).
 * @param  word: The 32-bit raw ARINC word.
 * @return 8-bit unsigned label value.
 * @note   Label is the first 8 bits. Octal representation is standard for this
 * field.
 */
static inline uint8_t a429_get_label(uint32_t word) {
  return word & A429_LABEL_MASK;
}
/**
 * @brief  Extracts the SDI (Source/Destination Identifier) field (ARINC 429
 * bits 9-10 / C bits 8-9).
 * @param  word: The 32-bit raw ARINC word.
 * @return 2-bit value (0-3).
 * @note   Used to identify the source or destination of the data.
 */
static inline uint8_t a429_get_sdi(uint32_t word) {
  return (uint8_t)(word >> A429_SDI_SHIFT) & A429_SDI_MASK;
}

/**
 * @brief  Extracts the DATA field (ARINC 429 bits 11-29 / C bits 10-28).
 * @param  word: The 32-bit raw ARINC word.
 * @return 19-bit data value.
 * @note   This field's interpretation depends on the coding type (BNR, BCD, or
 * Discrete).
 */
static inline uint32_t a429_get_data(uint32_t word) {
  return (word >> A429_DATA_SHIFT) & A429_DATA_MASK;
}
/**
 * @brief  Extracts the SSM (Sign/Status Matrix) field (ARINC 429 bits 30-31 / C
 * bits 29-30).
 * @param  word: The 32-bit raw ARINC word.
 * @return 2-bit value.
 * @note   Indicates hardware condition, operational mode, or validity of data.
 */
static inline uint8_t a429_get_ssm(uint32_t word) {
  return (uint8_t)(word >> A429_SSM_SHIFT) & A429_SSM_MASK;
}

/**
 * @brief  Extracts the PARITY field (ARINC 429 bit 32 / C bit 31).
 * @param  word: The 32-bit raw ARINC word.
 * @return 1-bit value.
 * @note   ARINC 429 typically uses Odd Parity for error detection.
 */
static inline uint8_t a429_get_parity(uint32_t word) {
  return (uint8_t)(word >> A429_PARITY_SHIFT) & A429_PARITY_MASK;
}

/**
 * @brief  Extracts all bits except the parity bit (ARINC 429 bits 1-31).
 * @param  word: The 32-bit raw ARINC word.
 * @return 31-bit value used for parity calculation or data integrity checks.
 * @note   Masks out the 32nd bit (MSB / Parity bit).
 */
static inline uint32_t a429_get_without_parity(uint32_t word) {
  return word & A429_WORD_MASK;
}
#endif
