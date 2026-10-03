# 💻 Comprehensive Software Taxonomy for Embedded Systems

> Broad taxonomy tree of software domains, languages, tools, stacks, and standards (adapted from Roadmap B taxonomy).

---

## 1. Programming Languages

- **Embedded C (C99 / C11):** The gold standard of embedded firmware. Fixed-width types (`stdint.h`), memory-mapped pointers, volatile qualifier, bitwise manipulation, assembly inlining, deterministic execution, and MISRA-C compliance.
- **Embedded C++ (C++17 / C++20):** Type-safe hardware abstractions, zero-cost abstractions, `constexpr` compile-time evaluation, templates, avoiding dynamic allocation (`new`/`delete`), and disabling RTTI/exceptions (`-fno-exceptions -fno-rtti`).
- **Assembly Language (ARM Thumb-2 / RISC-V):** Core register manipulation, vector table setup, reset sequence, startup code, low-level context switching routines, and performance-critical DSP inner loops.
- **Python (MicroPython & CircuitPython):** Rapid hardware prototyping, test automation, benchtop scripting, and running high-level interpreted logic on ESP32, RP2040, and STM32.
- **Embedded Rust (`no_std`):** Memory safety without garbage collection, compile-time race condition elimination, peripheral access crates (PAC), and the `embedded-hal` ecosystem.
- **Zig:** Emerging systems language with compile-time code execution (`comptime`), manual memory control, and seamless C header integration without a C compiler runtime.

---

## 2. Programming Fundamentals & Algorithms

- **Discrete Math & Logic:** Number bases (Binary, Octal, Hexadecimal), two's complement, Boolean algebra, floating-point IEEE-754 representation.
- **Embedded Data Structures:** Ring buffers (circular queues), intrusive linked lists, lookup tables, bit arrays, priority heaps for event schedulers.
- **Design Patterns in C:** Opaque pointers (Pimpl idiom for encapsulation), active objects, publish-subscribe event brokers, callback jump tables.
- **State Machines:** Hierarchical state machines (HSM), Moore vs. Mealy machines, state-transition tables.
- **Memory Management:** Memory pools, static block allocators, stack watermarking, zero-copy buffer passing.

---

## 3. Microcontroller Silicon Families & Peripherals

- **Architectures & Families:**
  - Microchip AVR (ATmega328P, ATtiny) & PIC
  - STMicroelectronics STM32 (ARM Cortex-M0/M3/M4/M7/M33)
  - Texas Instruments MSP430 (Ultra-low power 16-bit RISC)
  - Nordic Semiconductor nRF52/nRF53 (BLE, 2.4GHz RF, Cortex-M4/M33)
  - Espressif ESP32, ESP32-S3, ESP32-C3 (Wi-Fi, Bluetooth, Xtensa & RISC-V)
  - Raspberry Pi RP2040 (Dual-core Cortex-M0+ with programmable I/O - PIO)
- **Standard Silicon Subsystems:**
  - GPIO, ADC (SAR & Sigma-Delta), DAC, Analog Comparators
  - Timers, Input Capture, Output Compare, PWM, Real-Time Clock (RTC)
  - Watchdogs (Independent IWDG and Window WWDG)
  - Nested Vectored Interrupt Controller (NVIC)
  - Direct Memory Access (DMA) controllers
  - Clock generation: Crystal oscillators (HSE/LSE), RC oscillators (HSI/LSI), Phase-Locked Loops (PLL)
  - Power management: Run, Sleep, Stop, Standby modes
  - Bootloader & Device Firmware Upgrade (DFU over USB/UART/CAN)

---

## 4. Communication Interfaces & Protocol Stacks

- **Board-Level Buses:** UART/USART, I2C, SPI, SDIO, I3C, 1-Wire.
- **Digital Audio & Multimedia:** I2S, PCM, PDM microphones, SPDIF.
- **Display & Camera Interfaces:** MIPI CSI-2 (Camera), MIPI DSI (Display), parallel 8080/6800 interface, SPI ST7789/ILI9341, HDMI.
- **Wireless Networking:** Bluetooth Low Energy (BLE 5.x GAP/GATT/Mesh), Wi-Fi (802.11 b/g/n), LoRa & LoRaWAN, Zigbee, Thread, Matter.
- **Industrial & Fieldbuses:** RS-485, Modbus RTU/TCP, EtherCAT, Profibus.
- **High-Speed & Automotive:** USB 2.0/3.0 (Device/Host/OTG, CDC, HID, MSC), Ethernet (10/100/1000 with RMII/MII PHY), CAN 2.0A/B, CAN-FD, LIN, FlexRay, Automotive Ethernet.
- **Networking Stacks:** TCP/IP, UDP, LwIP, DHCP, DNS, SNTP, HTTP/REST, WebSocket, MQTT, CoAP.
- **Cellular IoT:** GSM/GPRS, LTE Cat-M1, NB-IoT with AT commands and PPP.

---

## 5. Storage, Filesystems & Memory

- **Non-Volatile Flash:** NOR Flash (byte-addressable reads), NAND Flash (block-based reads/writes), eMMC, SD Cards.
- **Embedded Filesystems:** LittleFS (power-resilient for microcontrollers), SPIFFS, FATFS (for SD cards), UBIFS / JFFS2 (for Linux MTD).
- **RAM Technologies:** Internal SRAM, Core-Coupled Memory (CCM), External PSRAM, SDRAM.

---

## 6. Simulation & Emulation Tools

- **Web Simulators:** Wokwi (STM32, ESP32, Pico, Arduino with interactive sensors and logic analyzers).
- **Desktop Simulators:** SimulIDE (circuit simulation + MCU code execution).
- **System Emulators:** QEMU (ARM Cortex-M and Cortex-A system emulation).
- **Distributed Simulators:** Renode by Antmicro (multi-node virtual networks).

---

## 7. Digital Signal Processing & Control Systems

- **DSP Algorithms:** Finite Impulse Response (FIR) filters, Infinite Impulse Response (IIR) filters, Fast Fourier Transform (FFT), convolution, windowing functions.
- **CMSIS-DSP:** Optimized ARM assembly DSP functions using SIMD instructions.
- **Control Theory:** Proportional-Integral-Derivative (PID) control, anti-windup, Kalman filtering, state-space control, MATLAB/Simulink code generation.

---

## 8. Operating Systems & Real-Time Kernels

- **RTOS Engines:** FreeRTOS, Zephyr RTOS, RT-Thread, Apache NuttX, Arm Mbed OS, Eclipse ThreadX (Azure RTOS), QNX Neutrino, VxWorks, $\mu\text{C/OS-III}$.
- **Embedded Linux Subsystems:** Linux Kernel internals, Character Drivers, Platform Drivers, Device Trees, U-Boot bootloader, BusyBox, Buildroot, Yocto Project (Poky), `libgpiod`, V4L2, IIO.

---

## 9. Debugging, Build & Developer Tooling

- **Debug Hardware & Protocols:** JTAG, SWD, ETM trace, OpenOCD, J-Link GDB Server, ST-Link, Black Magic Probe.
- **Command-Line & Build:** GNU Arm Embedded Toolchain (`arm-none-eabi-*`), LLVM/Clang, Make, CMake, Ninja, Bash scripting, Docker containers for reproducible builds.

---

## 10. Software Development Lifecycle & Verification

- **Process Methodologies:** V-Model (standard in safety-critical systems), Agile/Scrum.
- **Version Control:** Git, GitLab, GitHub Actions, semantic versioning.
- **Testing:** Test-Driven Development (TDD), Unit testing (Unity, CMock, Ceedling, GoogleTest), Hardware-in-the-Loop (HIL), Software-in-the-Loop (SIL).
- **Industry Standards:** MISRA-C:2012, ISO 26262 (Automotive ASIL), DO-178C (Avionics), IEC 62304 (Medical Device Software), IEC 62443 (Industrial Cybersecurity).

---

## 11. Security, Graphical Interfaces & Specializations

- **Embedded Security:** Cryptographic primitives (AES, RSA, ECC, SHA-256), TLS 1.3 (`mbedTLS`), Secure Boot, ARM TrustZone, secure elements (ATECC608A), anti-tamper.
- **Embedded GUI:** LVGL (Light and Versatile Graphics Library), TouchGFX, Qt for Embedded Linux.
- **IoT & TinyML:** MQTT/CoAP telemetry, AWS IoT Core / Azure IoT, TensorFlow Lite for Microcontrollers, Edge Impulse, CMSIS-NN quantized inferencing.
- **AUTOSAR:** Classic Platform (MCAL, BSW, RTE, SWC) and Adaptive Platform.
