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
    A429_ERR_INVALID_PARITY = 120,
    A429_ERR_DUPLICATE_LABEL = 119,
    A429_ERR_BAD_PARITY = 118,
    A429_ERR_UNKNOWN_LABEL = 117,

} a429_error_t;

typedef union a429_payload
{
    uint32_t discrete;
    a429_value_t value;
} a429_payload_u;

typedef struct a429_word_fields
{
    a429_payload_u payload;
    uint8_t sdi;
    uint8_t ssm;
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

typedef struct
{
    uint8_t bit_offset;
    uint8_t bit_width;
} a429_discrete_field_t;
typedef union encode_info
{
    a429_discrete_field_t discrete;
    uint8_t bit_width;

} a429_encoding_info_t;

typedef struct
{
    uint8_t label;
    uint16_t equipment_id;
    const char *name;
    const char *unit;
    a429_label_type_t ltype;
    a429_encoding_info_t encoding;
    uint32_t min_tx_us;
    uint32_t max_tx_us;
    a429_value_t scale;
    a429_value_t offset;
} a429_label_dictionary_t;

#define A429_LABEL_COUNT 256U

#define A429_BNR(label_, name_, unit_, width_, scale_, offset_) \
    [label_] = &(const a429_label_dictionary_t)                 \
    {                                                           \
        .label = (label_),                                      \
        .name = (name_),                                        \
        .unit = (unit_),                                        \
        .ltype = A429_LABEL_BNR,                                \
        .encoding.bit_width = (width_),                         \
        .scale = (scale_),                                      \
        .offset = (offset_)                                     \
    }

#define A429_BCD(label_, name_, unit_, width_, scale_, offset_) \
    [label_] = &(const a429_label_dictionary_t)                 \
    {                                                           \
        .label = (label_),                                      \
        .name = (name_),                                        \
        .unit = (unit_),                                        \
        .ltype = A429_LABEL_BCD,                                \
        .encoding.bit_width = (width_),                         \
        .scale = (scale_),                                      \
        .offset = (offset_)                                     \
    }

#define A429_DISC(label_, name_, bit_offset_, bit_width_) \
    [label_] = &(const a429_label_dictionary_t)           \
    {                                                     \
        .label = (label_),                                \
        .name = (name_),                                  \
        .ltype = A429_LABEL_DISC,                         \
        .encoding.discrete.bit_offset = (bit_offset_),    \
        .encoding.discrete.bit_width = (bit_width_)       \
    }

typedef const a429_label_dictionary_t *a429_dictionary_table_t[A429_LABEL_COUNT];

a429_error_t a429_decode_word(a429_word_t word, a429_decode_result_t *result,
                              const a429_dictionary_table_t *table);

a429_error_t a429_encode_word(uint8_t label, a429_word_t *out_word, const a429_encode_params_t *params,
                              const a429_dictionary_table_t *table);

#endif // LIBA429_H