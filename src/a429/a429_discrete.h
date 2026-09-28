#ifndef A429_DISCRETE_H
#define A429_DISCRETE_H

#include <stdbool.h>
#include <a429_types.h>

a429_discrete_t a429_get_discrete_bit(a429_word_t word, uint8_t bit_position);

void a429_set_discrete_bit(a429_word_t *word, uint8_t bit_position, a429_discrete_t state);

uint32_t a429_get_discrete_field(uint32_t word, uint8_t bit_offset, uint8_t bit_width);

void a429_set_discrete_field(uint32_t *word, uint8_t bit_offset, uint8_t bit_width, uint32_t value);

#endif // A429_DISCRETE_H