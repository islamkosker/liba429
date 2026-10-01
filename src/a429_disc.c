#include "a429_disc.h"
#include "a429_word.h"

#include <limits.h>

uint32_t a429_get_discrete_field(a429_word_t word, uint8_t payload_begin, uint8_t payload_width)
{
    return a429_get_bits(word, payload_begin, payload_width);
}

void a429_set_discrete_field(a429_word_t *word, uint32_t value, uint8_t payload_begin, uint8_t payload_width)
{
    a429_set_bits(word, value, payload_begin, payload_width);
}
