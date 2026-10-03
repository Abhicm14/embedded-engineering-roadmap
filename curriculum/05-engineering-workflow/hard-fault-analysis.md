# 💥 HardFault Diagnostic Triage & Map File Analysis

> How to diagnose ARM Cortex-M crashes, unstack hardware registers, and pinpoint the exact source code line.

---

## 1. What Triggers a HardFault?

On ARM Cortex-M microcontrollers, a `HardFault` occurs when an unhandled exception or critical violation occurs:
1. **Memory Bus Abort (`BusFault`):** Attempting to read or write to an invalid physical address (e.g. dereferencing a NULL pointer `0x00000000` or accessing a peripheral whose clock was not enabled in RCC!).
2. **Usage Violation (`UsageFault`):** Division by zero, unaligned memory access on Cortex-M0, or attempting to execute non-executable memory (`XN`).
3. **Memory Protection Unit (`MemManage`):** Violating MPU access rules.

---

## 2. HardFault Assembly Trap & Register Unstacking

In your firmware, implement an assembly stub in `startup.c` to identify which stack pointer (`MSP` or `PSP`) was active when the fault occurred:

```c
void HardFault_Handler(void) {
    __asm__ volatile (
        "tst lr, #4          \n" /* Check bit 2 of EXC_RETURN */
        "ite eq              \n"
        "mrseq r0, msp       \n" /* R0 = MSP if bit 2 is 0 */
        "mrsne r0, psp       \n" /* R0 = PSP if bit 2 is 1 */
        "b HardFault_Decoder \n" /* Pass stack pointer into C decoder */
    );
}

void HardFault_Decoder(uint32_t *stack_frame) {
    uint32_t r0  = stack_frame[0];
    uint32_t r1  = stack_frame[1];
    uint32_t r2  = stack_frame[2];
    uint32_t r3  = stack_frame[3];
    uint32_t r12 = stack_frame[4];
    uint32_t lr  = stack_frame[5];
    uint32_t pc  = stack_frame[6]; /* CRASHING INSTRUCTION ADDRESS! */
    uint32_t psr = stack_frame[7];

    /* SCB Fault Registers */
    volatile uint32_t cfsr = (*((volatile uint32_t *)0xE000ED28));
    volatile uint32_t bfar = (*((volatile uint32_t *)0xE000ED38));

    // Halt in infinite loop for GDB inspection:
    while (1) { __asm__ volatile ("bkpt #0"); }
}
```

---

## 3. Finding the Crashing Source Line

1. Note the value of `pc` from the unstacked frame (e.g. `pc = 0x0800042a`).
2. Run GNU `addr2line` from your terminal:
   ```bash
   arm-none-eabi-addr2line -e build/firmware.elf 0x0800042a
   # Output: /path/to/project/main.c:142
   ```
3. Line 142 is the exact C code line that caused the processor crash!
