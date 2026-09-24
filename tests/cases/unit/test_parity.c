#include "unity.h"
#include "a429/parity.h"

void setUp(void)
{
}

void tearDown(void)
{
}

void test_parity_all_zero(void)
{
    uint32_t word = 0x00000000;

    word = a429_apply_parity(word);

    TEST_ASSERT_TRUE(a429_verify_parity(word));
}
void test_parity_all_ones(void)
{
    uint32_t word = 0xFFFFFFFF;

    word = a429_apply_parity(word);

    TEST_ASSERT_TRUE(a429_verify_parity(word));
}

void test_parity_known_pattern(void)
{
    uint32_t word = 0x12345678;

    word = a429_apply_parity(word);

    TEST_ASSERT_TRUE(a429_verify_parity(word));
}

/*
 * Main runner
 */
int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_parity_all_zero);
    RUN_TEST(test_parity_all_ones);
    RUN_TEST(test_parity_known_pattern);

    return UNITY_END();
}