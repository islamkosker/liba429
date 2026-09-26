#ifndef A429_PROTOCOL_H
#define A429_PROTOCOL_H

#include "a429_label_dict.h"
#include "a429_error.h"

#define A429_LABEL_COUNT 256U

#define A429_BNR(label_, name_, unit_, width_, scale_, offset_) \
    [label_] = &(const a429_label_dictionary_t)                 \
    {                                                           \
        .label = (label_),                                      \
        .name = (name_),                                        \
        .unit = (unit_),                                        \
        .ltype = A429_LABEL_BNR,                                \
        .encoding.bit_width = (width_),                         \
        .scale = (scale_),                                      \
        .offset = (offset_)                                     \
    }

#define A429_BCD(label_, name_, unit_, width_, scale_, offset_) \
    [label_] = &(const a429_label_dictionary_t)                 \
    {                                                           \
        .label = (label_),                                      \
        .name = (name_),                                        \
        .unit = (unit_),                                        \
        .ltype = A429_LABEL_BCD,                                \
        .encoding.bit_width = (width_),                         \
        .scale = (scale_),                                      \
        .offset = (offset_)                                     \
    }

#define A429_DISC(label_, name_, bit_offset_, bit_width_) \
    [label_] = &(const a429_label_dictionary_t)           \
    {                                                     \
        .label = (label_),                                \
        .name = (name_),                                  \
        .ltype = A429_LABEL_DISC,                         \
        .encoding.discrete.bit_offset = (bit_offset_),    \
        .encoding.discrete.bit_width = (bit_width_)       \
    }

typedef const a429_label_dictionary_t *a429_dictionary_table_t[A429_LABEL_COUNT];

// const a429_dictionary_table_t  table = {
//     A429_BNR(001, "Altitude", "ft", 16, 1.0, 0.0),
//     A429_BCD(027, "Heading", "deg", 16, 0.1, 0.0),
//     A429_DISC(203, "Status", 10, 3),
//     A429_BNR(255, "Speed", "kt", 16, 0.5, 0.0)};

// a429_label_dictionary_t *dict = table[1];
// a429_label_dictionary_t *dict1;

#endif // A429_PROTOCOL_H