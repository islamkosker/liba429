#include "liba429.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define EXAMPLE_SCALE 79999
#define EXAMPLE_OFFSET 0.0
#define EXAMPLE_RESOLUTION 0.1
#define EXAMPLE_BIT_TIME 100

#define LABEL_BCD 10
#define LABEL_BNR 100
#define LABEL_DISC 200

#define GET_TABLE_ELEM(table, idx) (*(table)[idx])

static const a429_dictionary_table_t a429_example_table = {
    A429_BCD(LABEL_BCD, LABEL_BCD, "FUEL_QUANTITY", "lb", A429_DATA_BEGIN, A429_DEFAULT_DATA_WIDTH, EXAMPLE_BIT_TIME, EXAMPLE_RESOLUTION, EXAMPLE_SCALE, EXAMPLE_OFFSET),
    A429_BNR(LABEL_BNR, LABEL_BNR, "VERTICAL_SPEED", "ft/min", A429_DATA_BEGIN, A429_DEFAULT_DATA_WIDTH, EXAMPLE_BIT_TIME, EXAMPLE_RESOLUTION, EXAMPLE_SCALE, EXAMPLE_OFFSET),
    A429_DISC(LABEL_DISC, LABEL_DISC, "LANDING_GEAR_STATUS", "state", A429_DATA_BEGIN, A429_DEFAULT_DATA_WIDTH, EXAMPLE_BIT_TIME, EXAMPLE_RESOLUTION, EXAMPLE_SCALE, EXAMPLE_OFFSET),

};

static unsigned long long large_rand(void)
{
    unsigned long long r = 0;
    for (int i = 0; i < 5; i++)
    {
        r = (r << 15) | (rand() & RAND_MAX);
    }
    return r;
}

static inline double random_value_bcd(uint8_t width, double resolution)
{
    uint8_t full_digits = width / 4;
    uint8_t remaining_bits = width % 4;

    uint8_t digit_count = full_digits + ((remaining_bits != 0U) ? 1U : 0U);

    uint32_t max_bcd_int = 0U;
    uint32_t multiplier = 1U;

    for (uint8_t i = 0; i < digit_count; i++)
    {
        uint8_t digit_width = 4U;

        if ((i == (digit_count - 1U)) && remaining_bits != 0U)
        {
            digit_width = remaining_bits;
        }
        uint32_t max_digit = (UINT32_C(1) << digit_width) - UINT32_C(1);
        if (max_digit > 9U)
        {
            max_digit = 9U;
        }
        max_bcd_int += max_digit * multiplier;

        multiplier *= 10U;
    }
    uint32_t raw_bcd_int = (uint32_t)(large_rand() % (max_bcd_int + 1U));

    return (double)raw_bcd_int * resolution;
}

static inline double random_value_bnr(uint8_t width, double scale)
{
    double resolution = scale / (double)(UINT32_C(1) << width);
    uint32_t max_steps = (UINT32_C(1) << width) - 1;
    uint32_t random_step = (uint32_t)rand() % (max_steps + 1U);
    return (double)random_step * resolution;
}

static inline uint32_t random_value_disc(uint8_t width)
{

    uint32_t disc_word = 0;
    for (size_t i = 0; i < width; i++)
    {
        disc_word |= ((uint32_t)(rand() % 2) << i);
    }
    return disc_word;
}

static a429_wire_data_t sim_wire_in(a429_word_t in)
{
    return a429_pack_word(in);
}

static a429_word_t sim_wire_out(a429_wire_data_t out)
{
    return a429_unpack_word(out);
}

static void a429_basic_example(a429_encode_params_t *param, uint8_t label)
{
    a429_label_dictionary_t dict = GET_TABLE_ELEM(a429_example_table, label);
    a429_word_t word = 0;
    a429_error_t e = a429_encode_word(label, &word, param, &a429_example_table);
    if (e != A429_ERR_NO)
    {
        printf("%s label Encoding error %d\n", dict.name, e);
        return;
    }
    printf("Encoding successful [%s LABEL: %d]\n", dict.name, label);
    a429_wire_data_t wire_data = sim_wire_in(word);

    a429_word_t word_in = sim_wire_out(wire_data);
    a429_decode_result_t same_result = {0};
    uint8_t out_label = 0;
    e = a429_decode_word(word_in, &same_result, &a429_example_table, &out_label);

    if (e != A429_ERR_NO)
    {
        printf("%s label Decoding error %d\n", dict.name, e);
        return;
    }
    printf("Decoding successful [%s LABEL: %d]\n", dict.name, label);

    if (label != LABEL_DISC)
        printf("[%s label] encoeding value %f  decodeing value %f [%s] \n", dict.name, param->payload.value, same_result.payload.value, dict.unit);
    else
        printf("encoeding value %d decodeing value %d \n", param->payload.discrete, same_result.payload.discrete);

    printf("--------------------------------\n\n");
}

int main(void)
{
    srand((unsigned)time(NULL));

    a429_encode_params_t bcd_params = {
        .payload.value = random_value_bcd(GET_TABLE_ELEM(a429_example_table, LABEL_BCD).encoding.width,
                                          GET_TABLE_ELEM(a429_example_table, LABEL_BCD).resolution),
        .sdi = A429_SDI_ALL,
        .ssm.ssm_bcd = A429_SSM_BCD_PLUS_NORTH_EAST};

    a429_encode_params_t bnr_params = {
        .payload.value = random_value_bnr(GET_TABLE_ELEM(a429_example_table, LABEL_BNR).encoding.width,
                                          GET_TABLE_ELEM(a429_example_table, LABEL_BNR).scale),
        .sdi = A429_SDI_ALL,
        .ssm.ssm_bcd = A429_SSM_BNR_NORMAL};

    a429_encode_params_t disc_params = {
        .payload.discrete = random_value_disc(GET_TABLE_ELEM(a429_example_table, LABEL_DISC).encoding.width),
        .sdi = A429_SDI_ALL,
        .ssm.ssm_bcd = A429_SSM_DISC_NORMAL};

    a429_basic_example(&bcd_params, LABEL_BCD);
    a429_basic_example(&bnr_params, LABEL_BNR);
    a429_basic_example(&disc_params, LABEL_DISC);

    return 0;
}
