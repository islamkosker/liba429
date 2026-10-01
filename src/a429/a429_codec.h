#ifndef A429_CODEC_H
#define A429_CODEC_H

#include "liba429.h"

/**
 * @brief Interface for encoding and decoding an ARINC 429 label.
 *
 * Defines the codec operations used by the protocol layer to encode
 * and decode payload data according to a label dictionary entry.
 *
 * Each supported label type provides an implementation of this
 * interface. The codec implementations are selected through the
 * codec dispatch table indexed by @ref a429_label_type_t.
 */
typedef struct a429_codec
{
    /**
     * @brief Encode label data into an ARINC 429 word.
     *
     * Encodes the payload described by the supplied parameters and
     * dictionary entry into the specified ARINC 429 word.
     *
     * @param[out] word ARINC 429 word to be updated with the encoded payload.
     * @param[in] params Encoding parameters containing the payload value.
     * @param[in] dict Dictionary entry defining the label encoding.
     *
     * @return @ref A429_ERR_NONE on success, otherwise an appropriate
     *         @ref a429_error_t error code.
     */
    a429_error_t (*encode)(a429_word_t *word, const a429_encode_params_t *params, const a429_label_dictionary_t *dict);

    /**
     * @brief Decode label data from an ARINC 429 word.
     *
     * Decodes the payload of an ARINC 429 word according to the
     * encoding configuration specified by the dictionary entry.
     *
     * @param[in] word ARINC 429 word containing the encoded payload.
     * @param[out] result Structure receiving the decoded payload value.
     * @param[in] dict Dictionary entry defining the label encoding.
     *
     * @return @ref A429_ERR_NONE on success, otherwise an appropriate
     *         @ref a429_error_t error code.
     */
    a429_error_t (*decode)(a429_word_t word, a429_decode_result_t *result, const a429_label_dictionary_t *dict);

} a429_codec_t;

/**
 * @brief Codec dispatch table indexed by label type.
 *
 * Each entry provides the encode and decode operations associated
 * with an @ref a429_label_type_t.
 *
 * The protocol layer uses this table to select the appropriate
 * codec implementation without requiring label-type-specific
 * branching in the protocol workflow.
 */
extern const a429_codec_t a429_codec[A429_LABEL_TYPE_SIZE];

#endif // A429_CODEC_H
