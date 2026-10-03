# 🦾 ARM Cortex-M Core Registers & Execution Model

> The internal execution pipeline, CPU registers, stack pointers, and privilege levels of ARMv7-M (Cortex-M3/M4/M7).

---

## 1. Core CPU Registers

The ARM Cortex-M core contains 16 primary 32-bit registers plus special status and control registers:

```
R0  - R3   : Argument and return value registers (Caller-saved)
R4  - R11  : General purpose computational registers (Callee-saved)
R12 (IP)   : Intra-procedure scratch register
R13 (SP)   : Stack Pointer (Banked as MSP and PSP)
R14 (LR)   : Link Register (Holds function return address or EXC_RETURN)
R15 (PC)   : Program Counter (Current instruction address + 4)
xPSR       : Combined Program Status Register (APSR, IPSR, EPSR)
PRIMASK    : Disables all interrupts with configurable priority
BASEPRI    : Masks interrupts with priority >= specified threshold
CONTROL    : Selects active stack pointer (MSP/PSP) and privilege level
```

---

## 2. Operating Modes & Privilege Levels

The Cortex-M architecture provides two operating modes and two privilege levels to isolate operating systems from user application code:

| Operating Mode | Execution Privilege | Active Stack Pointer | Typical Usage |
| :--- | :--- | :--- | :--- |
| **Handler Mode** | Always Privileged | Main Stack Pointer (`MSP`) | Interrupt Service Routines (ISRs) and CPU exception handlers. |
| **Thread Mode**  | Privileged or Unprivileged | `MSP` or `PSP` | Main application code or RTOS user tasks. |

In an RTOS environment:
- The RTOS kernel and ISRs execute in **Handler Mode** using `MSP`.
- User tasks execute in **Thread Mode** using their own private `PSP` (Process Stack Pointer). If a task overflows its stack, it corrupts only its own task area without crashing the kernel!

---

## 3. The Combined Program Status Register (`xPSR`)

- **APSR (Application PSR):** Condition code flags updated by ALU instructions:
  - `N`: Negative result flag.
  - `Z`: Zero result flag.
  - `C`: Carry / borrow flag.
  - `V`: Overflow flag.
- **IPSR (Interrupt PSR):** Holds the exception number currently being executed ($0 = \text{Thread mode}, 15 = \text{SysTick}, 16+ = \text{External IRQ}$).
- **EPSR (Execution PSR):** Contains the **Thumb state bit (T-bit)**. On Cortex-M, all instructions execute in 16-bit/32-bit Thumb-2 mode; bit 0 of jump targets must always be 1!
