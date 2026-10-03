# ⚡ Vector Table & Nested Vectored Interrupt Controller (NVIC)

> How ARM Cortex-M prioritizes, preempts, stacks, and dispatches hardware interrupts.

---

## 1. The Vector Table

The Vector Table is an array of 32-bit function pointer words located at address `0x00000000` (or relocated via the Vector Table Offset Register `VTOR` in bootloaders):

```c
__attribute__((section(".isr_vector"), used))
const uint32_t g_pfnVectors[] = {
    (uint32_t)&_estack,          /* 0x00: Initial Main Stack Pointer */
    (uint32_t)Reset_Handler,     /* 0x04: Reset Handler */
    (uint32_t)NMI_Handler,       /* 0x08: Non-Maskable Interrupt */
    (uint32_t)HardFault_Handler, /* 0x0C: HardFault Exception */
    /* ... System exceptions ... */
    (uint32_t)SysTick_Handler,   /* 0x3C: 24-bit System Tick */
    (uint32_t)WWDG_IRQHandler,   /* 0x40: Window Watchdog (IRQ 0) */
    (uint32_t)USART1_IRQHandler, /* ... Peripheral IRQs ... */
};
```

---

## 2. Automatic Hardware Stacking

When an interrupt fires, the Cortex-M hardware **automatically pushes 8 registers** onto the current stack in 12 clock cycles before entering the ISR:

```
Stack Frame pushed by Hardware:
  SP + 0x00 : R0
  SP + 0x04 : R1
  SP + 0x08 : R2
  SP + 0x0C : R3
  SP + 0x10 : R12
  SP + 0x14 : LR (Link Register - previous function return)
  SP + 0x18 : PC (Program Counter - address where code was interrupted)
  SP + 0x1C : xPSR (Status flags)
```

The CPU then loads `LR` with a special **`EXC_RETURN` value** (e.g. `0xFFFFFFF9` or `0xFFFFFFFD`). When the ISR returns via standard `BX LR`, the hardware detects this special value, restores the 8 stacked registers, and resumes the interrupted code with zero software overhead!

---

## 3. NVIC Priority Grouping

Interrupt priorities in ARM Cortex-M are divided into:
1. **Preemption Priority:** Determines whether an incoming interrupt can interrupt (preempt) an currently executing ISR.
2. **Subpriority:** Resolves ties when two interrupts with identical preemption priority pend simultaneously.

> ⚠️ **ARM Inverted Priority Rule:** In ARM Cortex-M NVIC, **lower numerical value means higher priority!** Priority `0` is the highest possible interrupt priority; Priority `15` is the lowest.
