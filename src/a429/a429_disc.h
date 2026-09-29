#ifndef A429_DISCRETE_H
#define A429_DISCRETE_H

#include <stdbool.h>
#include <a429_types.h>

uint32_t a429_get_discrete_field(uint32_t word, uint8_t payload_begin, uint8_t payload_width);

void a429_set_discrete_field(uint32_t *word, uint32_t value, uint8_t payload_begin, uint8_t payload_width);

#endif // A429_DISCRETE_H