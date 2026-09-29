#include "a429_disc.h"
#include "decode_mock.h"
#define UNITY_INCLUDE_CONFIG_H

#include "unity.h"
#include "test_utils.h"

#define DATA_WIDTH 19
static unsigned int test_seed;

void setUp(void) {}
void tearDown(void) {}

static uint32_t random_discrete_word(size_t width, bool all_zero, bool all_one)
{
    uint32_t disc_word = 0;
    for (size_t i = 0; i < width; i++)
    {
        if (all_zero)
            disc_word = disc_word | (0 << i);
        if (all_one)
            disc_word = disc_word | (1 << i);
        disc_word = disc_word | ((rand() % 2) == 0 ? 1 : 0 << i);
    }

    return disc_word;
}

void test_dics_random_max_field(void)
{
    uint32_t random_bit_field = random_discrete_word(A429_MAX_PAYLOAD_WIDTH, false, false);

    uint32_t disc_word = 0;

    a429_set_discrete_field(&disc_word, random_bit_field, A429_SDI_BEGIN, A429_MAX_PAYLOAD_WIDTH);

    uint32_t decoded = a429_get_discrete_field(disc_word, A429_SDI_BEGIN, A429_MAX_PAYLOAD_WIDTH);
    TEST_ASSERT_EQUAL_UINT32(random_bit_field, decoded);
}

void test_dics_random_data_field(void)
{
    uint32_t random_bit_field = random_discrete_word(DATA_WIDTH, false, false);

    uint32_t disc_word = 0;

    a429_set_discrete_field(&disc_word, random_bit_field, A429_DATA_BEGIN, DATA_WIDTH);

    uint32_t decoded = a429_get_discrete_field(disc_word, A429_DATA_BEGIN, DATA_WIDTH);

    TEST_ASSERT_EQUAL_UINT32(random_bit_field, decoded);
}

void test_dics_random_all_zero(void)
{
    uint32_t random_bit_field = random_discrete_word(DATA_WIDTH, true, false);

    uint32_t disc_word = 0;

    a429_set_discrete_field(&disc_word, random_bit_field, A429_DATA_BEGIN, DATA_WIDTH);

    uint32_t decoded = a429_get_discrete_field(disc_word, A429_DATA_BEGIN, DATA_WIDTH);

    TEST_ASSERT_EQUAL_UINT32(random_bit_field, decoded);
}

void test_dics_random_all_one(void)
{
    uint32_t random_bit_field = random_discrete_word(DATA_WIDTH, false, true);

    uint32_t disc_word = 0;

    a429_set_discrete_field(&disc_word, random_bit_field, A429_DATA_BEGIN, DATA_WIDTH);

    uint32_t decoded = a429_get_discrete_field(disc_word, A429_DATA_BEGIN, DATA_WIDTH);

    TEST_ASSERT_EQUAL_UINT32(random_bit_field, decoded);
}

int main(void)
{
    test_seed = (unsigned)time(NULL);
    srand(test_seed);
    UNITY_BEGIN();
    RUN_TEST(test_dics_random_max_field);
    RUN_TEST(test_dics_random_data_field);
    RUN_TEST(test_dics_random_all_zero);
    RUN_TEST(test_dics_random_all_one);

    return UNITY_END();
}
