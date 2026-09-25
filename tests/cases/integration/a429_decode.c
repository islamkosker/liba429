#include "a429_codec.h"
#include "a429_word.h"

static a429_error_t a429_bnr_decode_wrapper(const a429_word_t *word, a429_decode_result_t *result, const a429_label_dictionary_t *dict)
{

    if (a429_get_label(*word) != dict->label)
        return A429_ERR_INVALID_LABEL;

    if (!a429_verify_parity(*word))
        return A429_ERR_INVALID_PARITY;

    a429_error_t err = A429_ERR_NO;

    a429_value_t value = a429_decode_bnr(*word, dict->bit_width, dict->scale, &err);
    if (err != A429_ERR_NO)
        return err;

    result->payload.value = value + dict->offset;
    result->sdi = a429_get_sdi(*word);
    result->ssm = a429_get_ssm(*word);

    return A429_ERR_NO;
}

static a429_error_t a429_bnr_encode_wrapper(a429_word_t *word, a429_encode_params_t *params, const a429_label_dictionary_t *dict)
{
    if (!word || !params || !dict)
        return A429_ERR_INVALID_ARG;

    uint32_t temp_word = 0;
    a429_error_t e = A429_ERR_NO;
    a429_encode_bnr(&temp_word, params->payload.value, dict->bit_width, dict->scale, &e);
    if (e != A429_ERR_NO)
        return e;
    a429_set_label(&temp_word, dict->label);
    a429_set_sdi(&temp_word, params->sdi);
    a429_set_ssm(&temp_word, params->ssm);
    *word = a429_apply_parity(temp_word);
    return A429_ERR_NO;
}

a429_codec_t a429_codec[A429_LABEL_TYPE_SIZE] = {
    [A429_LABEL_BNR] = {.decode = a429_bnr_decode_wrapper, .encode = a429_bnr_encode_wrapper}

};
