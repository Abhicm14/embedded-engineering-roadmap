# 🛠️ Simulators & Software Tools for Embedded Systems

> Quality-rated software tools, emulators, simulators, and protocol sniffers enabling high-efficiency embedded development with or without physical hardware.

---

## 💻 Hardware Simulators & Emulators

| Tool | Rating | Cost | Supported Targets | Why Recommended |
| :--- | :---: | :---: | :--- | :--- |
| **Wokwi**<br/>*(Web / VS Code)* | ★★★★★ | Free (Core) | STM32, ESP32, Raspberry Pi Pico, Arduino | Instant browser-based simulation of microcontrollers wired to real sensors, OLED displays, pushbuttons, and logic analyzers. Zero installation required. Supports GDB debugging. |
| **QEMU System Emulation**<br/>*(Cross-Platform)* | ★★★★★ | Free (Open Source) | ARM Cortex-M3/M4 (e.g. `lm3s6965evb`, `stm32vldiscovery`), Cortex-A (Linux) | Industry-standard machine emulator. Perfect for testing bare-metal boot code, RTOS scheduling, and cross-compiled Embedded Linux kernels in CI pipelines. |
| **Renode**<br/>*(Antmicro)* | ★★★★★ | Free (Open Source) | STM32, RISC-V, Nordic nRF52, multi-node networks | Sophisticated multi-node simulator capable of simulating entire distributed systems (e.g., multiple wireless nodes transmitting BLE or CAN packets simultaneously). |
| **Keil µVision Simulator**<br/>*(Windows)* | ★★★★☆ | Free (32KB Limit) | ARM Cortex-M0/M3/M4 | Comprehensive cycle-accurate CPU simulator with detailed register viewers, logic analyzers, and execution profiling. Great for learning ARM instruction timing. |

---

## 🔍 Protocol Sniffers & Logic Analysis

| Tool | Rating | Cost | Capabilities | Why Recommended |
| :--- | :---: | :---: | :--- | :--- |
| **PulseView / sigrok** | ★★★★★ | Free (Open Source) | Decodes 100+ protocols (UART, SPI, I2C, CAN, 1-Wire, DMX, USB) | The open-source standard for logic analysis. Works seamlessly with cheap $7 8-channel USB logic analyzers to visualize bus transactions and diagnose timing bugs. |
| **Wireshark** | ★★★★★ | Free (Open Source) | Ethernet, TCP/IP, MQTT, CoAP, BLE packet sniffing | Essential for IoT and networking projects. Inspect packet handshakes, TLS negotiation, and diagnose network frame errors. |
| **Saleae Logic Software** | ★★★★★ | Free (Viewer) | Advanced digital/analog decoding, custom protocol analyzers | Cleanest logic analyzer UI in the industry. The software can load and inspect captured trace files shared by teammates. |

---

## 🔌 Flashing & Debugging Utilities

| Tool | Rating | Cost | Capabilities | Why Recommended |
| :--- | :---: | :---: | :--- | :--- |
| **OpenOCD** | ★★★★★ | Free (Open Source) | On-Chip Debugging via SWD / JTAG | Connects GDB to STM32, ESP32, NXP, and RISC-V microcontrollers through ST-Link, J-Link, or CMSIS-DAP probes. |
| **STM32CubeProgrammer** | ★★★★★ | Free (ST) | Flash programming via SWD, UART, USB DFU | Essential for recovering bricked boards, setting option bytes, configuring read-out protection (RDP), and flashing raw binaries. |
| **J-Link Software Suite** | ★★★★★ | Free (with J-Link) | Ultra-fast flashing, RTT (Real-Time Terminal) | SEGGER RTT allows bidirectional logging to/from MCU at 1+ MB/s without blocking CPU execution or requiring a UART peripheral. |

---

## 📟 Serial Terminals

- **tio (CLI):** Extremely fast, minimalist serial terminal for Linux/macOS/WSL with automatic reconnection.
- **PuTTY / Tera Term (Windows):** Lightweight, reliable VT100 terminal emulators for viewing UART debug output and sending keystrokes.
- **Serial Studio (Cross-Platform):** Modern multi-channel data visualization tool for plotting real-time telemetry from microcontrollers over UART.
