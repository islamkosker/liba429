# liba429 API Reference

This document describes the public API provided by `liba429`, including its public data types, enumerations, macros, and API functions.

The public API is exposed through:

```c
#include "liba429.h"
```

The umbrella header is intended to provide access to the library's public functionality without requiring applications to include internal implementation headers directly.

---

## 1. Core Data Types and Enumerations

### 1.1. Basic Types

The following types are defined by `liba429`:

```c
typedef uint32_t a429_word_t;       /**< 32-bit logical ARINC 429 software word */
typedef uint32_t a429_wire_data_t;  /**< 32-bit ARINC 429 wire-level representation */
typedef bool     a429_discrete_t;   /**< Discrete data type */
typedef double   a429_value_t;      /**< Actual value used by BNR/BCD */
```

### 1.2. Error Codes

`a429_error_t` describes the result of an operation.

```c
typedef enum {
    A429_ERR_NO            = 0,     /**< Operation completed successfully */
    A429_ERR_DECODE        = 127,   /**< General decoding error */
    A429_ERR_ENCODE        = 126,   /**< General encoding error */
    A429_ERR_OUT_OF_RANGE  = 125,   /**< Value is outside the configured range */
    A429_ERR_INVALID_BCD   = 124,   /**< Invalid BCD digit (> 9) */
    A429_ERR_INVALID_BIT   = 123,   /**< Invalid bit-field configuration */
    A429_ERR_INVALID_ARG   = 122,   /**< Invalid argument, such as NULL */
    A429_ERR_INVALID_LABEL = 121,   /**< Word label does not match the dictionary entry */
    A429_ERR_BAD_PARITY    = 120,   /**< Odd parity verification failed */
    A429_ERR_UNKNOWN_LABEL = 119    /**< Label was not found in the dictionary table */
} a429_error_t;
```

A successful operation returns:

```c
A429_ERR_NO
```

Errors are returned directly by the public API functions, allowing applications to handle encoding and decoding failures without exceptions or dynamic error objects.

---

## 1.3. Sign / Status Matrix (SSM) Enumerations

SSM interpretation depends on the encoding type.

### BNR SSM — `a429_ssm_bnr_t`

```text
A429_SSM_BNR_FAILURE          0x00
A429_SSM_BNR_NO_COMPUTE_DATA  0x01
A429_SSM_BNR_FUNCTIONAL_TEST  0x02
A429_SSM_BNR_NORMAL           0x03
A429_SSM_BNR_NOT_USE          0x04
```

Meanings:

|  Value | Name                           | Meaning                       |
| :----: | :----------------------------- | :---------------------------- |
| `0x00` | `A429_SSM_BNR_FAILURE`         | Failure Warning               |
| `0x01` | `A429_SSM_BNR_NO_COMPUTE_DATA` | No Computed Data              |
| `0x02` | `A429_SSM_BNR_FUNCTIONAL_TEST` | Functional Test               |
| `0x03` | `A429_SSM_BNR_NORMAL`          | Normal Operation              |
| `0x04` | `A429_SSM_BNR_NOT_USE`         | SSM is not used independently |

### BCD SSM — `a429_ssm_bcd_t`

```text
A429_SSM_BCD_PLUS_NORTH_EAST   0x00
A429_SSM_BCD_NO_COMPUTE_DATA   0x01
A429_SSM_BCD_FUNCTIONAL_TEST   0x02
A429_SSM_BCD_MINUS_SOUTH_WEST  0x03
A429_SSM_BCD_NOT_USE            0x04
```

Meanings:

|  Value | Name                            | Meaning                       |
| :----: | :------------------------------ | :---------------------------- |
| `0x00` | `A429_SSM_BCD_PLUS_NORTH_EAST`  | Plus / North / East / Right   |
| `0x01` | `A429_SSM_BCD_NO_COMPUTE_DATA`  | No Computed Data              |
| `0x02` | `A429_SSM_BCD_FUNCTIONAL_TEST`  | Functional Test               |
| `0x03` | `A429_SSM_BCD_MINUS_SOUTH_WEST` | Minus / South / West / Left   |
| `0x04` | `A429_SSM_BCD_NOT_USE`          | SSM is not used independently |

### Discrete SSM — `a429_ssm_disc_t`

```text
A429_SSM_DISC_NORMAL           0x00
A429_SSM_DISC_NO_COMPUTE_DATA  0x01
A429_SSM_DISC_FUNCTIONAL_TEST  0x02
A429_SSM_DISC_FAILURE          0x03
A429_SSM_DISC_NOT_USE          0x04
```

Meanings:

|  Value | Name                            | Meaning                       |
| :----: | :------------------------------ | :---------------------------- |
| `0x00` | `A429_SSM_DISC_NORMAL`          | Normal Operation              |
| `0x01` | `A429_SSM_DISC_NO_COMPUTE_DATA` | No Computed Data              |
| `0x02` | `A429_SSM_DISC_FUNCTIONAL_TEST` | Functional Test               |
| `0x03` | `A429_SSM_DISC_FAILURE`         | Failure Warning               |
| `0x04` | `A429_SSM_DISC_NOT_USE`         | SSM is not used independently |

---

## 1.4. Source / Destination Identifier

The `a429_sdi_t` enumeration represents the 2-bit SDI field.

```text
A429_SDI_ALL      0x00
A429_SDI_SYS1     0x01
A429_SDI_SYS2     0x02
A429_SDI_SYS3     0x03
A429_SDI_NOT_USE  0x04
```

Meanings:

|  Value | Name               | Meaning                       |
| :----: | :----------------- | :---------------------------- |
| `0x00` | `A429_SDI_ALL`     | All Systems                   |
| `0x01` | `A429_SDI_SYS1`    | System 1                      |
| `0x02` | `A429_SDI_SYS2`    | System 2                      |
| `0x03` | `A429_SDI_SYS3`    | System 3                      |
| `0x04` | `A429_SDI_NOT_USE` | SDI is not used independently |

`A429_SDI_NOT_USE` is a library-level value indicating that SDI is not being handled as an independent field for the current payload configuration.

---

## 1.5. Parameter and Result Structures

SSM is represented as a union because its enumeration type depends on the encoding type.

```c
typedef union a429_ssm {
    a429_ssm_bnr_t  ssm_bnr;
    a429_ssm_bcd_t  ssm_bcd;
    a429_ssm_disc_t ssm_disc;
} a429_ssm_t;
```

The payload union provides storage for either an actual value or a discrete bit field:

```c
typedef union a429_payload {
    uint32_t     discrete;
    a429_value_t value;
} a429_payload_u;
```

The common word-field structure contains the decoded/encoded payload together with SDI and SSM information:

```c
typedef struct a429_word_fields {
    a429_payload_u payload;  /**< Actual value or discrete bit field */
    a429_sdi_t     sdi;      /**< SDI field */
    a429_ssm_t     ssm;      /**< SSM field */
} a429_word_fields_t;
```

The same structure is used for encoding parameters and decoding results:

```c
typedef a429_word_fields_t a429_encode_params_t;
typedef a429_word_fields_t a429_decode_result_t;
```

---

# 2. High-Level Protocol API

The high-level protocol API provides dictionary-driven ARINC 429 word encoding and decoding.

---

## 2.1. `a429_encode_word`

Encodes the supplied payload, SDI, and SSM information according to the selected dictionary entry and produces a 32-bit logical ARINC 429 word.

The function also applies the ARINC 429 odd parity bit.

```c
a429_error_t a429_encode_word(
    uint8_t label,
    a429_word_t *out_word,
    const a429_encode_params_t *params,
    const a429_dictionary_table_t *table
);
```

### Parameters

| Parameter  | Description                                                 |
| :--------- | :---------------------------------------------------------- |
| `label`    | 8-bit ARINC 429 label identifying the data item             |
| `out_word` | Output pointer receiving the encoded logical ARINC 429 word |
| `params`   | Payload, SDI, and SSM values to encode                      |
| `table`    | Dictionary table containing the label definition            |

### Return Value

Returns `A429_ERR_NO` on success.

Possible errors include:

* `A429_ERR_INVALID_ARG`
* `A429_ERR_UNKNOWN_LABEL`
* `A429_ERR_INVALID_LABEL`
* `A429_ERR_INVALID_BIT`
* `A429_ERR_OUT_OF_RANGE`
* `A429_ERR_INVALID_BCD`
* `A429_ERR_ENCODE`

---

## 2.2. `a429_decode_word`

Validates the parity of a logical ARINC 429 word, retrieves its label definition from the dictionary table, and decodes the payload according to the configured encoding.

```c
a429_error_t a429_decode_word(
    a429_word_t word,
    a429_decode_result_t *result,
    const a429_dictionary_table_t *table,
    uint8_t *out_label
);
```

### Parameters

| Parameter   | Description                                                  |
| :---------- | :----------------------------------------------------------- |
| `word`      | Logical 32-bit ARINC 429 word to decode                      |
| `result`    | Output structure receiving the decoded payload, SDI, and SSM |
| `table`     | Dictionary table containing label definitions                |
| `out_label` | Output pointer receiving the decoded label                   |

### Return Value

Returns `A429_ERR_NO` on success.

Possible errors include:

* `A429_ERR_INVALID_ARG`
* `A429_ERR_BAD_PARITY`
* `A429_ERR_UNKNOWN_LABEL`
* `A429_ERR_INVALID_BCD`
* `A429_ERR_DECODE`

---

# 3. Wire-Level Serialization

ARINC 429 labels use a bit ordering that differs from the logical software representation used internally by the library.

`liba429` therefore provides explicit pack/unpack operations for converting between the logical word representation and the wire-level representation.

---

## 3.1. `a429_pack_word`

Converts a logical ARINC 429 software word into its wire-level representation.

The operation includes the required label bit ordering transformation.

```c
a429_wire_data_t a429_pack_word(a429_word_t word);
```

Conceptually:

```text
Logical Software Word
          │
          ▼
    Label Bit Reversal
          │
          ▼
   Wire-Level Word
```

The function does not perform physical transmission. It only produces the 32-bit representation expected by the next hardware/driver layer.

---

## 3.2. `a429_unpack_word`

Converts a wire-level ARINC 429 representation into the logical software representation used by the library.

```c
a429_word_t a429_unpack_word(a429_wire_data_t wire_data);
```

Conceptually:

```text
Wire-Level Word
       │
       ▼
 Label Bit Reversal
       │
       ▼
Logical Software Word
```

---

# 4. Low-Level Bit and Parity API

The low-level APIs provide direct access to ARINC 429 word fields and parity operations.

These functions are primarily intended for protocol implementation, testing, and applications that require direct bit-level access.

---

## 4.1. Word Field Access

The following operations are provided by the word manipulation interface.

### `a429_get_label(word)`

Returns the label field from bits 1–8.

```text
ARINC 429 bits: 1–8
Software bits:  0–7
Width:          8 bits
```

### `a429_get_sdi(word)`

Returns the SDI field from bits 9–10.

```text
ARINC 429 bits: 9–10
Software bits:  8–9
Width:          2 bits
```

### `a429_get_ssm(word)`

Returns the SSM field from bits 30–31.

```text
ARINC 429 bits: 30–31
Software bits:  29–30
Width:          2 bits
```

### `a429_get_parity(word)`

Returns the parity bit from bit 32.

```text
ARINC 429 bit:  32
Software bit:  31
Width:         1 bit
```

---

## 4.2. Generic Bit-Field Access

### `a429_set_bits`

Writes a value into a specified bit field.

```c
a429_word_t a429_set_bits(
    a429_word_t word,
    uint32_t payload,
    uint8_t begin,
    uint8_t width
);
```

The field is defined by:

```text
begin
width
```

The function therefore supports configurable payload layouts rather than assuming a fixed 19-bit data field.

### `a429_get_bits`

Extracts a value from a specified bit field.

```c
uint32_t a429_get_bits(
    a429_word_t word,
    uint8_t begin,
    uint8_t width
);
```

These operations are used internally by the encoding and decoding layers.

---

# 5. Parity API

ARINC 429 uses **odd parity** across the complete 32-bit word.

The parity API provides operations for calculating, validating, and applying the parity bit.

---

## 5.1. `a429_compute_parity`

Calculates the parity bit required for the first 31 bits of the word.

```c
uint32_t a429_compute_parity(a429_word_t word);
```

The returned value represents the parity bit required to make the total number of `1` bits in the complete 32-bit word odd.

---

## 5.2. `a429_verify_parity`

Checks whether the complete word satisfies the ARINC 429 odd-parity requirement.

```c
bool a429_verify_parity(a429_word_t word);
```

A valid word contains an odd number of `1` bits across all 32 bits.

---

## 5.3. `a429_apply_parity`

Calculates and inserts the correct parity bit into the word.

```c
a429_word_t a429_apply_parity(a429_word_t word);
```

The returned word contains the appropriate odd parity bit.

---

# 6. Dictionary-Driven API Usage

The high-level API uses a dictionary table to associate an ARINC 429 label with its encoding configuration.

A typical dictionary entry contains information such as:

```c
label
equipment_id
name
unit
ltype
encoding.begin
encoding.width
bit_time
resolution
scale
offset
```

For example:

```c
#define LABEL_ALTITUDE 0102

static const a429_dictionary_table_t my_table = {
    A429_BNR(
        LABEL_ALTITUDE,
        100,
        "ALTITUDE",
        "ft",
        A429_DATA_BEGIN,
        A429_DEFAULT_DATA_WIDTH,
        100,
        0.01,
        40000.0,
        0.0
    )
};
```

The dictionary entry determines how the payload is encoded and decoded.

For an encoding type already supported by the library, adding a new label generally requires only a new dictionary entry.

---

# 7. Complete Example

The following example demonstrates the complete logical data flow:

1. Encode an actual value.
2. Generate an ARINC 429 logical word.
3. Convert the word to wire-level representation.
4. Convert it back to logical representation.
5. Decode the received word.

```c
#include "liba429.h"
#include <stdio.h>

#define LABEL_ALTITUDE 0102

static const a429_dictionary_table_t my_table = {
    A429_BNR(
        LABEL_ALTITUDE,
        100,
        "ALTITUDE",
        "ft",
        A429_DATA_BEGIN,
        A429_DEFAULT_DATA_WIDTH,
        100,
        0.01,
        40000.0,
        0.0
    )
};

int main(void)
{
    a429_encode_params_t tx_params = {
        .payload.value = 12500.5,
        .sdi           = A429_SDI_SYS1,
        .ssm.ssm_bnr   = A429_SSM_BNR_NORMAL
    };

    a429_word_t tx_word = 0;

    /* Encode */
    a429_error_t err =
        a429_encode_word(
            LABEL_ALTITUDE,
            &tx_word,
            &tx_params,
            &my_table
        );

    if (err != A429_ERR_NO) {
        return -1;
    }

    /* Convert to wire-level representation */
    a429_wire_data_t wire = a429_pack_word(tx_word);

    /*
     * In a real application, 'wire' would be passed to
     * the ARINC 429 hardware/driver layer.
     *
     * This example converts it back directly to simulate
     * a transmit/receive round trip.
     */

    a429_word_t rx_word = a429_unpack_word(wire);

    /* Decode */
    a429_decode_result_t rx_result = {0};
    uint8_t rx_label = 0;

    err =
        a429_decode_word(
            rx_word,
            &rx_result,
            &my_table,
            &rx_label
        );

    if (err == A429_ERR_NO) {
        printf(
            "Label 0%o received: Value = %.2f ft\n",
            rx_label,
            rx_result.payload.value
        );
    }

    return 0;
}
```

---

# 8. API Data Flow

The complete high-level API flow can be summarized as:

```text
                    Dictionary
                        │
                        ▼
Application ──► a429_encode_word()
                        │
                        ▼
                 Logical ARINC Word
                        │
                        ▼
                 a429_pack_word()
                        │
                        ▼
                Wire-Level Data
                        │
                  Hardware/Driver
                        │
                        ▼
               Wire-Level Data
                        │
                        ▼
                a429_unpack_word()
                        │
                        ▼
                 Logical ARINC Word
                        │
                        ▼
                 a429_decode_word()
                        │
                        ▼
                    Result
```

The high-level API therefore keeps application-level encoding and decoding independent from the hardware transport layer.

---

# 9. Public API Design Principles

The public API follows several design principles.

### Single Public Entry Point

Applications can include:

```c
#include "liba429.h"
```

without depending on internal headers.

### Dictionary-Driven Configuration

Encoding behavior is determined by dictionary entries rather than hard-coded label-specific logic.

### Static Configuration

The dictionary can be defined statically at compile time, avoiding dynamic memory allocation.

### Hardware Independence

The library operates on 32-bit software and wire-level representations and does not directly depend on:

* hardware registers
* DMA controllers
* vendor-specific drivers
* microcontroller peripherals

### Deterministic Data Processing

The encoding and decoding paths use deterministic transformations and do not require dynamic memory allocation.

### Layered API

Applications can use either:

* the high-level protocol API for normal encoding/decoding, or
* lower-level word/parity operations when direct bit-level control is required.

This allows the library to remain lightweight while still exposing the functionality required for protocol-level applications and verification.
