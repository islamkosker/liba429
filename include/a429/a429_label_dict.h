#ifndef A429_LABEL_DICT_H
#define A429_LABEL_DICT_H

#include <stdint.h>

typedef enum
{
    A429_LABEL_BNR = 0,
    A429_LABEL_BCD = 1,
    A429_LABEL_DISC = 2,
    A429_LABEL_TYPE_SIZE = 3
} a429_label_type_t;

typedef struct
{
    uint8_t bit_offset;
    uint8_t bit_width;
} a429_discrete_field_t;
typedef union encode_info
{
    a429_discrete_field_t discrete;
    uint8_t bit_width;

} encode_info_t;

typedef struct
{
    uint8_t label;
    uint16_t equipment_id;
    const char *name;
    const char *unit;
    a429_label_type_t ltype;
    encode_info_t encoding;
    uint32_t min_tx_us;
    uint32_t max_tx_us;
    double scale;
    double offset;
} a429_label_dictionary_t;

#endif // A429_LABEL_DICT_H