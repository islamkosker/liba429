#ifndef A429_TEST_UTILS
#define A429_TEST_UTILS

#include <stdio.h>

#define RANDOM_TEST_ITERATIONS 10000

static inline void set_invariant_randomized_error_msg(char *m, int e, int s, int i)
{
    snprintf(
        m,
        sizeof(m),
        "FAIL: status = %d seed = %d iter = %d",
        e, s, i);
}

#endif // A429_TEST_UTILS