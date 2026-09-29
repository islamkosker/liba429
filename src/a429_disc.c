#include "a429_disc.h"
#include "a429_word.h"

#include <stdint.h>
#include <limits.h>

uint32_t a429_get_discrete_field(a429_word_t word, uint8_t payload_begin, uint8_t payload_width)
{
    a429_word_t bnr_value = a429_get_bits(word, payload_begin, payload_width);
    return (bnr_value >> payload_begin) & ((1U << payload_width) - 1U);
}

void a429_set_discrete_field(a429_word_t *word, uint32_t value, uint8_t payload_begin, uint8_t payload_width)
{

    a429_word_t payload = 0;

    uint32_t mask = ((1U << payload_width) - 1U) << payload_begin;
    payload &= ~mask;
    payload |= (value << payload_begin) & mask;

    a429_set_bits(word, payload, payload_begin, payload_width);
}
