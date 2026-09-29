#include "a429_bcd.h"
#include "a429_word.h"

#include <math.h>
#define NIBBLE 4U

double a429_decode_bcd(a429_word_t word, uint8_t payload_begin, uint8_t payload_width,
                       double resolution, a429_error_t *error_code)
{
    if (payload_width == 0 || payload_width > A429_MAX_PAYLOAD_WIDTH)
    {
        *error_code = A429_ERR_DECODE;
        return 0.0;
    }
    if (resolution <= 0.0)
    {
        *error_code = A429_ERR_INVALID_ARG;
        return 0.0;
    }

    *error_code = A429_ERR_NO;

    uint32_t bnr_data = a429_get_bits(word, payload_begin, payload_width);

    const uint8_t full_digits = payload_width / 4U;
    const uint8_t remaining_bits = payload_width % 4U;

    uint8_t digit_count = full_digits;

    if (remaining_bits != 0)
        digit_count++;

    double value = 0.0;
    double multiplier = 1.0;

    for (uint8_t i = 0; i < digit_count; i++)
    {
        uint8_t digit_width = NIBBLE;
        if ((i == (digit_count - 1U)) && (remaining_bits != 0U))
        {
            digit_width = remaining_bits;
        }

        const uint32_t mask = (UINT32_C(1) << digit_width) - UINT32_C(1);
        const uint8_t digit = (uint8_t)(bnr_data & mask);

        if (digit > 9)
        {
            *error_code = A429_ERR_INVALID_BCD;

            return 0.0;
        }

        value += (double)digit * multiplier;
        multiplier *= 10.0;
        bnr_data >>= digit_width;
    }

    return value * resolution;
}

void a429_encode_bcd(a429_word_t *word, double value, uint8_t payload_begin,
                     uint8_t payload_width, double resolution, a429_error_t *error_code)
{
    if ((payload_width == 0U) ||
        (payload_width > A429_MAX_PAYLOAD_WIDTH))
    {
        *error_code = A429_ERR_ENCODE;
        return;
    }

    if (resolution <= 0.0)
    {
        *error_code = A429_ERR_INVALID_ARG;
        return;
    }

    const double resolutiond_value = round(fabs(value) / resolution);

    uint32_t raw_value = (uint32_t)resolutiond_value;

    const uint8_t full_digits = payload_width / 4U;
    const uint8_t remaining_bits = payload_width % 4U;

    uint8_t digit_count = full_digits;

    if (remaining_bits != 0U)
    {
        digit_count++;
    }

    uint32_t result = 0U;
    uint8_t shift = 0U;

    for (uint8_t i = 0U; i < digit_count; i++)
    {
        uint8_t digit_width = 4U;

        if ((i == (digit_count - 1U)) &&
            (remaining_bits != 0U))
        {
            digit_width = remaining_bits;
        }

        const uint32_t digit = raw_value % UINT32_C(10);
        const uint32_t max_digit = (UINT32_C(1) << digit_width) - UINT32_C(1);

        if ((digit > 9U) || (digit > max_digit))
        {
            *error_code = A429_ERR_OUT_OF_RANGE;
            return;
        }

        result |= digit << shift;

        raw_value /= UINT32_C(10);
        shift += digit_width;
    }

    if (raw_value != 0U)
    {
        *error_code = A429_ERR_OUT_OF_RANGE;
        return;
    }

    *error_code = A429_ERR_NO;

    a429_set_bits(word, result, payload_begin, payload_width);
}