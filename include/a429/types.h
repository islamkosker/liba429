#ifndef A429_TYPES
#define A429_TYPES

#include <stdbool.h>
#include <stdint.h>

typedef uint32_t a429_word_t;
typedef bool a429_discrete_t;

typedef enum
{
    A429_ERR_NO = 0,
    A429_ERR_DECODE = 127,
    A429_ERR_ENCODE = 126,
    A429_ERR_OUT_OF_RANGE = 125,
    A429_ERR_INVALID_BCD = 124,
    A429_ERR_INVALID_BIT = 123
} a429_error_t;

#endif