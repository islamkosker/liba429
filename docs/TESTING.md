# liba429 Testing and Verification

This document describes the test architecture, test categories, Unity-based unit testing infrastructure, and procedures used to build and execute the `liba429` test suite.

The testing strategy focuses on verifying the correctness of the encoding/decoding algorithms, protocol-level behavior, bit-field handling, parity processing, and logical-to-wire-level conversions.

---

## 1. Test Architecture and Infrastructure

`liba429` uses the following tools for testing and build automation:

* **Unity** for C unit testing
* **CMake** for test build configuration
* **CTest** for test discovery and execution

The test suite is designed to verify both individual codec components and complete protocol-level data flows.

### Core Testing Principles

#### High Code Coverage

The test suite targets the major functional paths of:

* BNR codec
* BCD codec
* Discrete codec
* Parity calculation and verification
* SDI/SSM handling
* Word bit-field operations
* Wire-level pack/unpack operations
* Protocol-level encode/decode paths

Coverage should be measured using an external coverage tool when a quantitative coverage percentage is required.

#### Invariant / Round-Trip Verification

A central testing strategy is to encode a value and then decode the resulting word.

Conceptually:

```text
Input
  │
  ▼
Encode
  │
  ▼
ARINC 429 Word
  │
  ▼
Decode
  │
  ▼
Output
```

For values that are exactly representable, the decoded value should match the original value.

For quantized BNR and BCD values, the decoded value is expected to match within the appropriate quantization tolerance.

#### Quantization Tolerance

BNR and BCD encoding may introduce quantization because actual values are converted into finite-resolution representations.

Floating-point round-trip tests therefore use an appropriate tolerance, for example:

```c
TEST_ASSERT_DOUBLE_WITHIN(...)
```

The selected tolerance should reflect the configured resolution and the expected quantization behavior of the codec.

---

# 2. Test Categories

The tests are organized under the `test/` directory into several functional categories.

---

## 2.1. Unit Tests

Unit tests verify individual library components independently from the complete protocol flow.

### `test_parity.c`

Tests ARINC 429 odd-parity calculation and verification using cases such as:

* all-zero words
* all-one patterns
* known bit patterns
* valid parity
* invalid parity

The tests verify both parity generation and parity validation.

---

### `test_bnr.c`

Tests the BNR codec using:

* randomized values
* round-trip/invariant testing
* boundary values
* zero
* positive and negative values
* values close to the representable limits
* out-of-range values
* invalid payload widths

Typical boundary cases include:

```text
0
+resolution
-resolution
+(scale - resolution)
-(scale)
```

The exact representable limits depend on the configured payload width and scale.

---

### `test_bcd.c`

Tests the BCD codec using:

* randomized decimal values
* valid BCD representations
* invalid BCD nibbles
* minimum values
* maximum representable values
* resolution-based values
* round-trip encoding and decoding

Invalid BCD fields must produce:

```text
A429_ERR_INVALID_BCD
```

when an extracted BCD digit contains a value greater than `9`.

---

### `test_disc.c`

Tests the Discrete codec using:

* all-zero values
* all-one values within the configured width
* randomized bit patterns
* different payload widths
* bit-field positioning

Because Discrete data is treated as a raw bit field, no numerical scaling or resolution is applied.

---

# 2.2. Protocol Invariant Tests

The protocol invariant tests validate the complete high-level API:

```c
a429_encode_word()
a429_decode_word()
```

The tests therefore verify the interaction between:

* dictionary lookup
* codec dispatch
* payload encoding
* SDI handling
* SSM handling
* parity generation
* payload decoding
* result reconstruction

The current test configuration covers four payload layout combinations.

---

### 1. Data Only

Standard 19-bit data payload:

```text
begin = 11
width = 19
```

The payload occupies the standard ARINC 429 data field.

SDI and SSM are handled as independent fields.

```text
LABEL | SDI |      DATA      | SSM | PARITY
```

---

### 2. Data + SSM

21-bit payload:

```text
begin = 11
width = 21
```

The payload extends into the SSM field:

```text
LABEL | SDI |         PAYLOAD          | PARITY
       │    │                           │
       │    └──── includes SSM bits ────┘
       └────── independent SDI
```

Since the payload overlaps the SSM field, SSM is treated as part of the payload rather than as an independent field.

---

### 3. Data + SDI

21-bit payload:

```text
begin = 9
width = 21
```

The payload begins at the SDI field and therefore includes the SDI bits.

```text
LABEL |          PAYLOAD          | SSM | PARITY
       │                           │
       └──── includes SDI bits ───┘
```

---

### 4. Data + SDI + SSM

23-bit payload:

```text
begin = 9
width = 23
```

The payload spans the SDI, data, and SSM regions:

```text
LABEL |             PAYLOAD             | PARITY
       │                                │
       └──── SDI + DATA + SSM ──────────┘
```

These tests verify that the codec correctly derives SDI/SSM behavior from the configured payload layout.

---

# 2.3. Wire-Level Round-Trip Tests

The following tests verify the logical-to-wire-level conversion path:

* `test_wire_bcd.c`
* `test_wire_bnr.c`
* `test_wire_disc.c`

The complete software-level data flow is:

```text
Encode Parameters
       │
       ▼
a429_encode_word()
       │
       ▼
Logical Software Word
       │
       ▼
a429_pack_word()
       │
       ▼
Wire-Level Representation
       │
       ▼
a429_unpack_word()
       │
       ▼
Logical Software Word
       │
       ▼
a429_decode_word()
       │
       ▼
Decoded Result
```

These tests do **not** simulate the electrical ARINC 429 physical layer.

Instead, they verify that the library correctly transforms between its logical software representation and the wire-level 32-bit representation, including label bit ordering.

The test therefore validates:

```text
encode
→ pack
→ unpack
→ decode
```

as a complete software round trip.

---

# 2.4. Generic Dictionary / Label Scan

A generic test can iterate through the configured dictionary table and execute the same protocol-level validation for each configured label.

Conceptually:

```c
void test_protocol_invariant_all_labels_generic(void)
{
    const size_t table_size = ARRAY_SIZE(a429_table);

    for (size_t i = 0; i < table_size; ++i) {
        const a429_label_dictionary_t *dict = a429_table[i];

        if (dict == NULL) {
            continue;
        }

        /*
         * Generate suitable test data according to
         * dict->ltype and dict->encoding.
         */

        /*
         * Encode -> Pack -> Unpack -> Decode
         */

        /*
         * Assert that the decoded result satisfies
         * the expected invariant.
         */
    }
}
```

This approach reduces duplicated test code and allows newly added dictionary entries to participate in the same generic validation flow.

For example, the test can select the appropriate input generation strategy according to:

```text
dict->ltype
```

and use:

```text
dict->encoding.begin
dict->encoding.width
dict->resolution
dict->scale
dict->offset
```

to construct appropriate test values.

For floating-point codecs, the verification can use a resolution-based tolerance.

For Discrete data, the decoded bit field can be compared directly against the encoded value.

---

# 3. Test Flow

A typical protocol-level invariant test follows this sequence:

```text
                 Dictionary
                     │
                     ▼
              Generate Input
                     │
                     ▼
             a429_encode_word()
                     │
                     ▼
                ARINC Word
                     │
                     ▼
             a429_decode_word()
                     │
                     ▼
              Decoded Result
                     │
                     ▼
                 Assertion
```

Wire-level tests extend the flow with packing and unpacking:

```text
                 Dictionary
                     │
                     ▼
              Generate Input
                     │
                     ▼
             a429_encode_word()
                     │
                     ▼
              Software Word
                     │
                     ▼
             a429_pack_word()
                     │
                     ▼
             Wire-Level Data
                     │
                     ▼
           a429_unpack_word()
                     │
                     ▼
              Software Word
                     │
                     ▼
             a429_decode_word()
                     │
                     ▼
              Decoded Result
                     │
                     ▼
                 Assertion
```

---

# 4. Boundary and Negative Testing

The test suite also verifies invalid and boundary conditions.

Important cases include:

### BNR

* minimum representable value
* maximum representable value
* zero
* values close to the limits
* negative values
* out-of-range values
* invalid payload widths

### BCD

* zero
* maximum representable value
* individual decimal digits `0–9`
* invalid digit values `10–15`
* values close to payload capacity
* resolution-based values

### Discrete

* zero
* maximum value for the configured width
* alternating bit patterns
* randomized bit fields

### Protocol

* unknown label
* invalid label
* invalid arguments
* invalid payload configuration
* invalid BCD data
* bad parity
* payload configurations involving SDI
* payload configurations involving SSM
* payload configurations involving both SDI and SSM

Negative tests are particularly important because they verify that invalid protocol data is rejected rather than silently decoded.

---

# 5. Building and Running the Tests

Tests are enabled through the CMake option:

```text
-DLIBA429_ENABLE_TESTS=ON
```

### 5.1. Configure the Project

```bash
cmake -S . -B build -DLIBA429_ENABLE_TESTS=ON
```

### 5.2. Build

```bash
cmake --build build
```

### 5.3. Run Tests

```bash
ctest --test-dir build --output-on-failure
```

`--output-on-failure` displays the output of failed tests, making test failures easier to diagnose.

---

# 6. Recommended Development Workflow

A typical development cycle is:

```text
Modify Source
     │
     ▼
Configure / Build
     │
     ▼
Run Unit Tests
     │
     ▼
Run Protocol Tests
     │
     ▼
Run Wire-Level Tests
     │
     ▼
Review Failures
     │
     ▼
Modify / Fix
     │
     └──────────────► Repeat
```

When adding a new codec feature or changing the payload layout logic, the corresponding unit tests should be updated first, followed by protocol-level invariant tests.

When adding a new dictionary entry for an already-supported encoding type, the generic dictionary scan can be used to automatically include the new label in the validation process.

---

# 7. Verification Strategy Summary

The `liba429` test architecture uses multiple layers of verification:

| Test Level               | Main Purpose                                                 |
| :----------------------- | :----------------------------------------------------------- |
| Unit Tests               | Verify individual codec and utility functions                |
| Boundary Tests           | Verify representable limits and edge conditions              |
| Negative Tests           | Verify invalid input handling                                |
| Protocol Invariant Tests | Verify complete encode/decode behavior                       |
| Wire-Level Tests         | Verify pack/unpack transformations                           |
| Generic Dictionary Tests | Validate multiple configured labels using a common test flow |

The overall verification model can be summarized as:

```text
Individual Functions
        │
        ▼
     Codecs
        │
        ▼
 Protocol Encode/Decode
        │
        ▼
 Wire-Level Conversion
        │
        ▼
 Complete Software Round Trip
```

This layered approach allows failures to be isolated more easily while also providing end-to-end verification of the public API.

The objective is not only to verify individual functions, but also to demonstrate that the complete transformation:

```text
Data
      ↓
Encode
      ↓
ARINC 429 Word
      ↓
Pack
      ↓
Wire-Level Representation
      ↓
Unpack
      ↓
ARINC 429 Word
      ↓
Decode
      ↓
Data
```

preserves the expected protocol semantics within the configured representation and quantization limits.
