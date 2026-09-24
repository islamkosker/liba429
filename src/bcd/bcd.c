#include "a429/bcd.h"
#include "a429/word.h"
#include <math.h>

#define LAST_NIBBLE_IDX 4
#define MAX_DIGIT (LAST_NIBBLE_IDX + 1)
#define _3BIT 0x07
#define _4_BIT 0x0F
#define MAX_BCD 79999U

static uint8_t get_mask(const uint8_t idx)
{

    return (idx == LAST_NIBBLE_IDX) ? _3BIT : _4_BIT;
}

double a429_decode_bcd(const a429_word_t word, const uint8_t digit_count,
                       const double resolution, int8_t *error_code)
{
    if (digit_count == 0 || digit_count > MAX_DIGIT)
    {
        if (error_code)
            *error_code = -A429_ERR_DECODE;
        return 0.0;
    }
    *error_code = A429_ERR_NO;

    uint32_t data = a429_get_data(word);
    double value = 0.0;
    double multiplier = 1.0;

    for (uint8_t i = 0; i < digit_count; i++)
    {
        uint8_t mask = get_mask(i);
        uint8_t digit = data & mask;

        if (digit > 9)
        {
            if (error_code)
                *error_code = -A429_ERR_INVALID_BCD;
            return 0.0;
        }

        value += (double)digit * multiplier;
        multiplier *= 10.0;

        data >>= 4;
    }

    return value * resolution;
}

void a429_encode_bcd(a429_word_t *word, const double value, const uint8_t digit_count,
                     const double resolution, int8_t *error_code)
{
    if (digit_count == 0 || digit_count > MAX_DIGIT)
    {
        *error_code = -A429_ERR_ENCODE;
        return;
    }

    *error_code = A429_ERR_NO;
    uint32_t raw_value = (uint32_t)round(fabs(value) / resolution);
    uint8_t shift = 0;
    uint32_t result = 0;

    if (raw_value > MAX_BCD)
    {
        *error_code = -A429_ERR_OUT_OF_RANGE;
        return;
    }

    while (raw_value > 0)
    {
        uint8_t digit = (raw_value % 10);

        result |= digit << shift;
        raw_value /= 10;
        shift += 4;
    }
    a429_set_data(word, result);
}