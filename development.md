### liba429 Development
This document tracks the implementation status of the library.

--------------------------------------------------------------------------------

### Current Focus
#### BCD
*  [ ] Packed BCD decoding
*  [ ] 3-bit MSB support
*  [ ] Variable digit count
--------------------------------------------------------------------------------

### Core
#### Types
**Status:**  ✅ Complete
**Files**
*  include/a429/types.h
**Implemented**
*  [x] Core library types
*  [x] ARINC 429 word types
*  [x] SSM enumerations

--------------------------------------------------------------------------------

#### Word API
**Status:**  ✅ Complete
**Files**
*  include/a429/word.h
**Implemented**
*  [x] Label getter
*  [x] SDI getter
*  [x] Data getter
*  [x] SSM getter
*  [x] Parity getter

--------------------------------------------------------------------------------

#### Parity
**Status:**  ✅ Complete
**Files**
*  include/a429/parity.h
**Implemented**
*  [x] Odd parity verification
*  [x] a429_check_parity()

--------------------------------------------------------------------------------

#### Bit Reversal
**Status:**  ✅ Complete
**Planned**
*  [x] 256-entry lookup table
*  [x] Hardware ↔ Logical label conversion
*  [x] Public API

--------------------------------------------------------------------------------

#### Pack / Unpack
**Status:**  ✅ Complete
**Planned**
*  [x] Hardware → Logical word conversion
*  [x] Logical → Hardware word conversion

--------------------------------------------------------------------------------

### Decoders
Status: 🚧 In Progress Planned
#### BNR
*  [x] Two's complement decoding
*  [x] Configurable bit width
*  [x] Scale factor support
#### BCD
*  [ ] Packed BCD decoding
*  [ ] 3-bit MSB support
*  [ ] Variable digit count
#### Discrete
*  [ ] Bit extraction
*  [ ] Individual discrete decoding
#### SSM
*  [ ] BNR interpretation
*  [ ] BCD interpretation
*  [ ] Discrete interpretation

--------------------------------------------------------------------------------

### Encoders
**Status:**  ⏳ Planned
#### Word Fields
*  [x] Label setter
*  [x] SDI setter
*  [x] Data setter
*  [x] SSM setter
#### BNR
*  [x] Float → BNR
#### BCD
*  [ ] Integer → Packed BCD
#### Discrete
*  [ ] Bit packing
#### Parity
*  [ ] a429_apply_parity()

--------------------------------------------------------------------------------

### Label Dictionary
**Status:**  ⏳ Planned
#### Descriptor
*  [ ] Label
*  [ ] Name
*  [ ] Encoding
*  [ ] Bit width
*  [ ] Resolution
*  [ ] Offset
*  [ ] Unit
#### API
*  [ ] a429_find_label()

--------------------------------------------------------------------------------

### High-Level API
**Status:**  ⏳ Planned
*  [ ] Automatic decoder
*  [ ] Automatic encoder
*  [ ] SDI filtering and matching (a429_match_sdi)
*  [ ] Human-readable output
*  [ ] Error reporting

--------------------------------------------------------------------------------

### Testing
**Status:**  ⏳ Planned
#### Core
*  [ ] Word API
*  [x] Parity
*  [ ] Bit reversal
#### Decoder
*  [ ] BNR
*  [ ] BCD
*  [ ] Discrete
*  [ ] SSM
#### Encoder
*  [ ] BNR
*  [ ] BCD
*  [ ] Discrete
*  [ ] Parity generation
#### Integration
*  [ ] Encode → Decode
*  [ ] Decode → Encode
*  [ ] Hardware compatibility
*  [ ] Reference ARINC vectors
#### Edge Cases & Error Handling
*  [ ] Invalid BCD digits handling (Hex A-F limits)
*  [ ] BNR out-of-bounds limits (exceeding Scale Factor)
*  [ ] Null word / Empty bus handling

--------------------------------------------------------------------------------

### Williamsburg Protocol (Optional)
**Status:**  ⏳ Planned
*  [ ] RTS
*  [ ] CTS
*  [ ] ACK
*  [ ] SOT
*  [ ] EOT
*  [ ] State machine
*  [ ] Block transfer

--------------------------------------------------------------------------------

### Future Ideas
*  [ ] Pretty printer
*  [ ] CSV label importer
*  [ ] JSON label importer
*  [ ] YAML label importer
*  [ ] Signal monitor
*  [ ] Logging utilities
*  [ ] PCAP export
*  [ ] Benchmark suite
*  [ ] Documentation website