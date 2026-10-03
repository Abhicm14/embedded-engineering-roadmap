# 🏛️ CPU Core & Memory Architecture

> The internal anatomy of embedded processors: Arithmetic Logic Unit (ALU), Program Counter, Stack Pointer, Bus Matrix, and Memory Hierarchies.

---

## 1. Microcontroller CPU Microarchitecture

```
┌─────────────────────────────────────────────────────────────┐
│                 CENTRAL PROCESSING UNIT (CPU)               │
│                                                             │
│  ┌───────────────────────┐       ┌───────────────────────┐  │
│  │ General Registers     │       │ Special Registers     │  │
│  │ R0 - R12              │       │ PC (Program Counter)  │  │
│  │                       │       │ SP (Stack Pointer)    │  │
│  │                       │       │ LR (Link Register)    │  │
│  │                       │       │ xPSR (Flags: N,Z,C,V) │  │
│  └───────────┬───────────┘       └───────────┬───────────┘  │
│              │                               │              │
│              ▼                               ▼              │
│       ┌──────────────┐              ┌─────────────────┐     │
│       │  ALU (Math)  │              │ Instruction     │     │
│       │  & FPU       │              │ Decoder         │     │
│       └──────┬───────┘              └────────┬────────┘     │
└──────────────┼───────────────────────────────┼──────────────┘
               │ Instruction & Data Buses      │
               ▼                               ▼
┌─────────────────────────────────────────────────────────────┐
│                      SYSTEM BUS MATRIX                      │
├──────────────────────────────┬──────────────────────────────┤
│ Flash Memory (Instruction)   │ SRAM Memory (Data)           │
│ Peripheral Bus (AHB / APB)   │ DMA Engine & Core Periphs    │
└──────────────────────────────┴──────────────────────────────┘
```

---

## 2. Core Execution Cycle: Fetch, Decode, Execute

1. **Fetch:** The CPU reads the instruction word pointed to by the **Program Counter (`PC`)** across the Instruction Bus (I-Bus) from Flash memory. The PC is automatically incremented.
2. **Decode:** The instruction decoder deciphers the opcode (e.g. `ADD R0, R1, #4` or `LDR R2, [R3]`).
3. **Execute:** The **ALU (Arithmetic Logic Unit)** performs the mathematical or logical operation, updates processor status flags (`N`, `Z`, `C`, `V` in `xPSR`), or performs a load/store over the Data Bus (D-Bus).

---

## 3. Memory Hierarchy in Embedded Systems

| Memory Type | Technology | Volatility | Typical Speed | Embedded Purpose |
| :--- | :--- | :--- | :--- | :--- |
| **CPU Registers** | Flip-Flops | Volatile | 1 clock cycle | Working variables and immediate computation operands. |
| **Cache (L1/L2)** | SRAM | Volatile | 1-2 clock cycles | Buffering frequently executed loops and lookup data. |
| **Internal SRAM**| CMOS SRAM | Volatile | 1 clock cycle | Application variables, stack frames, heap, and DMA buffers. |
| **Internal Flash**| NOR Flash | Non-Volatile | 2-5 cycles (wait-states) | Executable firmware code, vector table, constant tables. |
| **External Flash**| SPI / QSPI NOR | Non-Volatile | 10-50 cycles | Large assets, fonts, icons, OTA update download slots. |
| **EEPROM** | EEPROM | Non-Volatile | Very Slow (ms write) | Non-volatile device calibration, calibration constants. |

---

## 4. Endianness: Little-Endian vs Big-Endian

- **Little-Endian (ARM Cortex-M Default):** The Least Significant Byte (LSB) is stored at the lowest memory address.
  - Given 32-bit integer `0x12345678` stored at address `0x20000000`:
  - `Addr 0x20000000`: `0x78`
  - `Addr 0x20000001`: `0x56`
  - `Addr 0x20000002`: `0x34`
  - `Addr 0x20000003`: `0x12`
- **Big-Endian (Network Byte Order):** The Most Significant Byte (MSB) is stored at the lowest memory address (`0x12, 0x34, 0x56, 0x78`).
