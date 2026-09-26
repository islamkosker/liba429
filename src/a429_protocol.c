#include "a429_protocol.h"
#include "a429_types.h"

const a429_label_dictionary_t *a429_protocol_find_label(const a429_dictionary_table_t *table, uint8_t label)
{
    if (table == NULL)
    {
        return NULL;
    }

    return table[label];
}