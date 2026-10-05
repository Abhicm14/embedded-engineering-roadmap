# 🦾 STM32 ARM Cortex-M4 Custom Workspace

> **Silicon Platform:** STMicroelectronics STM32F4 (ARM Cortex-M4 with FPU, 84/100 MHz, 256/512 KB Flash)  
> **Simulation First:** [**Wokwi STM32 Simulator**](https://wokwi.com/) / [**QEMU ARM Cortex-M**](https://www.qemu.org/)

---

## 🎯 Architecture to Master Before Purchasing Hardware

Before buying a physical STM32 Black Pill or Nucleo board, master these core ARM Cortex-M4 concepts in software simulation:

1. **Bare-Metal Boot Sequence (No HAL / No CubeMX):**
   - Writing your own `startup.c` with the Interrupt Vector Table (`g_pfnVectors`).
   - Writing your own Linker Script (`stm32f4.ld`) defining `MEMORY { FLASH, RAM }` and `SECTIONS { .text, .rodata, .data, .bss }`.
   - Initializing `.data` (copying from Flash to RAM) and zeroing `.bss` in your `Reset_Handler`.
2. **Clock Tree & Power Configuration:**
   - Enabling peripheral buses via Reset and Clock Control (`RCC_AHB1ENR`, `RCC_APB1ENR`, `RCC_APB2ENR`).
   - Switching from internal 16 MHz HSI oscillator to Phase-Locked Loop (PLL) for maximum clock speed.
   - Configuring Flash Latency wait-states before increasing CPU core clock.
3. **Hardware Registers & Atomic Bit Manipulation:**
   - Atomic pin toggling with Bit Set/Reset Register (`GPIOx->BSRR`).
   - Configuring pin modes via `GPIOx->MODER` and speed via `GPIOx->OSPEEDR`.
4. **Nested Vectored Interrupt Controller (NVIC):**
   - Setting interrupt priority levels and enabling IRQs in the core NVIC.
   - Writing deterministic, non-blocking ISR handlers.

---

## 💻 Simulation-First Workflow for STM32

You do **not** need physical hardware to learn and verify 90% of STM32 bare-metal firmware:
1. **Wokwi STM32 Simulation:**
   - Open Wokwi and simulate ARM Cortex-M microcontrollers interactively with LEDs, sensors, and displays.
2. **QEMU Simulation:**
   - Compile with `arm-none-eabi-gcc` and run directly on your PC:
     ```bash
     arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -T stm32f4.ld startup.c main.c -o firmware.elf
     qemu-system-arm -M netduinoplus2 -kernel firmware.elf -nographic
     ```
   - Connect GDB to QEMU via port 1234 (`arm-none-eabi-gdb firmware.elf -ex "target remote localhost:1234"`) and step through your assembly startup line by line!
3. **Renode Multi-Node Simulation:**
   - Run complex peripheral scripts simulating STM32 peripherals, DMA, and sensors.

---

## 📁 Source Code Directory
Place your custom STM32 bare-metal C drivers, linker scripts, and Makefiles in:
👉 [`src/`](src/)
