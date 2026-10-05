# 🏛️ Broad Embedded Systems Topic Taxonomy

> Reference taxonomy covering the complete breadth of the embedded engineering discipline organized under the three pillars of **SOFTWARE**, **HARDWARE**, and **SOFT SKILLS**.

---

## 🧭 Taxonomy Directory

| Pillar Reference | Coverage |
| :--- | :--- |
| [**💻 Software Reference (`software.md`)**](software.md) | Programming languages (C, C++, Assembly, Python/MicroPython, Rust, Zig); Algorithms & Data Structures; Microcontroller families (AVR, PIC, STM32, MSP430, nRF, ESP32, RP2040); Interfaces & Protocols (UART, I2C, SPI, CAN, USB, Ethernet, BLE, Wi-Fi, LoRa, cellular); Memory & filesystems (LittleFS, FAT); RTOS & Linux; Testing, CI/CD, MISRA, and Security. |
| [**🔌 Hardware Reference (`hardware.md`)**](hardware.md) | Circuits, electronics fundamentals, digital design, computer architecture (ARM, RISC-V), test equipment (DMM, Scope, Logic Analyzer), breadboarding, PCB design (KiCad), soldering/rework, and FPGA development (Verilog/VHDL, Nandland). |
| [**🧠 Soft Skills Reference (`soft-skills.md`)**](soft-skills.md) | Technical communication, cross-functional collaboration, systematic debugging mindset, career growth, and technical interview preparation. |

---

## 🔬 Silicon Register Walkthroughs & Cross-Architecture Index

Master the exact "open the manual and configure every register by hand" bare-metal workflow across all major microcontrollers:

* [**🗺️ GPIO Registers Across Microcontrollers (`gpio-registers-across-mcus.md`)**](gpio-registers-across-mcus.md): Comprehensive side-by-side comparison matrix (STM32, PIC, AVR, ESP32) mapping concepts to real silicon registers.
* [**⚡ AVR ATmega328P / Arduino Uno Walkthrough (`gpio-bare-metal-walkthrough-avr.md`)**](gpio-bare-metal-walkthrough-avr.md): Direct 1-cycle register access (`DDRB`, `PORTB`, `PINB`) on PB5 (Pin 13).
* [**📶 Espressif ESP32 Walkthrough (`gpio-bare-metal-walkthrough-esp32.md`)**](gpio-bare-metal-walkthrough-esp32.md): The IO_MUX pad router, GPIO Matrix crossbar, and atomic W1TS/W1TC registers on GPIO2.
* [**🦾 STM32F4 (ARM Cortex-M4) Walkthrough (`../03-stm32-microcontrollers/gpio-bare-metal-walkthrough.md`)**](../03-stm32-microcontrollers/gpio-bare-metal-walkthrough.md): AHB1 bus clock gating, MODER, OTYPER, and atomic BSRR on PB12.
* [**🔌 Microchip PIC16F877A Walkthrough (`../../pic-mplab-xc8/gpio-bare-metal-walkthrough.md`)**](../../pic-mplab-xc8/gpio-bare-metal-walkthrough.md): Banked memory, inverted TRIS direction, and the Read-Modify-Write trap on RB0.
