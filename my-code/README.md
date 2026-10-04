# 🛠️ My Embedded Workspace (`my-code/`)

> **Your Personal Coding & Hardware Driver Repository**  
> *"The best way to learn embedded systems is not by reading or copying, but by writing every driver yourself directly from the silicon datasheet."*

---

## 🧭 Purpose of This Workspace

This directory is **your dedicated workspace** to write, organize, test, and save your own custom firmware drivers, applications, and experiments.

Unlike the reference examples in `code-examples/` (which serve as study material), the folders here are structured for **your own code creation**:
1. You can code drivers for your target boards from scratch.
2. You can track your personal progress as you advance from simple GPIO toggling to complex protocol stacks.
3. You can commit and push your work to your own GitHub fork or repository.

---

## 📁 Workspace Architecture

```
my-code/
├── pic16f877a/            # Microchip PIC16F877A Drivers (MPLAB X / XC8 / PICSimLab)
│   ├── README.md          # Peripheral checklist & building instructions
│   ├── templates/         # Header/Source starter skeletons with register TODOs
│   └── drivers/           # Place your completed .c and .h driver files here
├── arduino-mega2560/      # Arduino Mega 2560 (ATmega2560 8-bit AVR)
│   ├── README.md          # Register map, timer registers, Wokwi simulation link
│   └── sketches/          # Place your .ino and bare-metal AVR C sketches here
├── esp8266/               # ESP8266 (ESP-12E / NodeMCU Wi-Fi)
│   ├── README.md          # Wi-Fi station mode, TCP sockets, Wokwi simulation link
│   └── sketches/          # Place your network sketches here
├── esp32/                 # ESP32 (Xtensa Dual-Core 32-bit MCU + FreeRTOS)
│   ├── README.md          # FreeRTOS tasks, queues, dual-core pinning, Wokwi guide
│   └── sketches/          # Place your FreeRTOS & BLE/Wi-Fi projects here
└── stm32/                 # STM32 (ARM Cortex-M4 / Black Pill / Nucleo)
    ├── README.md          # Cortex-M4 registers, RCC clock setup, QEMU/Wokwi guide
    └── src/               # Place your bare-metal C drivers & linker scripts here
```

---

## 🔨 How to Use This Workspace

### 1. Follow the "Simulation-First" Workflow
Before flashing physical microcontrollers, always verify your logic in software simulators:
- **PIC16F877A:** Test in **PICSimLab** (Board 1: PICGenios or Breadboard).
- **Arduino Mega 2560:** Test in **Wokwi** (select Arduino Mega 2560).
- **ESP8266 & ESP32:** Test in **Wokwi** (with virtual Wi-Fi and logic analyzer).
- **STM32:** Test in **Wokwi** or **QEMU** before purchasing or wiring your physical Black Pill / Nucleo board.

Read our complete [**Simulation-First Workflow Guide (`guides/simulation-first-workflow.md`)**](../guides/simulation-first-workflow.md).

### 2. Follow the 7-Step Code Construction Workflow
When writing any driver in this workspace:
1. **Never copy-paste code.**
2. Open the datasheet for your specific chip alongside your IDE.
3. Consult the driver templates in `my-code/<platform>/templates/`.
4. Implement the numbered initialization sequence found in the datasheet.
5. Compile, load into your simulator, and verify behavior with virtual instruments.
6. Commit your driver files to your repository!
