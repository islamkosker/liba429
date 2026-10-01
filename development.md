# liba429 Development Status

This document tracks the current implementation status and planned work for `liba429`.

---

## Current Status

**Core library:** ✅ Implemented
**High-level API:** ✅ Implemented
**Encoding / Decoding:** ✅ Implemented
**Dictionary-driven configuration:** ✅ Implemented
**Testing:** ✅ Implemented
**Documentation:** 🚧 In Progress

---

## Core

### Word Representation

* [x] ARINC 429 word type
* [x] Label field access
* [x] SDI field access
* [x] Data field access
* [x] SSM field access
* [x] Parity field access
* [x] Configurable payload extraction
* [x] Configurable payload insertion

### Parity

* [x] Odd parity calculation
* [x] Parity verification
* [x] Parity application

### Label Representation

* [x] Label bit reversal
* [x] Software ↔ wire-level Label conversion
* [x] Pack / unpack API

---

## Encoding

### BNR

* [x] Configurable payload width
* [x] Two's-complement encoding
* [x] Scale factor
* [x] Resolution
* [x] Offset
* [x] Range checking

### BCD

* [x] Packed BCD encoding
* [x] Variable payload width
* [x] Partial most-significant digit support
* [x] Resolution handling
* [x] Range checking

### Discrete

* [x] Discrete payload encoding
* [x] Configurable payload width

### Word Fields

* [x] Label
* [x] SDI
* [x] Data
* [x] SSM
* [x] Parity

---

## Decoding

### BNR

* [x] Two's-complement decoding
* [x] Configurable payload width
* [x] Scale factor
* [x] Resolution
* [x] Offset

### BCD

* [x] Packed BCD decoding
* [x] Variable payload width
* [x] Partial most-significant digit support
* [x] Invalid BCD detection

### Discrete

* [x] Discrete payload extraction
* [x] Configurable payload width

### SSM

* [x] BNR SSM interpretation
* [x] BCD SSM interpretation
* [x] Discrete SSM interpretation
* [x] Library-level `NOT_USE` semantics

### SDI

* [x] SDI extraction
* [x] SDI encoding
* [x] Configurable SDI usage through payload layout

---

## Label Dictionary

**Status:** ✅ Complete

### Descriptor

* [x] Label
* [x] Equipment identifier
* [x] Name
* [x] Unit
* [x] Encoding type
* [x] Payload begin position
* [x] Payload width
* [x] Resolution
* [x] Scale
* [x] Offset
* [x] Bit time metadata

### Dictionary Table

* [x] Static dictionary table
* [x] Direct Label-indexed lookup
* [x] Designated initializers
* [x] BNR dictionary macro
* [x] BCD dictionary macro
* [x] Discrete dictionary macro

---

## High-Level API

**Status:** ✅ Complete

* [x] Dictionary-driven encoding
* [x] Dictionary-driven decoding
* [x] Automatic codec selection
* [x] Label validation
* [x] Error reporting

---

## Testing

**Status:** ✅ Complete

### Unit Tests

* [x] Word field operations
* [x] Parity
* [x] Label bit reversal
* [x] BNR encoding
* [x] BNR decoding
* [x] BCD encoding
* [x] BCD decoding
* [x] Discrete encoding
* [x] Discrete decoding
* [x] SSM handling

### Integration Tests

* [x] Encode → Decode
* [x] Decode → Encode
* [x] Dictionary-driven protocol flow
* [x] Configurable payload layouts

### Edge Cases

* [x] Invalid BCD digits
* [x] BNR range limits
* [x] Invalid payload widths
* [x] Invalid labels
* [x] Invalid arguments
* [x] Parity errors

---

## Documentation

**Status:** 🚧 In Progress

* [x] README
* [x] Architecture documentation
* [x] Encoding documentation
* [x] API documentation
* [x] Testing documentation
* [x] Complete Doxygen comments
* [x] Generate API reference
* [ ] Documentation website

---

## Future Protocol Support

### Williamsburg Protocol

**Status:** ⏳ Planned / Optional

The Williamsburg protocol layer is planned as a future extension.
It is intentionally kept separate from the core ARINC 429 word
encoding and decoding functionality.

* [ ] RTS
* [ ] CTS
* [ ] ACK
* [ ] SOT
* [ ] EOT
* [ ] State machine
* [ ] Block transfer

---

## Future Ideas

The following items are outside the current core scope and may be considered in future versions:

* [ ] Pretty printer
* [ ] CSV label importer
* [ ] JSON label importer
* [ ] YAML label importer
* [ ] Signal monitor
* [ ] Logging utilities
* [ ] PCAP export
* [ ] Benchmark suite
* [ ] Documentation website

---

## Scope

`liba429` focuses on ARINC 429 word representation, encoding, decoding,
dictionary-driven configuration, and validation.

Protocol-level communication features and higher-level tooling are
considered separate extensions and are not part of the current core
scope.
