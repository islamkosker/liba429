#ifndef LIBA429_H
#define LIBA429_H

#include <stdint.h>
#include <stdbool.h>

/**
 * @brief One-based ARINC 429 bit position of the standard data field.
 *
 * The standard ARINC 429 data field occupies bits 11 through 29.
 */
#define A429_DATA_BEGIN 11U

/**
 * @brief One-based ARINC 429 bit position of the SDI field.
 *
 * The SDI field occupies bits 9 and 10.
 */
#define A429_SDI_BEGIN 9U

/**
 * @brief Default payload width of the standard ARINC 429 data field.
 *
 * This represents the 19-bit data field spanning ARINC 429 bits 11 through
 * 29, without SDI or SSM included in the payload.
 */
#define A429_DEFAULT_DATA_WIDTH 19

/**
 * @brief Payload width when the standard data field includes SSM.
 *
 * The payload consists of the 19-bit data field plus the 2-bit SSM field.
 */
#define A429_DATA_SSM_WIDTH 21U

/**
 * @brief Payload width when the data field includes SDI.
 *
 * The payload consists of the 2-bit SDI field plus the 19-bit data field.
 */
#define A429_DATA_SDI_WIDTH A429_DATA_SSM_WIDTH

/**
 * @brief Maximum supported payload width.
 *
 * The maximum payload consists of SDI, data, and SSM fields:
 * 2 + 19 + 2 = 23 bits.
 */
#define A429_MAX_PAYLOAD_WIDTH 23U

/**
 * @brief Software representation of an ARINC 429 word.
 *
 * The value uses a 32-bit unsigned integer with the ARINC 429 fields
 * represented according to their bit positions in the software word.
 */
typedef uint32_t a429_word_t;

/**
 * @brief Wire-level representation of an ARINC 429 word.
 *
 * This type represents the 32-bit value exchanged with an ARINC 429
 * interface after applying the library's wire-level label representation.
 */
typedef uint32_t a429_wire_data_t;

/**
 * @brief Boolean value used for ARINC 429 discrete data.
 */
typedef bool a429_discrete_t;

/**
 * @brief Floating-point value used for encoded and decoded ARINC 429 data.
 */
typedef double a429_value_t;

/**
 * @brief Error codes returned by the liba429 API.
 */
typedef enum
{
    /** @brief Operation completed successfully. */
    A429_ERR_NO = 0,

    /** @brief Decoding operation failed. */
    A429_ERR_DECODE = 127,

    /** @brief Encoding operation failed. */
    A429_ERR_ENCODE = 126,

    /** @brief Value is outside the representable range. */
    A429_ERR_OUT_OF_RANGE = 125,

    /** @brief BCD value or representation is invalid. */
    A429_ERR_INVALID_BCD = 124,

    /** @brief Bit position or bit configuration is invalid. */
    A429_ERR_INVALID_BIT = 123,

    /** @brief Function argument is invalid. */
    A429_ERR_INVALID_ARG = 122,

    /** @brief ARINC 429 Label is invalid. */
    A429_ERR_INVALID_LABEL = 121,

    /** @brief Word parity does not match the expected parity. */
    A429_ERR_BAD_PARITY = 120,

    /** @brief No dictionary entry exists for the specified Label. */
    A429_ERR_UNKNOWN_LABEL = 119,

} a429_error_t;

/**
 * @brief Sign/Status Matrix values for BNR-encoded labels.
 *
 * The interpretation follows the BNR SSM definition. The two-bit SSM field
 * is represented by the corresponding enumeration value.
 */
typedef enum
{
    /** @brief 00: Failure Warning. */
    A429_SSM_BNR_FAILURE = 0x00,

    /** @brief 01: No Computed Data. */
    A429_SSM_BNR_NO_COMPUTE_DATA = 0x01,

    /** @brief 10: Functional Test. */
    A429_SSM_BNR_FUNCTIONAL_TEST = 0x02,

    /** @brief 11: Normal Operation. */
    A429_SSM_BNR_NORMAL = 0x03,

    /**
     * @brief Library-level value indicating that SSM is not used.
     *
     * This value is not an ARINC 429 two-bit SSM encoding. It is a
     * liba429-specific semantic value used by the API to indicate that
     * the corresponding label does not use the SSM field.
     */
    A429_SSM_BNR_NOT_USE = 0x04,

} a429_ssm_bnr_t;

/**
 * @brief Sign/Status Matrix values for BCD-encoded labels.
 *
 * The interpretation follows the BCD SSM definition. The two-bit SSM field
 * represents sign or directional information together with status.
 */
typedef enum
{
    /** @brief 00: Plus, North, East, Right, To, or Above. */
    A429_SSM_BCD_PLUS_NORTH_EAST = 0x00,

    /** @brief 01: No Computed Data. */
    A429_SSM_BCD_NO_COMPUTE_DATA = 0x01,

    /** @brief 10: Functional Test. */
    A429_SSM_BCD_FUNCTIONAL_TEST = 0x02,

    /** @brief 11: Minus, South, West, Left, From, or Below. */
    A429_SSM_BCD_MINUS_SOUTH_WEST = 0x03,

    /**
     * @brief Library-level value indicating that SSM is not used.
     *
     * This value is not an ARINC 429 two-bit SSM encoding. It is a
     * liba429-specific semantic value used by the API to indicate that
     * the corresponding label does not use the SSM field.
     */
    A429_SSM_BCD_NOT_USE = 0x04,

} a429_ssm_bcd_t;

/**
 * @brief Sign/Status Matrix values for discrete labels.
 */
typedef enum
{
    /** @brief 00: Verified Data / Normal Operation. */
    A429_SSM_DISC_NORMAL = 0x00,

    /** @brief 01: No Computed Data. */
    A429_SSM_DISC_NO_COMPUTE_DATA = 0x01,

    /** @brief 10: Functional Test. */
    A429_SSM_DISC_FUNCTIONAL_TEST = 0x02,

    /** @brief 11: Failure Warning. */
    A429_SSM_DISC_FAILURE = 0x03,

    /**
     * @brief Library-level value indicating that SSM is not used.
     *
     * This value is not an ARINC 429 two-bit SSM encoding. It is a
     * liba429-specific semantic value used by the API to indicate that
     * the corresponding label does not use the SSM field.
     */
    A429_SSM_DISC_NOT_USE = 0x04,

} a429_ssm_disc_t;

/**
 * @brief Source/Destination Identifier values.
 *
 * SDI identifies the intended source or destination subsystem when the
 * corresponding ARINC 429 label uses the SDI field.
 */
typedef enum
{
    /** @brief SDI value 00: all systems / SDI not used. */
    A429_SDI_ALL = 0x00,

    /** @brief SDI value 01: system 1. */
    A429_SDI_SYS1 = 0x01,

    /** @brief SDI value 10: system 2. */
    A429_SDI_SYS2 = 0x02,

    /** @brief SDI value 11: system 3. */
    A429_SDI_SYS3 = 0x03,

    /**
     * @brief Library-level value indicating that SDI is not used.
     *
     * This value is not an ARINC 429 two-bit SDI encoding. It is a
     * liba429-specific semantic value used by the API to indicate that
     * the corresponding label does not use the SDI field.
     */
    A429_SDI_NOT_USE = 0x04,

} a429_sdi_t;

/**
 * @brief SSM value container for different ARINC 429 encoding types.
 *
 * The active member is selected according to the label's encoding type.
 */
typedef union a429_ssm
{
    /** @brief SSM value for a BNR-encoded label. */
    a429_ssm_bnr_t ssm_bnr;

    /** @brief SSM value for a BCD-encoded label. */
    a429_ssm_bcd_t ssm_bcd;

    /** @brief SSM value for a discrete label. */
    a429_ssm_disc_t ssm_disc;

} a429_ssm_t;

/**
 * @brief Payload value container for ARINC 429 data.
 *
 * The active member depends on the label encoding type.
 */
typedef union a429_payload
{
    /** @brief Discrete payload value. */
    uint32_t discrete;

    /** @brief Numeric payload value used by BNR and BCD encodings. */
    a429_value_t value;

} a429_payload_u;

/**
 * @brief Payload, SDI, and SSM fields used during encoding or decoding.
 *
 * This structure provides the data fields required by the high-level
 * encode and decode APIs. The interpretation of @ref payload and @ref ssm
 * depends on the label's encoding type.
 */
typedef struct a429_word_fields
{
    /** @brief Encoded or decoded payload value. */
    a429_payload_u payload;

    /** @brief Source/Destination Identifier value. */
    a429_sdi_t sdi;

    /** @brief Sign/Status Matrix value. */
    a429_ssm_t ssm;

} a429_word_fields_t;

/**
 * @brief Parameters supplied to the ARINC 429 encoding operation.
 */
typedef a429_word_fields_t a429_encode_params_t;

/**
 * @brief Result produced by the ARINC 429 decoding operation.
 */
typedef a429_word_fields_t a429_decode_result_t;

/**
 * @brief Supported ARINC 429 label encoding types.
 */
typedef enum
{
    /** @brief Binary Number Representation encoding. */
    A429_LABEL_BNR = 0,

    /** @brief Binary Coded Decimal encoding. */
    A429_LABEL_BCD = 1,

    /** @brief Discrete data encoding. */
    A429_LABEL_DISC = 2,

    /** @brief Number of supported label encoding types. */
    A429_LABEL_TYPE_SIZE = 3

} a429_label_type_t;

/**
 * @brief Payload location and width within an ARINC 429 word.
 *
 * The @c begin field uses one-based ARINC 429 bit numbering.
 * For example, ARINC bit 11 corresponds to the standard data field start.
 */
typedef struct encode_info
{
    /** @brief One-based ARINC 429 bit position at which the payload begins. */
    uint8_t begin;

    /** @brief Number of bits occupied by the configured payload. */
    uint8_t width;

} a429_payload_info_t;

/**
 * @brief Static dictionary entry describing an ARINC 429 Label.
 *
 * A dictionary entry defines the semantic and encoding properties of a
 * Label. The high-level encode and decode APIs use these entries to select
 * the appropriate codec and interpret the associated payload.
 */
typedef struct
{
    /** @brief ARINC 429 Label value. */
    uint8_t label;

    /** @brief Identifier of the equipment or subsystem associated with the Label. */
    uint16_t equipment_id;

    /** @brief Human-readable name of the Label. */
    const char *name;

    /** @brief Physical or engineering unit associated with the Label. */
    const char *unit;

    /** @brief Encoding type used by the Label. */
    a429_label_type_t ltype;

    /**
     * @brief Location and width of the encoded payload.
     *
     * The location is expressed using one-based ARINC 429 bit numbering.
     * The width determines whether SDI and/or SSM are included in the
     * configured payload representation.
     */
    a429_payload_info_t encoding;

    /**
     * @brief ARINC 429 transmission bit time in microseconds.
     *
     * @note Currently stored as metadata only and not used by liba429.
     */
    uint32_t bit_time;

    /** @brief Scale factor used by the encoding. */
    a429_value_t scale;

    /** @brief Resolution of the encoded value. */
    a429_value_t resolution;

    /** @brief Offset applied to the encoded value. */
    a429_value_t offset;

} a429_label_dictionary_t;

/**
 * @brief Number of possible ARINC 429 Label values.
 */
#define A429_LABEL_COUNT 256U

/**
 * @brief Defines a BNR Label dictionary entry.
 *
 * This macro creates a designated initializer for an
 * @ref a429_dictionary_table_t entry.
 *
 * @param label_ ARINC 429 Label value.
 * @param eqid_ Equipment or subsystem identifier.
 * @param name_ Human-readable Label name.
 * @param unit_ Engineering unit associated with the Label.
 * @param begin_ One-based ARINC 429 bit position of the payload.
 * @param width_ Width of the configured payload in bits.
 * @param bit_time_ ARINC 429 transmission bit time in microseconds.
 * @param resolution_ Resolution of the encoded value.
 * @param scale_ Scale factor used by the encoding.
 * @param offset_ Offset applied to the encoded value.
 */
#define A429_BNR(label_, eqid_, name_, unit_, begin_, width_, bit_time_, resolution_, scale_, offset_) \
    [label_] = &(const a429_label_dictionary_t)                                                        \
    {                                                                                                  \
        .label = (label_),                                                                             \
        .equipment_id = (eqid_),                                                                       \
        .name = (name_),                                                                               \
        .unit = (unit_),                                                                               \
        .ltype = A429_LABEL_BNR,                                                                       \
        .encoding.begin = (begin_),                                                                    \
        .encoding.width = (width_),                                                                    \
        .bit_time = (bit_time_),                                                                       \
        .resolution = (resolution_),                                                                   \
        .scale = (scale_),                                                                             \
        .offset = (offset_)                                                                            \
    }

/**
 * @brief Defines a BCD Label dictionary entry.
 *
 * This macro creates a designated initializer for an
 * @ref a429_dictionary_table_t entry.
 *
 * @param label_ ARINC 429 Label value.
 * @param eqid_ Equipment or subsystem identifier.
 * @param name_ Human-readable Label name.
 * @param unit_ Engineering unit associated with the Label.
 * @param begin_ One-based ARINC 429 bit position of the payload.
 * @param width_ Width of the configured payload in bits.
 * @param bit_time_ ARINC 429 transmission bit time in microseconds.
 * @param resolution_ Resolution of the encoded value.
 * @param scale_ Scale factor used by the encoding.
 * @param offset_ Offset applied to the encoded value.
 */
#define A429_BCD(label_, eqid_, name_, unit_, begin_, width_, bit_time_, resolution_, scale_, offset_) \
    [label_] = &(const a429_label_dictionary_t)                                                        \
    {                                                                                                  \
        .label = (label_),                                                                             \
        .equipment_id = (eqid_),                                                                       \
        .name = (name_),                                                                               \
        .unit = (unit_),                                                                               \
        .ltype = A429_LABEL_BCD,                                                                       \
        .encoding.begin = (begin_),                                                                    \
        .encoding.width = (width_),                                                                    \
        .bit_time = (bit_time_),                                                                       \
        .resolution = (resolution_),                                                                   \
        .scale = (scale_),                                                                             \
        .offset = (offset_)                                                                            \
    }

/**
 * @brief Defines a discrete Label dictionary entry.
 *
 * This macro creates a designated initializer for an
 * @ref a429_dictionary_table_t entry.
 *
 * @param label_ ARINC 429 Label value.
 * @param eqid_ Equipment or subsystem identifier.
 * @param name_ Human-readable Label name.
 * @param unit_ Engineering unit associated with the Label.
 * @param begin_ One-based ARINC 429 bit position of the payload.
 * @param width_ Width of the configured payload in bits.
 * @param bit_time_ ARINC 429 transmission bit time in microseconds.
 * @param resolution_ Resolution of the encoded value.
 * @param scale_ Scale factor used by the encoding.
 * @param offset_ Offset applied to the encoded value.
 */
#define A429_DISC(label_, eqid_, name_, unit_, begin_, width_, bit_time_, resolution_, scale_, offset_) \
    [label_] = &(const a429_label_dictionary_t)                                                         \
    {                                                                                                   \
        .label = (label_),                                                                              \
        .equipment_id = (eqid_),                                                                        \
        .name = (name_),                                                                                \
        .unit = (unit_),                                                                                \
        .ltype = A429_LABEL_DISC,                                                                       \
        .encoding.begin = (begin_),                                                                     \
        .encoding.width = (width_),                                                                     \
        .bit_time = (bit_time_),                                                                        \
        .resolution = (resolution_),                                                                    \
        .scale = (scale_),                                                                              \
        .offset = (offset_)                                                                             \
    }

/**
 * @brief Static dictionary table indexed by ARINC 429 Label.
 *
 * Each array element points to the dictionary entry associated with the
 * corresponding Label value. Unused Label values may remain NULL.
 */
typedef const a429_label_dictionary_t *a429_dictionary_table_t[A429_LABEL_COUNT];

/**
 * @brief Decodes an ARINC 429 word using a dictionary table.
 *
 * The Label is extracted from the word and used to select the corresponding
 * dictionary entry and codec. The decoded payload, SDI, and SSM values are
 * written to @p result.
 *
 * @param[in] word ARINC 429 word in the software representation.
 * @param[out] result Structure receiving the decoded fields.
 * @param[in] table Dictionary table containing the Label definition.
 * @param[out] out_label Extracted ARINC 429 Label value.
 *
 * @return A429_ERR_NO on success; otherwise an appropriate @ref a429_error_t
 *         value describing the failure.
 */
a429_error_t a429_decode_word(a429_word_t word,
                              a429_decode_result_t *result,
                              const a429_dictionary_table_t *table,
                              uint8_t *out_label);

/**
 * @brief Encodes an ARINC 429 word using a dictionary table.
 *
 * The dictionary entry associated with @p label determines the encoding type
 * and payload layout. The supplied payload, SDI, and SSM values are encoded
 * into the resulting ARINC 429 word.
 *
 * @param[in] label ARINC 429 Label to encode.
 * @param[out] out_word Encoded ARINC 429 word in software representation.
 * @param[in] params Payload, SDI, and SSM values to encode.
 * @param[in] table Dictionary table containing the Label definition.
 *
 * @return A429_ERR_NO on success; otherwise an appropriate @ref a429_error_t
 *         value describing the failure.
 */
a429_error_t a429_encode_word(uint8_t label,
                              a429_word_t *out_word,
                              const a429_encode_params_t *params,
                              const a429_dictionary_table_t *table);

/**
 * @brief Converts a wire-level ARINC 429 word to software representation.
 *
 * The conversion reverses the bit order of the 8-bit Label field while
 * leaving the remaining fields unchanged. This converts the representation
 * used by an ARINC 429 interface into the software representation used
 * internally by the library.
 *
 * @param[in] wire_data 32-bit ARINC 429 word in wire-level representation.
 *
 * @return ARINC 429 word in software representation.
 *
 * @note Only the Label field (ARINC 429 bits 1-8) is modified.
 *       All remaining bits are preserved.
 */
a429_word_t a429_unpack_word(a429_wire_data_t wire_data);

/**
 * @brief Converts a software ARINC 429 word to wire-level representation.
 *
 * The conversion reverses the bit order of the 8-bit Label field while
 * leaving the remaining fields unchanged. The resulting value can be passed
 * to an ARINC 429 interface using the representation expected by that
 * interface.
 *
 * @param[in] word ARINC 429 word in software representation.
 *
 * @return ARINC 429 word in wire-level representation.
 *
 * @note Only the Label field (ARINC 429 bits 1-8) is modified.
 *       All remaining bits are preserved.
 */
a429_wire_data_t a429_pack_word(a429_word_t word);

#endif /* LIBA429_H */