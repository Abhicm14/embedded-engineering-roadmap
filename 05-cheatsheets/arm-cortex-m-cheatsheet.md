# 🦾 ARM Cortex-M Architecture & NVIC Cheatsheet

> Quick reference for ARMv7-M (Cortex-M3/M4/M7) core registers, vector table layout, NVIC priority registers, and HardFault triage.

---

## 1. Core CPU Register Set

| Register | Name | Function |
| :--- | :--- | :--- |
| **`R0 - R3`** | Argument / Scratch | Function call parameters and return values; caller-saved. |
| **`R4 - R11`**| General Purpose | Preserved across function calls; callee-saved. |
| **`R12` (IP)** | Intra-Procedure | Scratch register used by linker veneers; caller-saved. |
| **`R13` (SP)** | Stack Pointer | Banked as `MSP` (Main Stack Pointer) and `PSP` (Process Stack Pointer). |
| **`R14` (LR)** | Link Register | Holds return address or `EXC_RETURN` value during interrupts. |
| **`R15` (PC)** | Program Counter | Address of current instruction + 4 (Thumb mode bit 0 always 1). |
| **`xPSR`** | Program Status | Combined APSR (flags: N, Z, C, V), IPSR (current exception #), and EPSR (Thumb state). |
| **`PRIMASK`** | Priority Mask | 1-bit: When set to 1, disables all interrupts with configurable priority. |
| **`BASEPRI`** | Base Priority Mask | Disables interrupts with priority level equal to or lower than the value. |

---

## 2. Vector Table Layout (Base Address: `0x00000000` or `VTOR`)

| Offset | Vector Type | Exception # | Description |
| :---: | :--- | :---: | :--- |
| `0x00` | Initial `MSP` | - | Top of stack memory address (e.g. `0x20020000`) |
| `0x04` | Reset Handler | 1 | Entry point upon power-on or system reset |
| `0x08` | NMI Handler | 2 | Non-Maskable Interrupt (clock security, parity failure) |
| `0x0C` | HardFault Handler | 3 | Fatal hardware/software exception |
| `0x10` | MemManage Handler | 4 | MPU memory permission violation |
| `0x14` | BusFault Handler | 5 | Memory bus error (AHB/APB abort, unmapped address) |
| `0x18` | UsageFault Handler | 6 | Undefined instruction, unaligned access, div-by-zero |
| `0x1C - 0x2B` | *Reserved* | 7-10 | Reserved by ARM |
| `0x2C` | SVCall Handler | 11 | Supervisor Call (software interrupt used by OS) |
| `0x30` | Debug Monitor | 12 | Debug breakpoint/watchpoint exception |
| `0x34` | *Reserved* | 13 | Reserved |
| `0x38` | PendSV Handler | 14 | Pendable Service Request (used for RTOS context switch) |
| `0x3C` | SysTick Handler | 15 | System 24-bit periodic timer tick |
| `0x40+`| External IRQ 0..N | 16+ | Vendor peripheral interrupts (UART, SPI, Timers, DMA) |

---

## 3. Nested Vectored Interrupt Controller (NVIC) Registers

All NVIC registers reside in the System Control Space (`0xE000E000` - `0xE000EFFF`):

- **`NVIC->ISER[0..7]` (Interrupt Set-Enable):** Write 1 to bit $N$ to enable IRQ $N$.
- **`NVIC->ICER[0..7]` (Interrupt Clear-Enable):** Write 1 to bit $N$ to disable IRQ $N$.
- **`NVIC->ISPR[0..7]` (Interrupt Set-Pending):** Write 1 to bit $N$ to manually trigger IRQ $N$.
- **`NVIC->ICPR[0..7]` (Interrupt Clear-Pending):** Write 1 to bit $N$ to clear pending status.
- **`NVIC->IABR[0..7]` (Interrupt Active Bit):** Bit $N$ is 1 if IRQ $N$ is currently executing.
- **`NVIC->IP[0..239]` (Interrupt Priority):** 8-bit priority byte per IRQ (upper 4 bits typically implemented on STM32).

---

## 4. HardFault Diagnostic Triage

When inside a `HardFault_Handler`, check the System Control Block (SCB) status registers:

```
Address: 0xE000ED28  ->  SCB->CFSR (Configurable Fault Status Register, 32-bit)
   ├── [31:16] UsageFault Status (DIVBYZERO, UNALIGNED, UNDEFINSTR)
   ├── [15:8]  BusFault Status (PRECISERR, IMPRECISERR, BFARVALID)
   └── [7:0]   MemManage Status (IACCVIOL, DACCVIOL, MMARVALID)

Address: 0xE000ED38  ->  SCB->BFAR (BusFault Address Register)
   └── Contains the exact memory address that caused the memory bus abort.
```

### Stacking on Exception Entry:
The hardware automatically pushes 8 registers onto the active stack (`MSP` or `PSP`) before jumping to the fault handler:
`[SP+0]: R0` | `[SP+4]: R1` | `[SP+8]: R2` | `[SP+12]: R3` | `[SP+16]: R12` | `[SP+20]: LR` | `[SP+24]: PC (Crashing instruction)` | `[SP+28]: xPSR`
