#ifndef TEST_PROTOCOL_MOCK_H
#define TEST_PROTOCOL_MOCK_H

#include "liba429.h"
#include "a429_word.h"
#include <stdlib.h>
#include <time.h>

#define CAT_HELPER(a, b) a##b
#define CAT(a, b) CAT_HELPER(a, b)

#define STRINGIFY_HELPER(x) #x
#define STRINGIFY(x) STRINGIFY_HELPER(x)

#define NAME(x) STRINGIFY(CAT(TEST_NAME_, x))
#define UNIT(x) STRINGIFY(CAT(TEST_UNIT_, x))

#define DEFAULT_BIT_TIME 100

#define TEST_SCALE 1.0
#define TEST_OFFSET 0.0
#define TEST_RESOLUTION 0.01

#define TEST_DATA 10
#define TEST_DATA_SSM 20
#define TEST_DATA_SDI 30
#define TEST_DATA_SDI_SSM 40

const a429_dictionary_table_t a429_table = {
    // standart (ID begin 10)
    A429_BCD(TEST_DATA, TEST_DATA, NAME(TEST_DATA), UNIT(TEST_DATA), A429_DATA_BEGIN,
             A429_DEFAULT_DATA_WIDTH, DEFAULT_BIT_TIME, TEST_RESOLUTION, TEST_SCALE, TEST_OFFSET),

    A429_BNR(TEST_DATA + 1, TEST_DATA + 1, NAME(TEST_DATA + 1), UNIT(TEST_DATA + 1), A429_DATA_BEGIN,
             A429_DEFAULT_DATA_WIDTH, DEFAULT_BIT_TIME, TEST_RESOLUTION, TEST_SCALE, TEST_OFFSET),

    A429_DISC(TEST_DATA + 2, TEST_DATA + 2, NAME(TEST_DATA + 2), UNIT(TEST_DATA + 2), A429_DATA_BEGIN,
              A429_DEFAULT_DATA_WIDTH, DEFAULT_BIT_TIME, TEST_RESOLUTION, TEST_SCALE, TEST_OFFSET),

    // payload = data + ssm (ID begin 20)
    A429_BCD(TEST_DATA_SSM, TEST_DATA_SSM, NAME(TEST_DATA_SSM), UNIT(TEST_DATA_SSM), A429_DATA_BEGIN,
             A429_DATA_SSM_WIDTH, DEFAULT_BIT_TIME, TEST_RESOLUTION, TEST_SCALE, TEST_OFFSET),

    A429_BNR(TEST_DATA_SSM + 1, TEST_DATA_SSM + 1, NAME(TEST_DATA_SSM + 1), UNIT(TEST_DATA_SSM + 1), A429_DATA_BEGIN,
             A429_DATA_SSM_WIDTH, DEFAULT_BIT_TIME, TEST_RESOLUTION, TEST_SCALE, TEST_OFFSET),

    A429_DISC(TEST_DATA_SSM + 2, TEST_DATA_SSM + 2, NAME(TEST_DATA_SSM + 2), UNIT(TEST_DATA_SSM + 2), A429_DATA_BEGIN,
              A429_DATA_SSM_WIDTH, DEFAULT_BIT_TIME, TEST_RESOLUTION, TEST_SCALE, TEST_OFFSET),

    // paylaoad = data + sdi (ID begin 30)
    A429_BCD(TEST_DATA_SDI, TEST_DATA_SDI, NAME(TEST_DATA_SDI), UNIT(TEST_DATA_SDI), A429_SDI_BEGIN,
             A429_DATA_SDI_WIDTH, DEFAULT_BIT_TIME, TEST_RESOLUTION, TEST_SCALE, TEST_OFFSET),

    A429_BNR(TEST_DATA_SDI + 1, TEST_DATA_SDI + 1, NAME(TEST_DATA_SDI + 1), UNIT(TEST_DATA_SDI + 1), A429_SDI_BEGIN,
             A429_DATA_SDI_WIDTH, DEFAULT_BIT_TIME, TEST_RESOLUTION, TEST_SCALE, TEST_OFFSET),

    A429_DISC(TEST_DATA_SDI + 2, TEST_DATA_SDI + 2, NAME(TEST_DATA_SDI + 2), UNIT(TEST_DATA_SDI + 2), A429_SDI_BEGIN,
              A429_DATA_SDI_WIDTH, DEFAULT_BIT_TIME, TEST_RESOLUTION, TEST_SCALE, TEST_OFFSET),

    // payload = ssm + data + sdi (ID begin 40)
    A429_BCD(TEST_DATA_SDI_SSM, TEST_DATA_SDI_SSM, NAME(TEST_DATA_SDI_SSM), UNIT(TEST_DATA_SDI_SSM), A429_SDI_BEGIN,
             A429_MAX_PAYLOAD_WIDTH, DEFAULT_BIT_TIME, TEST_RESOLUTION, TEST_SCALE, TEST_OFFSET),

    A429_BNR(TEST_DATA_SDI_SSM + 1, TEST_DATA_SDI_SSM + 1, NAME(TEST_DATA_SDI_SSM + 1), UNIT(TEST_DATA_SDI_SSM + 1), A429_SDI_BEGIN,
             A429_MAX_PAYLOAD_WIDTH, DEFAULT_BIT_TIME, TEST_RESOLUTION, TEST_SCALE, TEST_OFFSET),

    A429_DISC(TEST_DATA_SDI_SSM + 2, TEST_DATA_SDI_SSM + 2, NAME(TEST_DATA_SDI_SSM + 2), UNIT(TEST_DATA_SDI_SSM + 2), A429_SDI_BEGIN,
              A429_MAX_PAYLOAD_WIDTH, DEFAULT_BIT_TIME, TEST_RESOLUTION, TEST_SCALE, TEST_OFFSET),
};

#define GET_TABLE_ELEM(table, idx) (*(table)[idx])

unsigned long long large_rand()
{
    unsigned long long r = 0;
    for (int i = 0; i < 5; i++)
    {
        r = (r << 15) | (rand() & 0x7FFF);
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
    uint32_t capacity = UINT32_C(1) << (width - 1U);
    uint32_t max_positive = capacity - UINT32_C(1);
    double resolution = scale / (double)capacity;
    double max_positive_value = (double)max_positive * resolution;
    double random_multiplier = ((double)rand() / (double)RAND_MAX) * 2.0 - 1.0;
    return random_multiplier * max_positive_value;
}

static inline uint32_t random_value_disc(uint8_t width, bool all_zero, bool all_one)
{
    uint32_t disc_word = 0;
    for (size_t i = 0; i < width; i++)
    {
        if (all_zero)
            disc_word = disc_word | (0 << i);
        if (all_one)
            disc_word = disc_word | (1 << i);
        disc_word |= ((uint32_t)(rand() % 2) << i);
    }
    return disc_word;
}

#endif // TEST_PROTOCOL_MOCK_H