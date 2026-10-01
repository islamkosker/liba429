# liba429

**liba429** is a lightweight, hardware-independent C11 library for encoding, decoding, packing, and unpacking **ARINC 429** words.

It provides a modular, dictionary-driven architecture for working with **BNR, BCD, and Discrete** data while handling ARINC 429-specific details such as label bit ordering, SDI, SSM, and odd parity.

## Features

* ARINC 429 word encoding and decoding
* BNR, BCD, and Discrete data types
* Dictionary-driven label configuration
* Configurable payload position and width
* SDI and SSM handling
* Odd parity generation and verification
* ARINC 429 label bit reversal
* Wire-level pack/unpack operations
* Hardware-independent design
* C11-compatible implementation
* CMake build system
* Unity and CTest-based testing
* Public API separated from internal implementation
* Suitable for embedded and general-purpose C applications

## Architecture

liba429 uses a layered architecture that separates word-level manipulation, data encoding, and application-facing protocol handling.

```text
                         Application
                              │
                              ▼
                    ┌──────────────────┐
                    │  a429_protocol   │
                    │  Encode / Decode │
                    └────────┬─────────┘
                             │
                       Dictionary
                          Lookup
                             │
                             ▼
                    ┌──────────────────┐
                    │    a429_codec    │
                    │    Dispatcher    │
                    └────────┬─────────┘
                             │
                 ┌───────────┼───────────┐
                 ▼           ▼           ▼
              ┌─────┐     ┌─────┐     ┌──────┐
              │ BNR │     │ BCD │     │ DISC │
              └─────┘     └─────┘     └──────┘
                 │           │           │
                 └───────────┼───────────┘
                             ▼
                    ┌──────────────────┐
                    │    a429_word     │
                    │ Bit Manipulation │
                    └────────┬─────────┘
                             │
                             ▼
                     ARINC 429 Word
```

The core library operates on 32-bit ARINC 429 words and does not directly access ARINC 429 hardware. Hardware drivers, communication interfaces, DMA, interrupts, and transceiver-specific implementations remain outside the library.

For a detailed description of the architecture, see [`docs/ARCHITECTURE.md`](docs/ARCHITECTURE.md).

## Dictionary-Driven Design

Encoding and decoding rules are defined through a static label dictionary rather than hard-coded label-specific protocol logic.

A dictionary entry contains information such as:

```text
Label
 ├── Equipment ID
 ├── Name
 ├── Unit
 ├── Type
 ├── Encoding
 │    ├── begin
 │    └── width
 ├── Bit Time
 ├── Resolution
 ├── Scale
 └── Offset
```

The label type determines the codec used for the payload:

```text
Dictionary
     │
     ├── BNR  ──► BNR codec
     │
     ├── BCD  ──► BCD codec
     │
     └── DISC ──► Discrete codec
```

For supported encoding types, adding a new label generally requires only a new dictionary entry.

## Configurable Payload Layout

The `encoding.begin` and `encoding.width` fields define the payload location within the ARINC 429 word.

This allows the same codec infrastructure to support different payload layouts, including configurations where SDI and/or SSM are included in the payload.

```text
Standard ARINC 429 word

┌────────┬────────┬───────────────────┬────────┬────────┐
│ LABEL  │  SDI   │       DATA        │  SSM   │ PARITY │
│ 8 bit  │ 2 bit  │      19 bit       │ 2 bit  │ 1 bit  │
└────────┴────────┴───────────────────┴────────┴────────┘
```

The payload layout is described entirely by the dictionary configuration, keeping codec implementations independent of individual label definitions.

## Supported Encodings

### BNR

Binary Number Representation (BNR) uses a signed binary representation with configurable resolution, scale, and offset.

The resolution for a payload width of `N` bits is:

```text
Resolution = Scale / 2^(N - 1)
```

BNR encoding includes range checking and quantization, while decoding reconstructs the corresponding physical value.

For details, see [`docs/ENCODING.md`](docs/ENCODING.md).

### BCD

Binary Coded Decimal (BCD) represents decimal digits using 4-bit nibbles.

```text
6939

  6       9       3       9
0110    1001    0011    1001
```

The codec validates BCD digit values and supports configurable payload widths.

### Discrete

Discrete labels represent bit-oriented or enumerated states rather than scaled continuous values.

The configured dictionary payload width determines the field size and placement.

## SDI and SSM

liba429 supports the ARINC 429 **Source/Destination Identifier (SDI)** and **Sign/Status Matrix (SSM)** fields.

Their handling depends on the configured payload layout and encoding type.

| Field  | Standard Position | Description                   |
| ------ | ----------------: | ----------------------------- |
| SDI    |         Bits 9–10 | Source/Destination Identifier |
| SSM    |        Bits 30–31 | Sign/Status Matrix            |
| Parity |            Bit 32 | Odd parity                    |

SSM interpretation is encoding-dependent. BNR, BCD, and Discrete data use different SSM semantics where applicable.

## Public API

The application-facing API is exposed through a single public header:

```c
#include "liba429.h"
```

### Encode

```c
a429_error_t a429_encode_word(
    uint8_t label,
    a429_word_t *out_word,
    const a429_encode_params_t *params,
    const a429_dictionary_table_t *table
);
```

### Decode

```c
a429_error_t a429_decode_word(
    a429_word_t word,
    a429_decode_result_t *result,
    const a429_dictionary_table_t *table,
    uint8_t *out_label
);
```

### Wire-level conversion

```c
a429_wire_data_t a429_pack_word(a429_word_t word);
a429_word_t a429_unpack_word(a429_wire_data_t wire_data);
```

The public API is documented in [`docs/API.md`](docs/API.md).

## Quick Start

A typical application workflow is:

```text
Dictionary
    │
    ▼
Encode
    │
    ▼
a429_word_t
    │
    ▼
Pack
    │
    ▼
Wire Representation
    │
    ▼
Hardware Driver / Interface
```

On reception, the reverse operation can be used:

```text
Hardware Driver / Interface
    │
    ▼
Wire Representation
    │
    ▼
Unpack
    │
    ▼
a429_word_t
    │
    ▼
Decode
    │
    ▼
Application Data
```

## Testing

liba429 uses **Unity** for unit testing and **CMake/CTest** for test execution.

The test suite covers:

* BNR, BCD, and Discrete encoding/decoding
* Boundary and negative testing
* Payload width and positioning
* SDI and SSM handling
* Parity generation and verification
* Dictionary-driven protocol behavior
* Encode/decode round trips
* Wire-level pack/unpack operations

Detailed testing and verification information is available in [`docs/TESTING.md`](docs/TESTING.md).

## Build

liba429 uses CMake.

### Configure

```bash
cmake -S . -B build -DLIBA429_ENABLE_TESTS=ON
```

### Build

```bash
cmake --build build
```

### Run tests

```bash
ctest --test-dir build --output-on-failure
```

## Project Structure

```text
liba429/
│
├── include/
│   └── liba429.h
│
├── src/
│   ├── a429_word.c
│   ├── a429_parity.c
│   ├── a429_codec.c
│   ├── a429_bnr.c
│   ├── a429_bcd.c
│   ├── a429_disc.c
│   └── a429_protocol.c
│
├── test/
│   ├── test_*.c
│   └── ...
│
├── example/
│   └── ...
│
├── docs/
│   ├── ARCHITECTURE.md
│   ├── API.md
│   ├── ENCODING.md
│   └── TESTING.md
│
├── CMakeLists.txt
├── LICENSE
└── README.md
```

## Scope

liba429 focuses on **ARINC 429 word-level processing**.

### Supported

* BNR
* BCD
* Discrete
* SDI
* SSM
* Odd parity
* Label bit reversal
* Configurable payload layout
* Wire-level pack/unpack

### Outside the Core Library

The following are intentionally outside the scope of liba429:

* ARINC 429 physical-layer transmission
* ARINC 429 receiver hardware
* Transceiver-specific drivers
* DMA
* Interrupt handling
* Hardware-specific I/O

This keeps the core library portable and independent of a particular ARINC 429 interface.

## Documentation

| Document                                  | Description                                       |
| ----------------------------------------- | ------------------------------------------------- |
| [`ARCHITECTURE.md`](docs/ARCHITECTURE.md) | Internal architecture and design                  |
| [`ENCODING.md`](docs/ENCODING.md)         | BNR, BCD, Discrete, SDI, SSM, and encoding models |
| [`API.md`](docs/API.md)                   | Public API reference                              |
| [`TESTING.md`](docs/TESTING.md)           | Testing and verification strategy                 |

## License

See [`LICENSE`](LICENSE) for licensing information.
