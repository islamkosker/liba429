# liba429 Encoding Mathematics

This document describes the mathematical encoding and decoding models used by `liba429` for the following ARINC 429 data types:

* **BNR (Binary Number Representation)**
* **BCD (Binary Coded Decimal)**
* **Discrete**
* **SDI (Source/Destination Identifier)**
* **SSM (Sign/Status Matrix)**

The document focuses on the mathematical transformations between actual values and their ARINC 429 bit-field representations.

---

## 1. BNR (Binary Number Representation)

BNR is used to represent continuous actual quantities such as altitude, airspeed, vertical speed, temperature, acceleration, and similar physical values.

The encoded value is represented as a **signed two's-complement integer**.

### 1.1 Mathematical Model and Resolution

The BNR payload contains `N` bits, where `N` is defined by `encoding.width`.

For the standard ARINC 429 BNR representation, the most significant payload bit is the **sign bit**.

The resolution (LSB value) is derived from the configured `scale`:

$$
\text{Resolution} =
\frac{\text{Scale}}{2^{N-1}}
$$

where:

* `N` = payload width
* `Scale` = configured full-scale magnitude
* `Resolution` = actual value represented by one LSB

For example, with:

```text
width = 19
scale = 1.0
```

the resolution is:

$$
\text{Resolution} =
\frac{1.0}{2^{18}}
$$

The signed integer range of an `N`-bit two's-complement value is:

$$
-2^{N-1} \leq x \leq 2^{N-1}-1
$$

Therefore, with the resolution above, the representable actual range is:

$$
-\text{Scale}
\leq
\text{value}
<
\text{Scale}
$$

The positive endpoint is one LSB below `Scale`.

---

### 1.2 BNR Encoding

The BNR encoding process consists of the following steps.

#### Step 1 — Apply the configured offset

If an offset is configured, the actual value is converted to a value relative to the offset:

$$
\text{adjusted\_value}
=
\text{value}
-
\text{offset}
$$

This is the inverse operation of the offset addition performed during decoding.

#### Step 2 — Range validation

The adjusted value must be representable by the configured BNR width and scale.

For the standard model:

$$
-\text{Scale}
\leq
\text{adjusted\_value}
<
\text{Scale}
$$

If the value cannot be represented, `A429_ERR_OUT_OF_RANGE` is returned.

#### Step 3 — Quantization

The actual value is converted to an integer number of LSBs:

$$
\text{scaled\_int}
=
\operatorname{round}
\left(
\frac{\text{adjusted\_value}}
{\text{Resolution}}
\right)
$$

The use of rounding converts the continuous actual value into the nearest representable BNR value.

#### Step 4 — Two's-complement representation

The signed integer is represented in `N` bits.

The corresponding bit mask is:

$$
\text{mask}
=
2^N-1
$$

The encoded field is obtained by:

$$
\text{bnr\_value}
=
\text{scaled\_int}
\;\&\;
\text{mask}
$$

For negative values, the resulting bit pattern is the `N`-bit two's-complement representation.

#### Step 5 — Insert the payload

The encoded value is written to the configured payload location using:

```c
a429_set_bits(...)
```

The payload position is determined by:

```text
encoding.begin
encoding.width
```

The BNR codec therefore does not assume that the payload always starts at the standard data-bit position.

---

### 1.3 BNR Decoding

The decoding process reverses the encoding operation.

#### Step 1 — Extract the raw payload

The raw BNR field is extracted using:

```text
encoding.begin
encoding.width
```

#### Step 2 — Sign extension

If the most significant bit of the configured BNR field is set, the value represents a negative two's-complement number.

For an `N`-bit payload:

$$
\text{sign\_bit}
=
1 \ll (N-1)
$$

If the sign bit is set, the value is sign-extended to the library's integer representation.

Conceptually:

$$
\text{signed\_value}
=
\text{sign-extended}(\text{bnr\_value},N)
$$

#### Step 3 — Convert to actual value

The signed integer is converted back into an actual value:

$$
\text{decoded\_value}
=
\text{signed\_value}
\times
\text{Resolution}
+
\text{offset}
$$

Therefore, encoding and decoding form the following transformation:

```text
Actual Value
       │
       ▼
  subtract offset
       │
       ▼
    / resolution
       │
       ▼
     round()
       │
       ▼
 two's-complement
       │
       ▼
   ARINC payload
```

and the reverse:

```text
   ARINC payload
       │
       ▼
 two's-complement
       │
       ▼
   × resolution
       │
       ▼
    + offset
       │
       ▼
Actual Value
```

---

## 2. BCD (Binary Coded Decimal)

BCD represents decimal values by storing each decimal digit in a 4-bit binary nibble.

Each BCD digit must satisfy:

$$
0 \leq d_i \leq 9
$$

Values from `1010` to `1111` are therefore invalid BCD digits.

---

### 2.1 Digit and Bit Structure

The number of complete BCD digits is:

$$
\text{full\_digits}
=
\left\lfloor
\frac{\text{width}}{4}
\right\rfloor
$$

The number of remaining bits is:

$$
\text{remaining\_bits}
=
\text{width}\bmod4
$$

For example, with:

```text
width = 19
```

the payload contains:

* 4 complete 4-bit BCD digits
* 3 additional bits for the most significant digit

Therefore:

```text
19 = 4 × 4 + 3
```

The four complete nibbles can represent decimal digits `0–9`.

The remaining 3-bit field can represent values from:

$$
0 \ldots 2^3-1
$$

or:

```text
0 ... 7
```

Consequently, a 19-bit BCD field can represent values from:

```text
00000
```

through:

```text
79999
```

giving a maximum decimal capacity of **79,999** for a positive unsigned BCD value.

The exact number of usable digits depends on the configured payload width.

---

### 2.2 BCD Encoding

#### Step 1 — Apply the configured offset

If an offset is configured:

$$
\text{adjusted\_value}
=
\text{value}
-
\text{offset}
$$

#### Step 2 — Convert to an integer representation

The actual value is converted into resolution steps:

$$
\text{raw\_bcd\_int}
=
\operatorname{round}
\left(
\frac{\text{adjusted\_value}}
{\text{Resolution}}
\right)
$$

The resulting integer represents the decimal value that will be encoded digit by digit.

#### Step 3 — Extract decimal digits

Starting from the least significant digit, each decimal digit is obtained using modulo 10:

$$
d_i
=
\text{raw\_bcd\_int}
\bmod10
$$

The remaining value is then reduced:

$$
\text{raw\_bcd\_int}
=
\left\lfloor
\frac{\text{raw\_bcd\_int}}{10}
\right\rfloor
$$

This process is repeated for every BCD digit.

For example:

```text
6939
```

is decomposed as:

```text
6    9    3    9
│    │    │    │
0110 1001 0011 1001
```

The resulting nibbles are placed into the configured payload field.

#### Step 4 — Insert the BCD field

The generated BCD representation is written into the ARINC 429 word according to:

```text
encoding.begin
encoding.width
```

The BCD codec therefore operates on the configured payload rather than assuming a fixed payload location.

---

### 2.3 BCD Decoding

#### Step 1 — Extract the payload

The BCD field is extracted using:

```text
encoding.begin
encoding.width
```

#### Step 2 — Extract each decimal digit

Each complete 4-bit nibble is extracted from the payload.

For every digit:

$$
0 \leq d_i \leq 9
$$

must be satisfied.

If a nibble contains a value greater than `9`, the field is not valid BCD and:

```text
A429_ERR_INVALID_BCD
```

is returned.

#### Step 3 — Reconstruct the decimal value

The decimal value is reconstructed using powers of ten:

$$
\text{raw\_bcd\_value}
=
\sum_{i=0}^{K-1}
d_i\times10^i
$$

The actual value is then calculated as:

$$
\text{decoded\_value}
=
\text{raw\_bcd\_value}
\times
\text{Resolution}
+
\text{offset}
$$

The complete transformation is therefore:

```text
Actual Value
       │
       ▼
  subtract offset
       │
       ▼
    / resolution
       │
       ▼
     round()
       │
       ▼
 decimal digits
       │
       ▼
  4-bit BCD fields
       │
       ▼
   ARINC payload
```

---

## 3. Discrete Encoding

Discrete data does not represent a continuous physical quantity.

It is typically used to represent:

* flags
* switch states
* operating modes
* status bits
* boolean conditions
* system states

The discrete codec treats the configured payload as a bit field.

### 3.1 Encoding

The input discrete value is written directly into the configured field.

Conceptually:

$$
\text{discrete\_field}
=
\text{value}
\;\&\;
(2^N-1)
$$

where `N` is `encoding.width`.

The resulting field is shifted to `encoding.begin` and inserted into the ARINC 429 word.

No numerical scaling or resolution is applied.

---

### 3.2 Decoding

The configured field is extracted directly:

$$
\text{decoded\_discrete}
=
\text{payload}
$$

The resulting bit field is stored in:

```c
result->payload.discrete
```

The interpretation of individual discrete states is determined by the application or dictionary configuration.

---

## 4. SDI (Source/Destination Identifier)

The SDI field normally occupies ARINC 429 bits 9–10.

It contains a 2-bit identifier used to distinguish the source or destination associated with a word.

The four possible values are:

|      SDI     | Binary |
| :----------: | :----: |
| `A429_SDI_0` |  `00`  |
| `A429_SDI_1` |  `01`  |
| `A429_SDI_2` |  `10`  |
| `A429_SDI_3` |  `11`  |

`liba429` does not hard-code SDI usage for every label.

Instead, SDI handling is determined by the configured payload layout.

When the configured encoding uses the standard data position, SDI remains an independent field.

When the configured payload begins at `A429_SDI_BEGIN`, the payload may include the SDI bits as part of the encoded data field.

Therefore, SDI interpretation is derived from:

```text
encoding.begin
encoding.width
```

rather than from a separate `has_sdi` flag.

---

## 5. SSM (Sign / Status Matrix)

The SSM field normally occupies ARINC 429 bits 30–31.

The meaning of these two bits depends on the encoding type.

For standard layouts where the payload does not overlap the SSM field, `liba429` treats SSM as an independent field.

The following table describes the standard SSM meanings used by the library:

| Code | BNR              | BCD                         | Discrete         |
| :--: | :--------------- | :-------------------------- | :--------------- |
| `00` | Failure Warning  | Plus / North / East / Right | Normal Operation |
| `01` | No Computed Data | No Computed Data            | No Computed Data |
| `10` | Functional Test  | Functional Test             | Functional Test  |
| `11` | Normal Operation | Minus / South / West / Left | Failure Warning  |

### 5.1 BNR SSM

For BNR data:

```text
00 → Failure Warning
01 → No Computed Data
10 → Functional Test
11 → Normal Operation
```

### 5.2 BCD SSM

For BCD data:

```text
00 → Plus / North / East / Right
01 → No Computed Data
10 → Functional Test
11 → Minus / South / West / Left
```

### 5.3 Discrete SSM

For Discrete data:

```text
00 → Normal Operation
01 → No Computed Data
10 → Functional Test
11 → Failure Warning
```

---

## 6. Payload Layout and SDI/SSM Interaction

The codec does not assume that every encoding uses the standard 19-bit data field.

The configured payload is described by:

```text
encoding.begin
encoding.width
```

The final payload bit is:

$$
\text{payload\_end}
=
\text{encoding.begin}
+
\text{encoding.width}
-
1
$$

This allows the library to determine whether the payload overlaps the standard SDI or SSM fields.

### 6.1 Standard Data Payload

For a standard 19-bit payload:

```text
begin = A429_DATA_BEGIN
width = 19
```

the payload occupies the normal ARINC 429 data region.

SDI and SSM remain independent fields.

```text
LABEL | SDI |      DATA      | SSM | P
       │           │            │
       │           │            └── Independent SSM
       │           └─────────────── Payload
       └─────────────────────────── Independent SDI
```

### 6.2 Payload Including SDI

If:

```text
begin = A429_SDI_BEGIN
```

the payload begins at the SDI field and can therefore include the SDI bits.

The effective payload width determines whether SDI is part of the encoded value.

### 6.3 Payload Including SSM

If the configured payload extends into the SSM region:

```text
payload_end >= A429_SSM_BEGIN
```

the SSM bits overlap the payload and are treated as part of the encoded data rather than as an independent SSM field.

In this configuration, the independent SSM value is not used.

### 6.4 Combined SDI and SSM Payload

The same principle applies when the configured payload begins at the SDI field and extends through the SSM field.

The payload can therefore span:

```text
SDI + DATA + SSM
```

depending on its configured width.

This layout-driven approach allows the codec to handle different payload organizations without introducing separate boolean configuration flags such as:

```text
has_sdi
has_ssm
```

---

## 7. Quantization and Round-Trip Behavior

Encoding a floating-point actual value into a finite-width ARINC 429 field necessarily introduces quantization.

For example, if:

```text
resolution = 0.01
```

a value such as:

```text
2653.622835
```

cannot be represented exactly if the encoded value must be an integer number of resolution steps.

The encoded value is therefore rounded:

$$
\text{encoded\_steps}
=
\operatorname{round}
\left(
\frac{\text{value}-\text{offset}}
{\text{resolution}}
\right)
$$

After decoding:

$$
\text{decoded\_value}
=
\text{encoded\_steps}
\times
\text{resolution}
+
\text{offset}
$$

Therefore:

```text
encoded value ≈ decoded value
```

rather than:

```text
encoded value == decoded value
```

This behavior is expected and should be considered when designing round-trip tests.

Tests should generally use an appropriate tolerance instead of requiring exact floating-point equality.

---

## 8. Encoding and Decoding Symmetry

The codecs in `liba429` are designed around a reversible transformation model.

### BNR

```text
Actual Value
      ↓
 Offset Adjustment
      ↓
 Quantization
      ↓
 Two's Complement
      ↓
 Bit Field
      ↓
 ARINC Word
```

Decoding reverses the process:

```text
ARINC Word
      ↓
 Bit Field
      ↓
 Two's Complement
      ↓
 Resolution Scaling
      ↓
 Offset Restoration
      ↓
Actual Value
```

### BCD

```text
Actual Value
      ↓
 Offset Adjustment
      ↓
 Resolution Scaling
      ↓
 Decimal Integer
      ↓
 Decimal Digits
      ↓
 BCD Nibbles
      ↓
 ARINC Word
```

Decoding reverses the transformation:

```text
ARINC Word
      ↓
 BCD Nibbles
      ↓
 Decimal Digits
      ↓
 Decimal Integer
      ↓
 Resolution Scaling
      ↓
 Offset Restoration
      ↓
Actual Value
```

### Discrete

```text
Discrete Value
      ↓
 Bit Field
      ↓
 ARINC Word
```

and:

```text
ARINC Word
      ↓
 Bit Field
      ↓
 Discrete Value
```

---

## 9. Summary

The `liba429` encoding model is based on a small number of deterministic transformations:

| Encoding | Representation                 | Scaling | Resolution | Offset |
| :------- | :----------------------------- | :-----: | :--------: | :----: |
| BNR      | Two's-complement integer       |   Yes   |     Yes    |   Yes  |
| BCD      | Decimal digits / 4-bit nibbles |   Yes   |     Yes    |   Yes  |
| Discrete | Raw bit field                  |    No   |     No     |   No   |
| SDI      | 2-bit identifier               |    No   |     No     |   No   |
| SSM      | 2-bit status/sign field        |    No   |     No     |   No   |

The payload location and width are determined by the dictionary:

```text
encoding.begin
encoding.width
```

This keeps the codec implementation independent from a fixed payload layout and allows the same encoding logic to be reused across different ARINC 429 label configurations.

The mathematical design can therefore be summarized as:

$$
\boxed{
\text{Actual Value}
\rightarrow
\text{Quantized Representation}
\rightarrow
\text{ARINC 429 Payload}
\rightarrow
\text{ARINC 429 Word}
}
$$

with decoding performing the inverse transformation.
