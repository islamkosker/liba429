#ifndef A429_CODEC_H
#define A429_CODEC_H

#include "liba429.h"

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

extern const a429_codec_t a429_codec[A429_LABEL_TYPE_SIZE];

#endif // A429_CODEC_H
