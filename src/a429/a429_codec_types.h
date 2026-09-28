#ifndef A429_PAYLOAD_H
#define A429_PAYLOAD_H

#include "liba429.h"
#include "a429_types.h"
#include "a429_label_dict.h"
#include "a429_types.h"
#include "a429_error.h"

typedef struct a429_codec
{
    a429_error_t (*encode)(
        a429_word_t *word,
        const a429_encode_params_t *params,
        const a429_label_dictionary_t *dict);

    a429_error_t (*decode)(
        a429_word_t word,
        a429_decode_result_t *result,
        const a429_label_dictionary_t *dict);

} a429_codec_t;

#endif // A429_PAYLOAD_H
