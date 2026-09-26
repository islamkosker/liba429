#include "a429/a429_discrete.h"
#include <a429/a429_word.h>

#include <stdint.h>
#include <limits.h>

a429_discrete_t a429_get_discrete_bit(a429_word_t word, uint8_t bit_position)
{
    a429_word_t data = a429_get_data(word);
    return (data & (1U << bit_position)) != 0;
}

void a429_set_discrete_bit(a429_word_t *word, uint8_t bit_position, a429_discrete_t state)
{
    uint32_t data = 0;
    data = (data & ~(1U << bit_position)) | ((state & 1U) << bit_position);
    a429_set_data(word, data);
}

uint32_t a429_get_discrete_field(a429_word_t word, uint8_t bit_offset, uint8_t bit_width)
{
    a429_word_t data = a429_get_data(word);
    return (data >> bit_offset) & ((1U << bit_width) - 1U);
}

void a429_set_discrete_field(a429_word_t *word, uint8_t bit_offset, uint8_t bit_width, uint32_t value)
{

    a429_word_t data = 0;

    uint32_t mask = ((1U << bit_width) - 1U) << bit_offset;
    data &= ~mask;
    data |= (value << bit_offset) & mask;

    a429_set_data(word, data);
}
