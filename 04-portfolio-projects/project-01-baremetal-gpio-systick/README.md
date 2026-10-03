# 📁 Project 01: Bare-Metal GPIO & SysTick FSM Driver

> Target: STM32F401 / STM32F411 (ARM Cortex-M4)  
> Language: Pure Embedded C (C99) without ST HAL, LL, or CMSIS libraries  
> Build System: GNU Arm Toolchain (`arm-none-eabi-gcc`) + Make

---

## 🎯 Project Overview

This project demonstrates the bedrock foundation of embedded systems engineering: controlling hardware directly through memory-mapped silicon registers without reliance on vendor abstraction libraries.

You will:
1. Write a custom ARM Cortex-M vector table and C runtime startup file (`startup.c`).
2. Author a linker script (`linker.ld`) mapping Flash (`0x08000000`) and SRAM (`0x20000000`).
3. Create a hardware register definition header (`stm32f401xe.h`) using C pointers and structs.
4. Configure the STM32 Reset & Clock Control (RCC) to enable the GPIO bus.
5. Initialize the 24-bit ARM Cortex-M SysTick system timer for millisecond timekeeping.
6. Implement a non-blocking Finite State Machine (FSM) to debounce physical pushbuttons and toggle user LEDs.

---

## 🔌 Hardware Setup & Wiring

| STM32 Pin | Peripheral / Signal | Connection Target | Notes |
| :--- | :--- | :--- | :--- |
| **PC13** | Onboard User LED | Active LOW LED | Internal pull-up to $V_{DD}$ on Black Pill |
| **PA0** | User Pushbutton | External Pushbutton to GND | Configured with internal pull-up (`PUPDR`) |
| **PA13 / PA14** | SWDIO / SWCLK | ST-Link V2 Debugger | Flashing & GDB debugging |
| **GND / 3.3V** | Power | ST-Link V2 or USB-C | Clean 3.3V DC supply |

---

## 🧱 Software Architecture

```
                    ┌────────────────────────────┐
                    │       Power-On Reset       │
                    └─────────────┬──────────────┘
                                  ▼
                    ┌────────────────────────────┐
                    │      startup.c: Reset      │
                    │   - Copy .data Flash->RAM  │
                    │   - Zero out .bss in RAM   │
                    └─────────────┬──────────────┘
                                  ▼
                    ┌────────────────────────────┐
                    │     main(): Hardware Init  │
                    │   - RCC GPIOA/GPIOC Clocks │
                    │   - SysTick 1ms Periodic   │
                    └─────────────┬──────────────┘
                                  ▼
               ┌───────────────────────────────────────┐
               │         Superloop (1ms Tick)          │
               │  ┌─────────────────────────────────┐  │
               │  │ Button FSM:                     │  │
               │  │ RELEASED -> PRESS_DETECTED      │  │
               │  │ -> CONFIRMED (Toggle LED)       │  │
               │  │ -> RELEASE_DETECTED             │  │
               │  └─────────────────────────────────┘  │
               └───────────────────────────────────────┘
```

---

## 🛠️ Building & Flashing

```bash
# 1. Compile and link the bare-metal binary
make

# 2. Inspect memory footprint
arm-none-eabi-size build/firmware.elf

# 3. Flash to target board via OpenOCD
make flash
```
