#ifndef LIBA429_H
#define LIBA429_H

#include <stdint.h>
#include <stdbool.h>

typedef uint32_t a429_word_t;
typedef uint32_t a429_wire_data_t;
typedef bool a429_discrete_t;
typedef double a429_value_t;

typedef enum
{
    A429_ERR_NO = 0,
    A429_ERR_DECODE = 127,
    A429_ERR_ENCODE = 126,
    A429_ERR_OUT_OF_RANGE = 125,
    A429_ERR_INVALID_BCD = 124,
    A429_ERR_INVALID_BIT = 123,
    A429_ERR_INVALID_ARG = 122,
    A429_ERR_INVALID_LABEL = 121,
    A429_ERR_BAD_PARITY = 120,
    A429_ERR_UNKNOWN_LABEL = 119,

} a429_error_t;

typedef enum
{
    A429_SSM_BNR_FAILURE = 0x00,         // 00: Failure Warning
    A429_SSM_BNR_NO_COMPUTE_DATA = 0x01, // 01: No Computed Data
    A429_SSM_BNR_FUNCTIONAL_TEST = 0x02, // 10: Functional Test
    A429_SSM_BNR_NORMAL = 0x03           // 11: Normal Operation
} a429_ssm_bnr_t;

typedef enum
{
    A429_SSM_BCD_PLUS_NORTH_EAST = 0x00, // 00: Plus, North, East, Right, To, Above
    A429_SSM_BCD_NO_COMPUTE_DATA = 0x01, // 01: No Computed Data
    A429_SSM_BCD_FUNCTIONAL_TEST = 0x02, // 10: Functional Test
    A429_SSM_BCD_MINUS_SOUTH_WEST = 0x03 // 11: Minus, South, West, Left, From, Below
} a429_ssm_bcd_t;

typedef enum
{
    A429_SSM_DISC_NORMAL = 0x00,          // 00: Verified Data, Normal Operation
    A429_SSM_DISC_NO_COMPUTE_DATA = 0x01, // 01: No Computed Data
    A429_SSM_DISC_FUNCTIONAL_TEST = 0x02, // 10: Functional Test
    A429_SSM_DISC_FAILURE = 0x03          // 11: Failure Warning
} a429_ssm_disc_t;

typedef enum
{
    A429_SDI_ALL = 0x00,
    A429_SDI_SYS1 = 0x01,
    A429_SDI_SYS2 = 0x02,
    A429_SDI_SYS3 = 0x03,

} a429_sdi_t;

typedef union a429_ssm
{
    a429_ssm_bnr_t ssm_bnr;
    a429_ssm_bcd_t ssm_bcd;
    a429_ssm_disc_t ssm_disc;
} a429_ssm_t;

typedef union a429_payload
{
    uint32_t discrete;
    a429_value_t value;
} a429_payload_u;

typedef struct a429_word_fields
{
    a429_payload_u payload;
    uint8_t sdi;
    a429_ssm_t ssm;
} a429_word_fields_t;

typedef a429_word_fields_t a429_encode_params_t;
typedef a429_word_fields_t a429_decode_result_t;

typedef enum
{
    A429_LABEL_BNR = 0,
    A429_LABEL_BCD = 1,
    A429_LABEL_DISC = 2,
    A429_LABEL_TYPE_SIZE = 3
} a429_label_type_t;

typedef struct encode_info
{
    uint8_t begin;
    uint8_t width;

} a429_payload_info_t;

typedef struct
{
    uint8_t label;
    uint16_t equipment_id;
    const char *name;
    const char *unit;
    a429_label_type_t ltype;
    a429_payload_info_t encoding;
    uint32_t bit_time;
    a429_value_t scale;
    a429_value_t resolution;
    a429_value_t offset;
} a429_label_dictionary_t;

#define A429_LABEL_COUNT 256U

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

#define A429_DISC(label_, eqid_, name_, unit_, begin_, width_, bit_time_, resolution_, scale_, offset_) \
    [label_] = &(const a429_label_dictionary_t)                                                         \
    {                                                                                                   \
        .label = (label_),                                                                              \
        .equipment_id = (eqid_),                                                                        \
        .name = (name_),                                                                                \
        .unit = (unit_),                                                                                \
        .ltype = A429_LABEL_BNR,                                                                        \
        .encoding.begin = (begin_),                                                                     \
        .encoding.width = (width_),                                                                     \
        .bit_time = (bit_time_),                                                                        \
        .resolution = (resolution_),                                                                    \
        .scale = (scale_),                                                                              \
        .offset = (offset_)                                                                             \
    }

typedef const a429_label_dictionary_t *a429_dictionary_table_t[A429_LABEL_COUNT];

a429_error_t a429_decode_word(a429_word_t word, a429_decode_result_t *result,
                              const a429_dictionary_table_t *table);

a429_error_t a429_encode_word(uint8_t label, a429_word_t *out_word, const a429_encode_params_t *params,
                              const a429_dictionary_table_t *table);

#endif /*LIBA429_H*/