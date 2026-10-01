# liba429 Architecture Documentation

## 1. Overview and Design Philosophy

`liba429` is a **hardware-independent, modular, and lightweight C11 library** for encoding and decoding **ARINC 429** data words.

The library is designed around the following principles:

* **Embedded-Friendly and Lightweight:** The library does not use dynamic memory allocation (`malloc`/`free`). Data structures are statically allocated and provide predictable memory usage.
* **Transparent Codec Principle:** The library does not make application-level business logic decisions. It encodes the values, SDI, and SSM information provided by the application into an ARINC 429 word, or decodes an ARINC 429 word back into its corresponding values.
* **Hardware Independence:** The library does not depend on specific drivers, DMA controllers, or hardware registers. It operates on 32-bit `a429_word_t` software words.
* **Dictionary-Driven Architecture:** Encoding parameters are not hard-coded for individual labels. Each ARINC 429 label is configured through an `a429_label_dictionary_t` entry.
* **Separation of Concerns:** Word manipulation, parity handling, data encoding, and protocol management are implemented as separate components.

---

## 2. Layered Architecture

`liba429` follows a layered architecture based on the **Separation of Concerns** principle.

```text
                       Application Layer
                              │
                              ▼
                    ┌──────────────────────────────────┐
                    │          a429_protocol           │
                    │   encode_word / decode_word     │
                    └────────────────┬─────────────────┘
                                     │
                              Dictionary Lookup
                                     │
                                     ▼
                    ┌──────────────────────────────────┐
                    │          a429_codec              │
                    │       Codec Dispatcher           │
                    └────────────────┬─────────────────┘
                                     │
                 ┌───────────────────┼───────────────────┐
                 ▼                   ▼                   ▼
        ┌─────────────────┐ ┌─────────────────┐ ┌─────────────────┐
        │    a429_bnr     │ │    a429_bcd     │ │    a429_disc    │
        │   BNR Codec     │ │   BCD Codec     │ │ Discrete Codec  │
        └────────┬────────┘ └────────┬────────┘ └────────┬────────┘
                 │                   │                   │
                 └───────────────────┼───────────────────┘
                                     ▼
                    ┌──────────────────────────────────┐
                    │      a429_word / a429_parity     │
                    │ Bit Operations / Parity Handling │
                    └────────────────┬─────────────────┘
                                     │
                                     ▼
                              32-bit ARINC Word
```

### Layer Responsibilities

| Layer                | Files                          | Main Responsibility                                                                                                                                         |
| :------------------- | :----------------------------- | :---------------------------------------------------------------------------------------------------------------------------------------------------------- |
| **Protocol**         | `a429_protocol.c`, `liba429.h` | Public high-level API. Performs dictionary lookup, label validation, parity handling, and dispatches encoding/decoding operations to the appropriate codec. |
| **Codec Dispatcher** | `a429_codec.c`, `a429_codec.h` | Selects the appropriate codec based on the label type (`BNR`, `BCD`, or `DISC`) and coordinates payload encoding/decoding.                                  |
| **BNR Codec**        | `a429_bnr.c`, `a429_bnr.h`     | Performs scaling, encoding, and decoding of Binary Number Representation values using signed binary representation.                                         |
| **BCD Codec**        | `a429_bcd.c`, `a429_bcd.h`     | Performs Binary Coded Decimal encoding and decoding using 4-bit decimal digit nibbles.                                                                      |
| **Discrete Codec**   | `a429_disc.c`, `a429_disc.h`   | Handles encoding and decoding of discrete bit fields and status values.                                                                                     |
| **Word & Parity**    | `a429_word.c`, `a429_parity.c` | Provides low-level bit manipulation, odd parity generation/verification, and ARINC 429 label bit reversal for wire-level representation.                    |

The protocol layer acts as the main coordination point, while the codec layer isolates encoding-specific logic from the rest of the library.

---

## 3. ARINC 429 Word Structure and Representations

An ARINC 429 word consists of 32 bits. The logical software representation and the wire-level representation differ primarily in the ordering of the label bits.

### 3.1. Software Representation — `a429_word_t`

Inside the software, the ARINC 429 word is represented as a 32-bit unsigned integer.

```text
 31         30 29                 11 10      9 8        1
┌─────────────┬─────────────────────┬─────────┬──────────┐
│   PARITY    │         SSM         │  DATA   │   SDI    │  LABEL
└─────────────┴─────────────────────┴─────────┴──────────┘
    Bit 32        Bit 30 - 31        Bit 11-29  Bit 9-10   Bit 1-8
   (Parity)        (Status)           (Payload)  (System)   (Label)
```

The corresponding C bit positions are:

| Field          | ARINC 429 Bits | C Bit Position |   Width |
| -------------- | -------------: | -------------: | ------: |
| Label          |            1–8 |            0–7 |  8 bits |
| SDI            |           9–10 |            8–9 |  2 bits |
| Data / Payload |          11–29 |          10–28 | 19 bits |
| SSM            |          30–31 |          29–30 |  2 bits |
| Parity         |             32 |             31 |   1 bit |

* **Label:** ARINC 429 label value, commonly represented in octal notation.
* **SDI:** Source/Destination Identifier.
* **Data / Payload:** Standard 19-bit data field.
* **SSM:** Sign/Status Matrix.
* **Parity:** Odd parity bit.

---

### 3.2. Wire-Level Representation — `a429_wire_data_t`

ARINC 429 transmits the label bits in reverse order relative to their conventional software representation.

The remaining bits retain their ARINC 429 transmission ordering.

`liba429` isolates this representation difference through:

```c
a429_wire_data_t a429_pack_word(a429_word_t word);

a429_word_t a429_unpack_word(a429_wire_data_t wire_data);
```

Conceptually:

```text
Software Representation
          │
          │ pack
          ▼
Wire-Level Representation
          │
          │ transmit
          ▼
     ARINC 429 Bus
```

The 8-bit label reversal is implemented using a precomputed 256-entry lookup table:

```c
a429_bit_reverse_table
```

This allows label reversal to be performed through a constant-time table lookup rather than repeated bit-by-bit operations.

---

## 4. Dictionary-Driven Architecture

`liba429` uses the `a429_label_dictionary_t` structure to separate label-specific configuration from the encoding and decoding implementation.

A dictionary entry contains the information required to interpret an ARINC 429 label:

```c
typedef struct {
    uint8_t label;                /**< ARINC label number (0-255) */
    uint16_t equipment_id;        /**< Equipment identifier */
    const char *name;             /**< Parameter name, e.g. "ALTITUDE" */
    const char *unit;             /**< Actual unit, e.g. "ft" */
    a429_label_type_t ltype;      /**< Label type: BNR, BCD, or DISC */
    a429_payload_info_t encoding; /**< Payload start bit and width */
    uint32_t bit_time;            /**< Configured bit timing information */
    a429_value_t scale;           /**< Full-scale value */
    a429_value_t resolution;      /**< LSB resolution */
    a429_value_t offset;          /**< Actual value offset */
} a429_label_dictionary_t;
```

The dictionary table is implemented as a 256-element pointer array:

```c
typedef const a429_label_dictionary_t *
    a429_dictionary_table_t[256];
```

Each index corresponds directly to an ARINC 429 label value.

This provides efficient label lookup without requiring a search through a list of dictionary entries.

### Label Configuration

The label type determines which codec is selected:

```text
                    Dictionary Entry
                           │
                           │ ltype
                           ▼
                 ┌────────────────────┐
                 │   Codec Selection  │
                 └─────────┬──────────┘
                           │
              ┌────────────┼────────────┐
              ▼            ▼            ▼
             BNR          BCD          DISC
              │            │            │
              ▼            ▼            ▼
         a429_bnr      a429_bcd      a429_disc
```

For labels using an already supported encoding type, adding a new label generally requires only a new dictionary entry; the codec implementation itself does not need to be modified.

This provides a **data-driven configuration model** while keeping the encoding logic independent from individual label definitions.

---

## 5. Configurable Payload Layout and SDI/SSM Handling

Standard ARINC 429 words provide a 19-bit data field between bits 11 and 29.

`liba429` allows the payload location and width to be configured through:

```c
a429_payload_info_t encoding;
```

which contains:

```text
encoding.begin
encoding.width
```

This allows the same codec infrastructure to support payload layouts that include fields normally reserved for SDI or SSM.

---

### 5.1. SSM Coverage

The end position of the configured payload is calculated as:

```text
payload_end = encoding.begin + encoding.width - 1
```

The payload boundary determines whether the standard SSM field remains available as an external field.

#### Case A — `payload_end < 30`

The payload ends at bit 29 or earlier.

```text
Payload
        │
        ▼
Bits 11 ... 29

SSM
        │
        ▼
Bits 30 ... 31
```

In this configuration, SSM remains an independent field and can be extracted using:

```c
a429_get_ssm(word);
```

The decoded SSM value is therefore returned through the corresponding `result->ssm` field.

#### Case B — `payload_end >= 30`

The configured payload overlaps at least part of the standard SSM field.

In this configuration, SSM is considered part of the payload and is not treated as an independent external field.

The corresponding decoded SSM result is therefore set to:

```c
A429_SSM_NOT_USE
```

This approach allows the payload layout to determine SSM handling without requiring a separate `has_ssm` configuration flag.

---

### 5.2. SDI Coverage

The standard SDI field occupies bits 9–10.

For the currently supported payload layouts, `encoding.begin` determines whether SDI remains an external field or becomes part of the payload.

#### Case A — Payload starts at `A429_DATA_BEGIN`

The payload starts at bit 11.

```text
Label | SDI | Payload
1-8   | 9-10| 11-...
```

SDI remains an independent field and can be extracted using:

```c
a429_get_sdi(word);
```

The decoded SDI value is returned through:

```c
result->sdi
```

#### Case B — Payload starts at `A429_SDI_BEGIN`

The payload starts at bit 9.

```text
Label |       Payload
1-8   | 9-...
```

In this configuration, SDI is part of the payload and is therefore not treated as an independent SDI field.

The decoded SDI result is set to:

```c
A429_SDI_NOT_USE
```

This design allows SDI handling to be derived from the payload layout rather than requiring a separate configuration flag.

---

## 6. Codec Dispatch

The codec dispatcher provides a common abstraction between the protocol layer and individual encoding implementations.

Conceptually:

```text
                 a429_protocol
                       │
                       ▼
                Dictionary Lookup
                       │
                       ▼
                 Label Type
                       │
          ┌────────────┼────────────┐
          ▼            ▼            ▼
         BNR          BCD          DISC
          │            │            │
          ▼            ▼            ▼
       BNR Codec    BCD Codec    DISC Codec
```

The protocol layer does not need to contain separate encoding algorithms for each label type.

Instead, it selects the appropriate codec through the codec abstraction.

This keeps:

* protocol management independent from encoding algorithms,
* individual codecs independently testable,
* label configuration data-driven,
* new encoding implementations easier to integrate.

---

## 7. Encoding Data Flow

The high-level encoding operation follows this sequence:

```text
Application Parameters
          │
          ▼
    Dictionary Lookup
          │
          ▼
      Label Validation
          │
          ▼
      Codec Selection
          │
          ▼
   ┌──────┼──────┐
   ▼      ▼      ▼
  BNR    BCD    DISC
   │      │      │
   └──────┼──────┘
          ▼
   Payload Generation
          │
          ▼
    SDI / SSM Handling
          │
          ▼
     Parity Generation
          │
          ▼
       a429_word_t
```

The resulting `a429_word_t` represents the complete logical ARINC 429 word.

If required by the target interface, the application can subsequently convert it to wire-level representation using:

```c
a429_pack_word();
```

---

## 8. Decoding Data Flow

The high-level decoding operation follows the reverse sequence:

```text
       a429_word_t
            │
            ▼
     Parity Verification
            │
            ▼
       Label Extraction
            │
            ▼
      Dictionary Lookup
            │
            ▼
       Codec Selection
            │
       ┌────┼────┐
       ▼    ▼    ▼
      BNR  BCD  DISC
       │    │    │
       └────┼────┘
            ▼
       Payload Decode
            │
            ▼
      SDI / SSM Extraction
            │
            ▼
    a429_decode_result_t
```

Parity verification is performed before the decoded data is accepted as valid.

An invalid parity condition results in:

```c
A429_ERR_BAD_PARITY
```

---

## 9. BNR Encoding Model

Binary Number Representation (BNR) uses signed binary representation for actual values.

For a payload width of `N` bits, the resolution is calculated from the configured scale:

```text
Resolution = Scale / 2^(N - 1)
```

The general conversion can be represented as:

```text
Actual Value
        │
        ▼
 Apply Offset / Scaling
        │
        ▼
 Quantization
        │
        ▼
 Signed Integer
        │
        ▼
 Payload Bits
```

The inverse operation reconstructs the actual value from the encoded integer.

For the standard ARINC 429 BNR representation, bit 29 is used as the sign bit.

The configurable `encoding.begin` and `encoding.width` fields allow the payload representation to be adapted to the label definition.

Because encoding involves quantization, the decoded value may differ slightly from the original actual value. The difference is bounded by the configured representation resolution.

---

## 10. BCD Encoding Model

Binary Coded Decimal (BCD) represents decimal digits using 4-bit nibbles.

For example:

```text
Decimal Value: 6939

   6       9       3       9
0110    1001    0011    1001
```

During encoding, the actual value is normalized using the configured resolution and converted into decimal digits.

Each encoded digit must satisfy:

```text
0 <= digit <= 9
```

An invalid digit results in:

```c
A429_ERR_INVALID_BCD
```

The available number of BCD digits depends on the configured payload width.

---

## 11. Discrete Encoding Model

Discrete labels represent bit-oriented states, flags, or enumerated status information.

The dictionary specifies the payload location and width, while the discrete codec handles the corresponding bit representation.

Conceptually:

```text
Discrete State
      │
      ▼
  Bit Encoding
      │
      ▼
Payload Field
      │
      ▼
ARINC 429 Word
```

The discrete codec does not impose application-specific semantic meaning on the encoded states. Interpretation remains the responsibility of the application or label configuration.

---

## 12. SDI and SSM Model

### SDI

The Source/Destination Identifier occupies bits 9–10 in the standard ARINC 429 word layout.

In liba429, SDI handling depends on whether these bits are part of the configured payload.

```text
External SDI:

Label | SDI | Payload
       │
       └── protocol-managed
```

or:

```text
Payload includes SDI:

Label | Payload
       │
       └── codec-managed
```

This distinction is derived from the dictionary configuration.

### SSM

The Sign/Status Matrix occupies bits 30–31 in the standard word layout.

Its interpretation depends on the label encoding type.

| SSM  | BNR              | BCD                         |
| ---- | ---------------- | --------------------------- |
| `00` | Failure Warning  | Plus / North / East / Right |
| `01` | No Computed Data | No Computed Data            |
| `10` | Functional Test  | Functional Test             |
| `11` | Normal Operation | Minus / South / West / Left |

When SSM is part of the configured payload, it is not independently interpreted by the protocol layer.

---

## 13. Error Handling

The public API uses `a429_error_t` to report operation results.

Common error conditions include:

| Error                    | Description                                               |
| ------------------------ | --------------------------------------------------------- |
| `A429_ERR_NONE`          | Operation completed successfully                          |
| `A429_ERR_BAD_PARITY`    | Parity verification failed                                |
| `A429_ERR_UNKNOWN_LABEL` | Label is not present in the dictionary                    |
| `A429_ERR_INVALID_BCD`   | Invalid BCD digit was detected                            |
| `A429_ERR_OUT_OF_RANGE`  | Value cannot be represented using the configured encoding |
| `A429_ERR_INVALID_ARG`   | Invalid function argument                                 |
| `A429_ERR_INVALID_LABEL` | Invalid label value                                       |
| `A429_ERR_INVALID_BIT`   | Invalid bit configuration                                 |

The complete error enumeration is defined by the public API in `liba429.h`.

---

## 14. Verification and Testing

The library uses **Unity** for unit testing and **CMake/CTest** for test execution.

The test suite covers:

* BNR encoding and decoding
* BCD encoding and decoding
* Discrete encoding and decoding
* Boundary values
* Invalid payload widths
* Invalid BCD values
* Parity generation and verification
* SDI handling
* SSM handling
* Dictionary-driven protocol behavior
* Encode/decode round trips
* Wire-format pack/unpack operations

### Round-Trip Verification

A typical end-to-end test follows this sequence:

```text
Actual Value
       │
       ▼
     Encode
       │
       ▼
    ARINC Word
       │
       ▼
      Pack
       │
       ▼
Wire Representation
       │
       ▼
     Unpack
       │
       ▼
    ARINC Word
       │
       ▼
     Decode
       │
       ▼
Actual Value
```

Because the encoding process may introduce quantization, the decoded actual value is compared against the expected representational tolerance rather than requiring exact floating-point equality.

---

## 15. Boundary Testing

Boundary Value Analysis is used to verify behavior near representational limits.

Examples include:

```text
Minimum representable value
Maximum representable value
Value below the representable range
Value above the representable range
Zero
Negative values
Invalid BCD digits
Invalid payload widths
Invalid parity
```

These tests verify both successful encoding/decoding and expected error handling.

---

## 16. Design Summary

The overall architecture can be summarized as:

```text
                    ┌───────────────────┐
                    │    Application    │
                    └─────────┬─────────┘
                              │
                              ▼
                    ┌───────────────────┐
                    │   a429_protocol   │
                    │ High-Level API    │
                    └─────────┬─────────┘
                              │
                              ▼
                    ┌───────────────────┐
                    │    Dictionary     │
                    │ Label Definition  │
                    └─────────┬─────────┘
                              │
                              ▼
                    ┌───────────────────┐
                    │   a429_codec      │
                    │ Codec Dispatcher  │
                    └─────────┬─────────┘
                              │
                 ┌────────────┼────────────┐
                 ▼            ▼            ▼
              ┌─────┐      ┌─────┐      ┌──────┐
              │ BNR │      │ BCD │      │ DISC │
              └─────┘      └─────┘      └──────┘
                 │            │            │
                 └────────────┼────────────┘
                              ▼
                    ┌───────────────────┐
                    │    a429_word      │
                    │ Bit Manipulation  │
                    └─────────┬─────────┘
                              │
                              ▼
                    ┌───────────────────┐
                    │   a429_parity     │
                    │  Odd Parity       │
                    └─────────┬─────────┘
                              │
                              ▼
                       ARINC 429 Word
```

The architecture provides a clear separation between:

* application-level protocol handling,
* label configuration,
* encoding-specific algorithms,
* low-level word manipulation,
* parity handling,
* and wire-level representation.

This design allows `liba429` to remain small and hardware-independent while supporting configurable ARINC 429 label definitions and multiple encoding formats.
