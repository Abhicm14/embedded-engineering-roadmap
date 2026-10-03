# 🛠️ Tools & Software for Embedded Engineering

> Emulators, simulators, cross-compilers, debuggers, and protocol analyzers (👶 Beginner-friendly, 💎 In-depth reference).

---

## 1. Hardware Simulators & Virtual Environments

- 🔗 👶 [**Wokwi**](https://wokwi.com/) *(Web / VS Code Extension)*  
  *Platform:* Browser-based online simulator for STM32, ESP32, Raspberry Pi Pico, and Arduino. Supports interactive wiring with OLEDs, sensors (BMP280, MPU6050), pushbuttons, and a virtual 8-channel logic analyzer. Zero setup required.
- 🔗 👶 [**SimulIDE**](https://www.simulide.com/) *(Cross-Platform)*  
  *Platform:* Real-time electronic circuit simulator with integrated microcontroller simulation (AVR, PIC, Arduino) and virtual oscilloscopes.
- 🔗 💎 [**QEMU (Quick Emulator)**](https://www.qemu.org/) *(Cross-Platform CLI)*  
  *Platform:* Industry-standard full-system machine emulator. Emulates ARM Cortex-M3/M4 microcontrollers and ARM Cortex-A processors running complete Embedded Linux distributions.
- 🔗 💎 [**Renode**](https://renode.io/) *(Antmicro)*  
  *Platform:* Advanced multi-node framework for simulating distributed embedded networks (e.g. multiple wireless STM32/ESP32 nodes communicating via BLE or CAN simultaneously).

---

## 2. Toolchains & Compilers

- 🔗 💎 [**GNU Arm Embedded Toolchain (`arm-none-eabi-*`)**](https://developer.arm.com/downloads/-/gnu-rm)  
  *Platform:* The standard free GCC C/C++ cross-compiler, assembler, linker, and GDB debugger for ARM Cortex-M/R cores.
- 🔗 💎 [**LLVM / Clang for Embedded**](https://clang.llvm.org/)  
  *Platform:* Modern modular compiler infrastructure providing advanced static analysis, sanitizers, and fast compile times.

---

## 3. Flashing & Hardware Debugging Utilities

- 🔗 💎 [**OpenOCD (Open On-Chip Debugger)**](https://openocd.org/)  
  *Platform:* Open-source SWD and JTAG server connecting GDB to microcontrollers via ST-Link, J-Link, or CMSIS-DAP adapters.
- 🔗 👶 [**STM32CubeProgrammer**](https://www.st.com/en/development-tools/stm32cubeprog.html)  
  *Platform:* Official graphical and CLI utility from STMicroelectronics for flashing Flash, setting option bytes, configuring read-out protection (RDP), and recovering locked boards.
- 🔗 💎 [**SEGGER J-Link Software & RTT**](https://www.segger.com/products/debug-probes/j-link/technology/about-real-time-transfer/)  
  *Platform:* High-speed flashing and Real-Time Transfer (RTT) for bidirectional terminal logging at $> 1\text{ MB/s}$ without CPU cycle stealing.

---

## 4. Signal Analysis & Protocol Sniffers

- 🔗 👶 [**PulseView / sigrok**](https://sigrok.org/wiki/PulseView)  
  *Platform:* Open-source GUI for digital logic analyzers. Decodes 100+ protocols (UART, SPI, I2C, CAN, USB, 1-Wire) in real-time.
- 🔗 💎 [**Wireshark**](https://www.wireshark.org/)  
  *Platform:* Packet analyzer for Ethernet, TCP/IP, MQTT, CoAP, and Bluetooth HCI packet streams.
- 🔗 👶 [**Serial Studio**](https://serial-studio.github.io/)  
  *Platform:* Multi-channel data dashboard for plotting live telemetry received over UART from microcontrollers.

---

## 5. Microchip PIC Ecosystem & Toolchains

- 🔗 👶 [**MPLAB X IDE**](https://www.microchip.com/mplab/mplab-x-ide) *(Microchip)*  
  *Platform:* Integrated development environment based on NetBeans for 8-bit, 16-bit, and 32-bit PIC and AVR microcontrollers.
- 🔗 💎 [**MPLAB XC8 C Compiler**](https://www.microchip.com/mplab/compilers) *(Microchip)*  
  *Platform:* Highly optimizing ANSI C compiler for 8-bit PIC devices (PIC10/12/16/18) with free license tier for students.
- 🔗 👶 [**PICSimLab (PIC Simulator Laboratory)**](https://picsimlab.sourceforge.net/) *(Open Source)*  
  *Platform:* Real-time hardware emulator supporting PIC16F877A, PIC18F4550, and Arduino boards with interactive breadboards, LCDs, keypads, potentiometers, and virtual serial loopbacks.
- 🔗 👶 [**MPLAB IPE (Integrated Programming Environment)**](https://www.microchip.com/mplab/mplab-integrated-programming-environment)  
  *Platform:* Dedicated standalone production programmer GUI for PICkit 3, PICkit 4, and ICD 4 hardware programmers.
