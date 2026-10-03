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
