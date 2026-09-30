
#include "a429_word.h"
#include "a429_bnr.h"
#include "a429_bcd.h"
#include "a429_disc.h"
#include "a429_parity.h"

#include "a429_codec.h"
#define A429_SSM_START_BIT 30
static a429_error_t a429_bnr_decode_wrapper(a429_word_t word, a429_decode_result_t *result,
                                            const a429_label_dictionary_t *dict)
{
    if (!result || !dict)
        return A429_ERR_INVALID_ARG;

    if (a429_get_label(word) != dict->label)
        return A429_ERR_INVALID_LABEL;

    a429_error_t err = A429_ERR_NO;

    a429_value_t value = a429_decode_bnr(word, dict->encoding.begin, dict->encoding.width, dict->scale, &err);
    if (err != A429_ERR_NO)
        return err;

    result->payload.value = value + dict->offset;

    if ((dict->encoding.begin + dict->encoding.width - 1U) < A429_SSM_START_BIT)
        result->ssm.ssm_bnr = a429_get_ssm(word);
    else
        result->ssm.ssm_bnr = A429_SSM_BNR_NOT_USE;

    if (dict->encoding.begin != A429_SDI_BEGIN)
        result->sdi = a429_get_sdi(word);
    else
        result->sdi = A429_SDI_NOT_USE;

    return A429_ERR_NO;
}

static a429_error_t a429_bnr_encode_wrapper(a429_word_t *word, const a429_encode_params_t *params,
                                            const a429_label_dictionary_t *dict)
{
    if (!word || !params || !dict)
        return A429_ERR_INVALID_ARG;

    a429_word_t temp_word = 0;
    a429_error_t e = A429_ERR_NO;
    a429_encode_bnr(&temp_word, params->payload.value, dict->encoding.begin, dict->encoding.width, dict->scale, &e);
    if (e != A429_ERR_NO)
        return e;

    if ((dict->encoding.begin + dict->encoding.width - 1U) < A429_SSM_START_BIT)

        a429_set_ssm(&temp_word, params->ssm.ssm_bnr);

    if (dict->encoding.begin != A429_SDI_BEGIN)

        a429_set_sdi(&temp_word, params->sdi);

    a429_set_label(&temp_word, dict->label);
    *word = a429_apply_parity(temp_word);
    return A429_ERR_NO;
}

static a429_error_t a429_bcd_decode_wrapper(a429_word_t word, a429_decode_result_t *result,
                                            const a429_label_dictionary_t *dict)
{
    if (!result || !dict)
        return A429_ERR_INVALID_ARG;

    if (a429_get_label(word) != dict->label)
        return A429_ERR_INVALID_LABEL;

    a429_error_t err = A429_ERR_NO;

    a429_value_t value = a429_decode_bcd(word, dict->encoding.begin, dict->encoding.width, dict->resolution, &err);
    if (err != A429_ERR_NO)
        return err;
    result->payload.value = value + dict->offset;

    if ((dict->encoding.begin + dict->encoding.width - 1U) < A429_SSM_START_BIT)
        result->ssm.ssm_bcd = a429_get_ssm(word); // using ssm
    else
        result->ssm.ssm_bcd = A429_SSM_BCD_NOT_USE;

    if (dict->encoding.begin != A429_SDI_BEGIN)
        result->sdi = a429_get_sdi(word);
    else
        result->sdi = A429_SDI_NOT_USE;

    return A429_ERR_NO;
}

static a429_error_t a429_bcd_encode_wrapper(a429_word_t *word, const a429_encode_params_t *params,
                                            const a429_label_dictionary_t *dict)
{
    if (!word || !params || !dict)
        return A429_ERR_INVALID_ARG;

    a429_word_t temp_word = 0;
    a429_error_t e = A429_ERR_NO;
    a429_encode_bcd(&temp_word, params->payload.value, dict->encoding.begin, dict->encoding.width, dict->resolution, &e);
    if (e != A429_ERR_NO)
        return e;

    a429_set_label(&temp_word, dict->label);
    if ((dict->encoding.begin + dict->encoding.width - 1U) < A429_SSM_START_BIT)
        a429_set_ssm(&temp_word, params->ssm.ssm_bcd);

    if (dict->encoding.begin != A429_SDI_BEGIN)
        a429_set_sdi(&temp_word, params->sdi);

    *word = a429_apply_parity(temp_word);

    return A429_ERR_NO;
}

#include <stdio.h>

static a429_error_t a429_disc_decode_wrapper(a429_word_t word, a429_decode_result_t *result,
                                             const a429_label_dictionary_t *dict)
{
    if (!result || !dict)
        return A429_ERR_INVALID_ARG;

    if (a429_get_label(word) != dict->label)
        return A429_ERR_INVALID_LABEL;

    a429_error_t err = A429_ERR_NO;

    uint32_t state = a429_get_discrete_field(word, dict->encoding.begin, dict->encoding.width);

    result->payload.discrete = state;

    if ((dict->encoding.begin + dict->encoding.width - 1U) < A429_SSM_START_BIT)
        result->ssm.ssm_disc = a429_get_ssm(word);
    else
        result->ssm.ssm_disc = A429_SSM_DISC_NOT_USE;

    if (dict->encoding.begin != A429_SDI_BEGIN)
        result->sdi = a429_get_sdi(word);
    else
        result->sdi = A429_SDI_NOT_USE;

    return err;
}

static a429_error_t a429_disc_encode_wrapper(a429_word_t *word, const a429_encode_params_t *params,
                                             const a429_label_dictionary_t *dict)
{
    if (!word || !params || !dict)
        return A429_ERR_INVALID_ARG;
    a429_word_t temp_word = 0;
    a429_set_discrete_field(&temp_word, params->payload.discrete, dict->encoding.begin, dict->encoding.width);
    a429_set_label(&temp_word, dict->label);
    if ((dict->encoding.begin + dict->encoding.width - 1U) < A429_SSM_START_BIT)
        a429_set_ssm(&temp_word, params->ssm.ssm_disc);

    if (dict->encoding.begin != A429_SDI_BEGIN)
        a429_set_sdi(&temp_word, params->sdi);
    *word = a429_apply_parity(temp_word);
    return A429_ERR_NO;
}

const a429_codec_t a429_codec[A429_LABEL_TYPE_SIZE] = {
    [A429_LABEL_BNR] = {.decode = a429_bnr_decode_wrapper, .encode = a429_bnr_encode_wrapper},
    [A429_LABEL_BCD] = {.decode = a429_bcd_decode_wrapper, .encode = a429_bcd_encode_wrapper},
    [A429_LABEL_DISC] = {.decode = a429_disc_decode_wrapper, .encode = a429_disc_encode_wrapper}

};
