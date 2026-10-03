# 💥 ARM Cortex-M Fault Status & HardFault Cheatsheet

---

## 1. System Control Block (SCB) Fault Registers

| Register | Address | Function |
| :--- | :---: | :--- |
| **`SCB->HFSR`** | `0xE000ED2C` | HardFault Status Register |
| **`SCB->CFSR`** | `0xE000ED28` | Configurable Fault Status Register (UFSR + BFSR + MMFSR) |
| **`SCB->BFAR`** | `0xE000ED38` | BusFault Address Register (Holds address that triggered bus abort) |
| **`SCB->MMFAR`**| `0xE000ED34` | MemManage Fault Address Register |

---

## 2. Configurable Fault Status Register (`CFSR`) Bitfield Breakdown

### `UsageFault` (Bits [31:16]):
- **Bit 25 (`DIVBYZERO`):** Integer divide by zero executed.
- **Bit 24 (`UNALIGNED`):** Unaligned memory access attempted.
- **Bit 18 (`NOCP`):** Coprocessor instruction attempted without enabling FPU in CPACR!
- **Bit 17 (`INVPC`):** Invalid `EXC_RETURN` loaded into PC.
- **Bit 16 (`UNDEFINSTR`):** CPU encountered an undefined opcode.

### `BusFault` (Bits [15:8]):
- **Bit 15 (`BFARVALID`):** If 1, the address in `SCB->BFAR` is valid!
- **Bit 9 (`PRECISERR`):** Precise data bus error (the exact instruction caused the fault).
- **Bit 10 (`IMPRECISERR`):** Imprecise data bus error (buffered write fault; PC has advanced).

### `MemManage` (Bits [7:0]):
- **Bit 7 (`MMARVALID`):** Address in `SCB->MMFAR` is valid.
- **Bit 1 (`DACCVIOL`):** Data access violation against MPU rules.
- **Bit 0 (`IACCVIOL`):** Instruction fetch violation from non-executable memory (`XN`).

---

## 3. Register Stack Frame Layout

On exception entry, hardware stacks 8 registers:

```
[SP + 0x00]: R0
[SP + 0x04]: R1
[SP + 0x08]: R2
[SP + 0x0C]: R3
[SP + 0x10]: R12
[SP + 0x14]: LR (Return address of function call)
[SP + 0x18]: PC (Crashing instruction address -> Feed into addr2line!)
[SP + 0x1C]: xPSR
```
