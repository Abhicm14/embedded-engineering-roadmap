# 🔵 Month 2: Microcontroller Architecture & Bare-Metal STM32

> Focus: ARM Cortex-M4 internals, vector tables, linker scripts, Makefiles, RCC clock trees, and pure register programming.

---

## 🎯 Monthly Objectives
1. Understand the ARM Cortex-M architecture, core registers, and privileged execution modes.
2. Write a bare-metal C runtime startup file and vector table from scratch without ST HAL or CMSIS.
3. Author a custom GNU linker script (`.ld`) mapping Flash and SRAM memory sections.
4. Configure the STM32 Phase-Locked Loop (PLL) and peripheral clock gating in the RCC.
5. Complete **Project 1: Bare-Metal GPIO & SysTick FSM Driver**.

---

## 📅 Weekly Breakdown

### Week 5: Cortex-M Architecture & Vector Table
- Core registers (`R0-R12`, `SP`, `LR`, `PC`, `xPSR`).
- The vector table, reset vector, and boot sequence.
- Lab: Write `startup.c` with `.data` copy loop and `.bss` zeroing loop.

### Week 6: Toolchains, Linker Scripts & Makefiles
- The GNU Arm Toolchain (`arm-none-eabi-gcc`, `objdump`, `size`, `objcopy`).
- Linker script commands: `MEMORY`, `SECTIONS`, symbols, and location counter (`.`).
- Lab: Create a modular Makefile compiling code to `.bin` and `.hex`.

### Week 7: Memory-Mapped I/O & Clock Trees (RCC)
- AHB and APB bus matrices, prescalers, and wait-states.
- Memory-mapped structs and peripheral base addresses.
- Lab: Initialize system clock to 84 MHz / 100 MHz using the internal PLL.

### Week 8: Bare-Metal GPIO & Project 1 Milestone
- GPIO registers: `MODER`, `OTYPER`, `OSPEEDR`, `PUPDR`, `BSRR`.
- SysTick 24-bit timer configuration.
- Lab: Deliver [Project 1](../../04-portfolio-projects/project-01-baremetal-gpio-systick/).
