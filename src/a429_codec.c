
#include "a429_word.h"
#include "a429_bnr.h"
#include "a429_bcd.h"
#include "a429_discrete.h"
#include "a429_parity.h"

#include "a429_codec.h"

static inline a429_error_t check_decode_input(const a429_word_t *word, const a429_label_dictionary_t *dict)
{

    if (a429_get_label(*word) != dict->label)
        return A429_ERR_INVALID_LABEL;

    if (!a429_verify_parity(*word))
        return A429_ERR_INVALID_PARITY;
    return A429_ERR_NO;
}

static inline void disassemble_word(const a429_word_t *word, a429_decode_result_t *result,
                                    const a429_label_dictionary_t *dict, a429_value_t value)
{
    result->payload.value = value + dict->offset;
    result->sdi = a429_get_sdi(*word);
    result->ssm = a429_get_ssm(*word);
}

static inline a429_word_t assemble_word(a429_word_t temp_word, const a429_encode_params_t *params,
                                        const a429_label_dictionary_t *dict)
{
    a429_set_label(&temp_word, dict->label);
    a429_set_sdi(&temp_word, params->sdi);
    a429_set_ssm(&temp_word, params->ssm);
    return a429_apply_parity(temp_word);
}

static a429_error_t a429_bnr_decode_wrapper(const a429_word_t *word, a429_decode_result_t *result,
                                            const a429_label_dictionary_t *dict)
{
    if (!word || !result || !dict)
        return A429_ERR_INVALID_ARG;

    a429_error_t err = check_decode_input(word, dict);
    if (err != A429_ERR_NO)
        return err;

    a429_value_t value = a429_decode_bnr(*word, dict->encoding.bit_width, dict->scale, &err);
    if (err != A429_ERR_NO)
        return err;

    disassemble_word(word, result, dict, value);

    return A429_ERR_NO;
}

static a429_error_t a429_bnr_encode_wrapper(a429_word_t *word, const a429_encode_params_t *params,
                                            const a429_label_dictionary_t *dict)
{
    if (!word || !params || !dict)
        return A429_ERR_INVALID_ARG;

    a429_word_t temp_word = 0;
    a429_error_t e = A429_ERR_NO;
    a429_encode_bnr(&temp_word, params->payload.value, dict->encoding.bit_width, dict->scale, &e);
    if (e != A429_ERR_NO)
        return e;

    *word = assemble_word(temp_word, params, dict);
    return A429_ERR_NO;
}

static a429_error_t a429_bcd_decode_wrapper(const a429_word_t *word, a429_decode_result_t *result,
                                            const a429_label_dictionary_t *dict)
{
    if (!word || !result || !dict)
        return A429_ERR_INVALID_ARG;
    a429_error_t err = check_decode_input(word, dict);
    if (err != A429_ERR_NO)
        return err;

    a429_value_t value = a429_decode_bcd(*word, dict->encoding.bit_width, dict->scale, &err);
    if (err != A429_ERR_NO)
        return err;
    disassemble_word(word, result, dict, value);

    return A429_ERR_NO;
}

static a429_error_t a429_bcd_encode_wrapper(a429_word_t *word, const a429_encode_params_t *params,
                                            const a429_label_dictionary_t *dict)
{
    if (!word || !params || !dict)
        return A429_ERR_INVALID_ARG;

    a429_word_t temp_word = 0;
    a429_error_t e = A429_ERR_NO;
    a429_encode_bcd(&temp_word, params->payload.value, dict->encoding.bit_width, dict->scale, &e);
    if (e != A429_ERR_NO)
        return e;

    *word = assemble_word(temp_word, params, dict);
    return A429_ERR_NO;
}

static a429_error_t a429_disc_decode_wrapper(const a429_word_t *word, a429_decode_result_t *result,
                                             const a429_label_dictionary_t *dict)
{
    if (!word || !result || !dict)
        return A429_ERR_INVALID_ARG;
    a429_error_t err = check_decode_input(word, dict);
    if (err != A429_ERR_NO)
        return err;

    uint32_t state = a429_get_discrete_field(*word, dict->encoding.discrete.bit_offset, dict->encoding.discrete.bit_width);

    // disassemble word
    result->payload.discrete = state;
    result->sdi = a429_get_sdi(*word);
    result->ssm = a429_get_ssm(*word);
    return A429_ERR_NO;
}

static a429_error_t a429_disc_encode_wrapper(a429_word_t *word, const a429_encode_params_t *params,
                                             const a429_label_dictionary_t *dict)
{
    if (!word || !params || !dict)
        return A429_ERR_INVALID_ARG;
    a429_word_t temp_word = 0;
    a429_set_discrete_field(&temp_word, dict->encoding.discrete.bit_offset, dict->encoding.discrete.bit_width, params->payload.discrete);
    *word = assemble_word(temp_word, params, dict);
    return A429_ERR_NO;
}

const a429_codec_t a429_codec[A429_LABEL_TYPE_SIZE] = {
    [A429_LABEL_BNR] = {.decode = a429_bnr_decode_wrapper, .encode = a429_bnr_encode_wrapper},
    [A429_LABEL_BCD] = {.decode = a429_bcd_decode_wrapper, .encode = a429_bcd_encode_wrapper},
    [A429_LABEL_DISC] = {.decode = a429_disc_decode_wrapper, .encode = a429_disc_encode_wrapper}

};
