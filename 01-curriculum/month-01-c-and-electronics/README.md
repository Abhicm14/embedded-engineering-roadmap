# 🟢 Month 1: Advanced Embedded C & Electronics Foundations

> Focus: Mastering pointer arithmetic, memory layout, bitwise manipulation, circuit physics, and lab test instruments.

---

## 🎯 Monthly Objectives
1. Understand how C code maps directly to memory sections (`.text`, `.rodata`, `.data`, `.bss`, stack, heap).
2. Master bitwise operations, bit-masks, endianness byte-swapping, and the `volatile` qualifier.
3. Understand passive/active analog circuits (Ohm's law, pull-up resistors, RC timing, MOSFET switching).
4. Learn how to probe circuits using a Digital Multimeter (DMM) and open-source Logic Analyzer (PulseView).

---

## 📅 Weekly Breakdown

### Week 1: Bitwise Mastery & Integer Promotion
- Study bitwise operators (`&`, `|`, `^`, `~`, `<<`, `>>`).
- Build bitmask macros (`BIT`, `BIT_SET`, `BIT_CLEAR`, `BIT_TOGGLE`, `FIELD_WRITE`).
- Lab: Implement a virtual 32-bit register emulator and verify bit manipulations with unit tests.

### Week 2: Pointers, Memory Model & Type Qualifiers
- Pointers to hardware memory, function pointers, and generic buffers.
- Deep dive into `volatile`, `const`, and `restrict`.
- Lab: Build a lock-free circular ring buffer with pointers.

### Week 3: Electronics Physics for Firmware Engineers
- Resistors, capacitors (decoupling/bypass), diodes, and N-channel MOSFET switches.
- Calculating pull-up resistor values for $100\text{ kHz}$ and $400\text{ kHz}$ I2C lines.
- Lab: Wire up a push-pull button circuit and measure debounce voltage bouncing.

### Week 4: Lab Instruments & Probing
- Digital Multimeter testing (continuity, current draw).
- Logic Analyzer setup using open-source **PulseView** (sigrok).
- Lab: Capture and decode an asynchronous serial transmission at 115200 baud.
