#ifndef A429_WORD_H
#define A429_WORD_H

#include <stdint.h>
#include "liba429.h"

/**
 * @brief Bit position of the Label field in the software word representation.
 *
 * The Label occupies software bits 0-7, corresponding to ARINC 429 bits 1-8.
 */
#define A429_LABEL_SHIFT 0U

/**
 * @brief Bit position of the SDI field in the software word representation.
 *
 * The SDI occupies software bits 8-9, corresponding to ARINC 429 bits 9-10.
 */
#define A429_SDI_SHIFT 8U

/**
 * @brief Bit position of the standard DATA field in the software word representation.
 *
 * The standard DATA field occupies software bits 10-28, corresponding to
 * ARINC 429 bits 11-29.
 */
#define A429_DATA_SHIFT 10U

/**
 * @brief Bit position of the SSM field in the software word representation.
 *
 * The SSM occupies software bits 29-30, corresponding to ARINC 429 bits 30-31.
 */
#define A429_SSM_SHIFT 29U

/**
 * @brief Bit position of the parity bit in the software word representation.
 *
 * The parity bit occupies software bit 31, corresponding to ARINC 429 bit 32.
 */
#define A429_PARITY_SHIFT 31U

/**
 * @brief Bit mask for the 8-bit Label field.
 */
#define A429_LABEL_MASK 0xFFU

/**
 * @brief Bit mask for the 2-bit SDI field.
 */
#define A429_SDI_MASK 0x03U

/**
 * @brief Bit mask for the standard 19-bit DATA field.
 */
#define A429_DATA_MASK 0x7FFFFU

/**
 * @brief Bit mask for the 2-bit SSM field.
 */
#define A429_SSM_MASK 0x03U

/**
 * @brief Bit mask for the single parity bit.
 */
#define A429_PARITY_MASK 0x01U

/**
 * @brief Mask covering ARINC 429 bits 1-31.
 *
 * The parity bit at software bit 31 is excluded.
 */
#define A429_WORD_MASK 0x7FFFFFFFU

/**
 * @brief Mask covering the ARINC 429 payload area excluding the Label field.
 */
#define A429_PAYLOAD_MASK 0xFFFFFF00U

/**
 * @brief Lookup table used to reverse the bit order of an ARINC 429 Label.
 */
extern const uint8_t a429_bit_reverse_table[];

/**
 * @brief Extract the Label field from an ARINC 429 word.
 *
 * Extracts software bits 0-7, corresponding to ARINC 429 bits 1-8.
 *
 * @param[in] word ARINC 429 word in software representation.
 *
 * @return 8-bit Label value.
 */
static inline uint8_t a429_get_label(a429_word_t word)
{
  return word & A429_LABEL_MASK;
}

/**
 * @brief Extract the SDI field from an ARINC 429 word.
 *
 * Extracts software bits 8-9, corresponding to ARINC 429 bits 9-10.
 *
 * @param[in] word ARINC 429 word in software representation.
 *
 * @return 2-bit SDI value in the range 0-3.
 */
static inline uint8_t a429_get_sdi(a429_word_t word)
{
  return (uint8_t)(word >> A429_SDI_SHIFT) & A429_SDI_MASK;
}

/**
 * @brief Extract the standard DATA field from an ARINC 429 word.
 *
 * Extracts software bits 10-28, corresponding to ARINC 429 bits 11-29.
 *
 * @param[in] word ARINC 429 word in software representation.
 *
 * @return 19-bit DATA field.
 *
 * @note The interpretation of the DATA field depends on the selected
 *       encoding type.
 */
static inline a429_word_t a429_get_data(a429_word_t word)
{
  return (word >> A429_DATA_SHIFT) & A429_DATA_MASK;
}

/**
 * @brief Extract a bit field from an ARINC 429 word.
 *
 * Extracts a field beginning at the specified one-based ARINC bit position
 * and returns the result right-aligned.
 *
 * @param[in] word ARINC 429 word in software representation.
 * @param[in] begin One-based ARINC bit position of the field.
 * @param[in] width Width of the field in bits.
 *
 * @return Extracted field value.
 *
 * @note The @p begin parameter follows the ARINC 429 bit numbering convention,
 *       where bit 1 is the least significant bit of the software word.
 */
static inline a429_word_t a429_get_bits(a429_word_t word, uint8_t begin, uint8_t width)
{
  uint8_t shift = begin - 1U;
  uint32_t mask = (UINT32_C(1) << width) - UINT32_C(1);

  return (word >> shift) & mask;
}

/**
 * @brief Extract the SSM field from an ARINC 429 word.
 *
 * Extracts software bits 29-30, corresponding to ARINC 429 bits 30-31.
 *
 * @param[in] word ARINC 429 word in software representation.
 *
 * @return 2-bit SSM value.
 */
static inline uint8_t a429_get_ssm(a429_word_t word)
{
  return (uint8_t)(word >> A429_SSM_SHIFT) & A429_SSM_MASK;
}

/**
 * @brief Extract the parity bit from an ARINC 429 word.
 *
 * Extracts software bit 31, corresponding to ARINC 429 bit 32.
 *
 * @param[in] word ARINC 429 word in software representation.
 *
 * @return Parity bit value, either 0 or 1.
 */
static inline uint8_t a429_get_parity(a429_word_t word)
{
  return (uint8_t)(word >> A429_PARITY_SHIFT) & A429_PARITY_MASK;
}

/**
 * @brief Remove the parity bit from an ARINC 429 word.
 *
 * Clears software bit 31, corresponding to ARINC 429 bit 32, while
 * preserving ARINC 429 bits 1-31.
 *
 * @param[in] word ARINC 429 word in software representation.
 *
 * @return Word value with the parity bit cleared.
 */
static inline a429_word_t a429_get_without_parity(a429_word_t word)
{
  return word & A429_WORD_MASK;
}

/**
 * @brief Set the Label field of an ARINC 429 word.
 *
 * Updates software bits 0-7, corresponding to ARINC 429 bits 1-8,
 * while preserving all other fields.
 *
 * @param[out] word ARINC 429 word to modify.
 * @param[in] label 8-bit Label value.
 */
static inline void a429_set_label(a429_word_t *word, uint8_t label)
{
  *word = (*word & ~A429_LABEL_MASK) | (label & A429_LABEL_MASK);
}

/**
 * @brief Set the SDI field of an ARINC 429 word.
 *
 * Updates software bits 8-9, corresponding to ARINC 429 bits 9-10,
 * while preserving all other fields.
 *
 * @param[out] word ARINC 429 word to modify.
 * @param[in] sdi 2-bit SDI value.
 */
static inline void a429_set_sdi(a429_word_t *word, uint8_t sdi)
{
  *word = (*word & ~(A429_SDI_MASK << A429_SDI_SHIFT)) |
          (((a429_word_t)sdi & A429_SDI_MASK) << A429_SDI_SHIFT);
}

/**
 * @brief Set the standard DATA field of an ARINC 429 word.
 *
 * Updates software bits 10-28, corresponding to ARINC 429 bits 11-29,
 * while preserving all other fields.
 *
 * @param[out] word ARINC 429 word to modify.
 * @param[in] data 19-bit DATA value.
 */
static inline void a429_set_data(a429_word_t *word, a429_word_t data)
{
  *word = (*word & ~(A429_DATA_MASK << A429_DATA_SHIFT)) |
          ((data & A429_DATA_MASK) << A429_DATA_SHIFT);
}

/**
 * @brief Set a bit field in an ARINC 429 word.
 *
 * Replaces the specified field with the supplied payload while preserving
 * all bits outside the field.
 *
 * @param[out] word ARINC 429 word to modify.
 * @param[in] payload Value to insert into the field.
 * @param[in] begin One-based ARINC bit position of the field.
 * @param[in] width Width of the field in bits.
 *
 * @note The @p begin parameter follows the ARINC 429 bit numbering convention.
 *       Only the least significant @p width bits of @p payload are inserted.
 */
static inline void a429_set_bits(a429_word_t *word, a429_word_t payload, uint8_t begin, uint8_t width)
{
  uint8_t shift = begin - 1U;
  a429_word_t mask = (UINT32_C(1) << width) - UINT32_C(1);

  *word &= ~(mask << shift);
  *word |= (payload & mask) << shift;
}

/**
 * @brief Set the SSM field of an ARINC 429 word.
 *
 * Updates software bits 29-30, corresponding to ARINC 429 bits 30-31,
 * while preserving all other fields.
 *
 * @param[out] word ARINC 429 word to modify.
 * @param[in] ssm 2-bit SSM value.
 */
static inline void a429_set_ssm(a429_word_t *word, uint8_t ssm)
{
  *word = (*word & ~(A429_SSM_MASK << A429_SSM_SHIFT)) |
          (((a429_word_t)ssm & A429_SSM_MASK) << A429_SSM_SHIFT);
}

#endif
