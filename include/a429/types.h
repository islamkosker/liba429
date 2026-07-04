#ifndef A429_TYPES
#define A429_TYPES

#include <stdbool.h>
#include <stdint.h>

typedef uint32_t a429_word_t;
typedef bool a429_discrete_t;

typedef enum {
    A429_SSM_BNR_FAILURE          = 0x00, // 00: Failure Warning
    A429_SSM_BNR_NO_COMPUTE_DATA  = 0x01, // 01: No Computed Data
    A429_SSM_BNR_FUNCTIONAL_TEST  = 0x02, // 10: Functional Test
    A429_SSM_BNR_NORMAL           = 0x03  // 11: Normal Operation
} a429_ssm_bnr_t;

typedef enum {
    A429_SSM_BCD_PLUS_NORTH_EAST  = 0x00, // 00: Plus, North, East, Right, To, Above
    A429_SSM_BCD_NO_COMPUTE_DATA  = 0x01, // 01: No Computed Data
    A429_SSM_BCD_FUNCTIONAL_TEST  = 0x02, // 10: Functional Test
    A429_SSM_BCD_MINUS_SOUTH_WEST = 0x03  // 11: Minus, South, West, Left, From, Below
} a429_ssm_bcd_t;

typedef enum {
    A429_SSM_DISC_NORMAL          = 0x00, // 00: Verified Data, Normal Operation 
    A429_SSM_DISC_NO_COMPUTE_DATA = 0x01, // 01: No Computed Data
    A429_SSM_DISC_FUNCTIONAL_TEST = 0x02, // 10: Functional Test
    A429_SSM_DISC_FAILURE         = 0x03  // 11: Failure Warning 
} a429_ssm_disc_t;


#endif