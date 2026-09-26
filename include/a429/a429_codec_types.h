#ifndef A429_PAYLOAD_H
#define A429_PAYLOAD_H

#include "a429_types.h"
#include "a429_label_dict.h"
#include "a429_types.h"
#include "a429_error.h"

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

typedef struct a429_codec
{
    a429_error_t (*encode)(
        a429_word_t *word,
        const a429_encode_params_t *params,
        const a429_label_dictionary_t *dict);

    a429_error_t (*decode)(
        const a429_word_t *word,
        a429_decode_result_t *result,
        const a429_label_dictionary_t *dict);

} a429_codec_t;

#endif // A429_PAYLOAD_H
