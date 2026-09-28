#include "a429_protocol.h"
#include "a429_types.h"
#include "a429_codec_types.h"
#include "a429_parity.h"
#include "a429_word.h"
#include "a429_codec.h"

a429_error_t a429_decode_word(a429_word_t word, a429_decode_result_t *result,
                              const a429_dictionary_table_t *table)
{
    if (!a429_verify_parity(word))
    {
        return A429_ERR_BAD_PARITY;
    }
    uint8_t label = a429_get_label(word);
    const a429_label_dictionary_t *dict = (*table)[label];

    if (!dict)
        return A429_ERR_UNKNOWN_LABEL;

    return a429_codec[dict->ltype].decode(word, result, dict);
}

a429_error_t a429_encode_word(uint8_t label, a429_word_t *out_word, const a429_encode_params_t *params,
                              const a429_dictionary_table_t *table)
{

    const a429_label_dictionary_t *dict = (*table)[label];
    if (!dict)
        return A429_ERR_UNKNOWN_LABEL;
    return a429_codec[dict->ltype].encode(out_word, params, dict);
}