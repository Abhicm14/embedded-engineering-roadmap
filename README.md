# ⚡ Embedded Engineering Roadmap — From Fresher to Advanced

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)
[![Prerequisites](https://img.shields.io/badge/Prerequisites-Start%20Here-orange)](PREREQUISITES.md)
[![Curriculum](https://img.shields.io/badge/Curriculum-8--Step%20Structured-blue)](ROADMAP.md)
[![Schedule](https://img.shields.io/badge/Schedule-6--Month%20Plan-green)](ROADMAP.md#-the-6-month-structured-plan)
[![Taxonomy](https://img.shields.io/badge/Taxonomy-3--Pillars-purple)](curriculum/reference/README.md)
[![PRs Welcome](https://img.shields.io/badge/PRs-welcome-brightgreen.svg)](CONTRIBUTING.md)

> A free, student-focused curriculum, taxonomy, and resource hub for aspiring and practicing embedded systems engineers. Written in simple, friendly, plain English so that **anyone—from a 16-year-old high school student to an undergraduate or career switcher**—can master embedded systems from the ground up without confusion.

---

### 👋 What is Embedded Engineering? (In Plain English!)

Have you ever wondered how a drone balances in the sky, how a microwave counts down and beeps, or how a video game controller registers your button presses?  
All of those gadgets are powered by **Embedded Systems**!

- A **regular computer** (like a PC or phone) is like a brilliant professor sitting at a desk doing math homework behind a screen.
- An **embedded microcontroller** is like an athletic robot with hands and eyes: it has tiny metal pins that can physically flip light switches, listen to temperature sensors, spin electric motors, and talk to radios in the real physical world!

---

> ### 🛑 THE CORE RULES OF EMBEDDED ENGINEERING
> 1. **"Do not learn peripherals only as APIs. Understand the register, electrical signal, timing diagram, protocol transaction and failure modes behind each API call."**
> 2. **"Never copy-paste code. Every driver and peripheral implementation must be constructed step-by-step from electrical specifications and silicon register maps."**

---

## 🧠 Prerequisites: What to Learn Before Writing Code

Before writing code for STM32, PIC, ESP32, or AVR, review our dedicated foundation guide:

👉 [**📘 Master Prerequisites Guide (`PREREQUISITES.md`)**](PREREQUISITES.md)

| Foundational Discipline | What You Must Understand Before Coding |
| :--- | :--- |
| **🔢 Mathematical Foundations** | Binary, Hexadecimal, Two's complement, bitwise masks, and fixed-point scaling (no software floats). |
| **⚡ Electrical Physics** | Ohm's law, pull-up/pull-down resistors, open-drain vs push-pull, decoupling capacitors, slew rates. |
| **🖥️ Computer Architecture** | Harvard vs Von Neumann, CPU registers (PC, SP, LR), memory map, memory-mapped I/O, endianness. |
| **💻 Embedded C Hygiene** | Pointer arithmetic, struct padding & `#pragma pack(1)`, `volatile` qualifier, and non-blocking ISR rules. |
| **📑 Silicon Datasheet Literacy** | Base addresses, offsets, pin multiplexing tables, register reset values, and errata sheets. |
| **🔨 7-Step Code Construction** | The non-negotiable step-by-step workflow followed across all code examples in this repository. |

---

## 🧭 Table of Contents

- [🧠 Prerequisites: What to Learn Before Coding](#-prerequisites-what-to-learn-before-coding)
- [The 3 Pillars of Embedded Systems Taxonomy](#-the-3-pillars-of-embedded-systems-taxonomy)
- [The 4-Pillar, 8-Step Guided Learning Path](#-the-4-pillar-8-step-guided-learning-path)
- [The 6-Month Study Plan](#-the-6-month-study-plan)
- [The 7 Production-Grade Portfolio Projects](#-the-7-production-grade-portfolio-projects)
- [Student Starter Hardware Lab Equipment](#-student-starter-hardware-lab-equipment)
- [Curated Resources & Cheatsheets](#-curated-resources--cheatsheets)
- [PIC Microcontrollers Track](#-pic-microcontrollers-with-mplab-x-xc8--picsimlab)
- [Simulation-First Workflow](#-simulation-first-workflow)
- [Personal Coding Workspace (`my-code/`)](#-personal-coding-workspace-my-code)
- [Repository Structure](#-repository-structure)
- [License](#-license)

---

## 🏛️ The 3 Pillars of Embedded Systems Taxonomy

Mastery in embedded engineering requires balanced expertise across three interconnected disciplines:

```
                                  ┌────────────────────────┐
                                  │   EMBEDDED ENGINEER    │
                                  └───────────┬────────────┘
                        ┌─────────────────────┼─────────────────────┐
                        ▼                     ▼                     ▼
             ┌─────────────────────┐┌─────────────────────┐┌─────────────────────┐
             │     1. SOFTWARE     ││     2. HARDWARE     ││   3. SOFT-SKILLS    │
             ├─────────────────────┤├─────────────────────┤├─────────────────────┤
             │ • Embedded C & C++  ││ • Electric Circuits ││ • Reading Datasheets│
             │ • Assembly & Startup││ • Passives & MOSFETs││ • Systematic Debug  │
             │ • Memory & Linkers  ││ • MCU Architectures ││ • Git & Code Reviews│
             │ • RTOS & Multitask  ││ • Schematics & PCB  ││ • Spec Writing      │
             │ • Linux & Drivers   ││ • Signal Integrity  ││ • Design Interviews │
             │ • Protocol Stacks   ││ • Lab Instruments   ││ • Teamwork & Growth │
             └─────────────────────┘└─────────────────────┘└─────────────────────┘
```

- [**💻 Software Taxonomy Reference**](curriculum/reference/software.md): C, C++, Assembly, Python/MicroPython, Rust, Zig; RTOS engines, Linux subsystems, protocol stacks, testing frameworks (Unity, CMock), and MISRA-C.
- [**🔌 Hardware Taxonomy Reference**](curriculum/reference/hardware.md): Circuits, digital logic, MCU architectures (ARM, RISC-V, ESP32), power management (LDO/Buck), PCB design in KiCad, test gear, and FPGA HDL synthesis.
- [**🧠 Soft-Skills & Practices**](curriculum/reference/soft-skills.md): Datasheet navigation, systematic root-cause debugging, engineering communication, and system design interviews.

---

## 🚀 The 4-Pillar, 8-Step Guided Learning Path

The learning path progresses sequentially across **4 Pillars** spanning **8 Steps**:

```
[ FOUNDATION ] ──► [ FIRMWARE ] ──► [ SYSTEMS ] ──► [ CAREER ]
  (C & Physics)      (STM32 & Periph) (RTOS & Linux)  (Projects & Interview)
```

| Step | Module Guide | Key Themes | Core Deliverable / Practice |
| :---: | :--- | :--- | :--- |
| **01** | [**C & Embedded C**](curriculum/01-c-embedded-c/README.md) | Pointers, memory layout, structs, bitwise manipulation, `volatile`/`const`, MISRA-C. | Circular ring buffer, FSM, and switch debounce in C. |
| **02** | [**Electronics & Computer Fundamentals**](curriculum/02-electronics-computer-fundamentals/README.md) | Ohm's law, pull-ups, MOSFETs, gates, CPU registers, ALU, scopes, and logic analyzers. | Pull-up calculation, low-side switch wiring, and UART signal decode. |
| **03** | [**STM32 & Microcontrollers**](curriculum/03-stm32-microcontrollers/README.md) | Cortex-M architecture, vector table, startup code, linker script, Makefiles, RCC clock trees. | [Project 1: Bare-Metal GPIO & SysTick FSM](projects/beginner.md#project-1-gpio-control-board) |
| **04** | [**Essential MCU Peripherals**](curriculum/04-essential-mcu-peripherals/README.md) | GPIO, Timers, PWM, Interrupts/NVIC, UART, SPI, I2C, ADC, DAC, and failure modes. | [Project 2: UART Command Console](projects/beginner.md#project-2-uart-command-console) |
| **05** | [**Engineering Workflow & Debugging**](curriculum/05-engineering-workflow/README.md) | SWD/JTAG, GDB, watchpoints, logic analyzers, scopes, HardFault triage, Git, and CI/CD. | Decode HardFault stack frame and isolate crashing line in `.map` file. |
| **06** | [**FreeRTOS & RTOS Internals**](curriculum/06-freertos-rtos/README.md) | Task states, TCB, PendSV context switch, Queues, Mutexes, PIP, and alternative RTOSes. | [Project 5: FreeRTOS Environmental Monitor](projects/intermediate.md#project-5-freertos-environmental-monitor) |
| **07** | [**Embedded Linux & System Architecture**](curriculum/07-embedded-linux/README.md) | Boot sequence, U-Boot, Device Trees, Kernel modules, character drivers, and POSIX I/O. | Minimal Linux kernel in QEMU + custom character device driver. |
| **08** | [**Specialization, Career & Job Readiness**](curriculum/08-specialization-career/README.md) | Automotive CAN/AUTOSAR, Secure IoT & OTA, TinyML Edge AI, and interview prep. | Flagship GitHub portfolio + 50+ interview questions drill. |

---

## 📅 The 6-Month Structured Plan

```
Month 1: C, Embedded C, Electronics, Git ────────► C Exercises + GPIO Bare-Metal Project
Month 2: STM32 Arch, GPIO, Timers, NVIC, UART ───► UART Command Console Project
Month 3: SPI, I2C, ADC, PWM, DMA, Lab Tools ─────► Sensor Data Logger + PWM Controller
Month 4: FreeRTOS, Concurrency, Sync, Faults ────► RTOS Environmental Monitor Project
Month 5: Embedded Linux + Specialization Track ──► Linux Kernel Driver + Track Prototype
Month 6: Flagship Capstone, Testing, Resume ─────► Industry Portfolio + Interview Prep
```

- **Weekly Study Rhythm:** ~40% Core Fundamentals, ~40% Hands-on Coding/Hardware, ~20% Debugging & Documentation.
- Full weekly schedule and milestones: Read [`ROADMAP.md`](ROADMAP.md).

---

## 🛠️ The 7 Production-Grade Portfolio Projects

Each project includes architecture diagrams, hardware pinouts, register-level analysis, and complete runnable code:

1. [**GPIO Control Board**](projects/beginner.md#project-1-gpio-control-board) (C, Bare-Metal GPIO, SysTick FSM)
2. [**UART Command Console**](projects/beginner.md#project-2-uart-command-console) (UART, Interrupts, Lock-Free Ring Buffer, CLI Shell)
3. [**Sensor Data Logger**](projects/intermediate.md#project-3-sensor-data-logger) (I2C BMP280, SPI Flash W25Qxx, DMA)
4. [**PWM Fan/Motor Controller**](projects/intermediate.md#project-4-pwm-fanmotor-controller) (Timer PWM, Input Capture, Closed-Loop PID)
5. [**FreeRTOS Environmental Monitor**](projects/intermediate.md#project-5-freertos-environmental-monitor) (FreeRTOS Tasks, Queues, Mutexes with PIP, Low Power)
6. [**Connected IoT Node**](projects/advanced.md#project-6-connected-iot-node) (Wi-Fi, TLS 1.3, MQTT, Dual-Bank A/B Fail-Safe OTA)
7. [**TinyML Edge Device**](projects/advanced.md#project-7-tinyml-edge-device) (6-Axis IMU, CMSIS-NN Quantized INT8 Gesture Classifier)

---

## 🛒 Student Starter Hardware Lab Equipment

You do **not** need expensive commercial equipment to complete this roadmap. Here is the recommended student workbench setup:

| Item | Specification | Purpose |
| :--- | :--- | :--- |
| **Microcontroller Board** | STM32F401CCU6 "Black Pill" | ARM Cortex-M4 with FPU, 84 MHz, 256KB Flash, USB-C |
| **Debug Probe** | ST-Link V2 Clone | SWD hardware flashing and GDB step-by-step debugging |
| **Logic Analyzer** | 8-Channel 24 MHz USB (Saleae-compatible) | Decoding UART, SPI, and I2C waveforms in PulseView |
| **USB-to-UART Adapter** | CP2102 or CH340 with 3.3V jumper | Interactive serial CLI terminal output to host PC |
| **Digital Multimeter** | Auto-ranging DMM (e.g. AN8002) | Continuity testing, power rail checks, current measurement |
| **Environmental Sensor** | Bosch BMP280 (I2C + SPI) | Real hardware registers and calibration compensation |
| **Motion Sensor** | InvenSense MPU-6050 (6-Axis IMU) | I2C motion tracking and TinyML gesture inferencing |
| **Prototyping Kit** | 830-pt breadboard + jumper wire bundle | Solderless rapid hardware prototyping |
| **Discrete Components** | Resistors, LEDs, Pushbuttons | Current limiting, pull-ups, and debounce circuits |

*Note: Every peripheral lab can also be completed entirely in software simulation using [PICSimLab](https://lcgamboa.github.io/picsimlab_docs/), [Wokwi Web Simulator](https://wokwi.com/), or [QEMU](https://www.qemu.org/). See our [Simulation-First Workflow Guide](guides/simulation-first-workflow.md).*

---

## 💎 Curated Resources & Cheatsheets

- **[Resource Hub (`resources/`)](resources/README.md):**
  - [📘 Books](resources/books.md)
  - [🎓 Online Courses](resources/courses.md)
  - [🛠️ Tools & Software](resources/tools-and-software.md)
  - [🎞️ YouTube & Blogs](resources/youtube-and-blogs.md)
  - [📑 Official Silicon Datasheets](resources/datasheets-and-reference.md)
- **[Cheatsheets (`cheatsheets/`)](cheatsheets/README.md):**
  - [Embedded C](cheatsheets/c-cheatsheet.md)
  - [Bitwise Operations](cheatsheets/bitwise-cheatsheet.md)
  - [PIC XC8 Cheatsheet](cheatsheets/pic-xc8-cheatsheet.md)
  - [Protocols Comparison](cheatsheets/protocols-comparison.md)
  - [Cortex-M Faults](cheatsheets/cortex-m-faults.md)
  - [Common I2C Addresses](cheatsheets/common-i2c-addresses.md)
  - [Interview Checklist](cheatsheets/interview-checklist.md)
  - [Top 15 Common Mistakes](cheatsheets/common-mistakes.md)

---

## 🔌 PIC Microcontrollers with MPLAB X, XC8 & PICSimLab

> A complete beginner-to-advanced guide for PIC microcontrollers using MPLAB X IDE, the XC8 compiler, and PICSimLab simulator. Follows the core rule: **understand the register, not just the API**.

[![MPLAB X](https://img.shields.io/badge/MPLAB%20X-v5.35-blue)](https://www.microchip.com/mplab/mplab-x-ide)
[![XC8](https://img.shields.io/badge/XC8-v2.36-green)](https://www.microchip.com/mplab/compilers)
[![PICSimLab](https://img.shields.io/badge/PICSimLab-v0.7-orange)](https://picsimlab.sourceforge.net/)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)

This track teaches PIC microcontroller programming at the register level, focusing on the PIC16F877A as a learning platform. You'll progress from basic LED blinking to advanced topics like PID control and real-world hardware interfacing.

**Core Philosophy**: Every line of code is explained in terms of register manipulation, timing diagrams, and electrical characteristics—not just API calls.

[📘 Full PIC Track Guide](./pic-mplab-xc8/README.md)  
[📖 How to Read Any Datasheet & Write Custom Drivers](./pic-mplab-xc8/datasheet-driver-guide.md)

---

## 🖥️ Simulation-First Workflow

Before flashing physical microcontrollers, verify your drivers in software simulation:
- **PIC16F877A:** [PICSimLab Simulator](https://lcgamboa.github.io/picsimlab_docs/) (Board 1: PICGenios / Breadboard)
- **Arduino Mega 2560:** [Wokwi Mega 2560](https://wokwi.com/projects/new/arduino-mega) (54 I/O pins, 4 hardware UARTs)
- **ESP8266:** [Wokwi ESP8266](https://wokwi.com/projects/new/esp8266) (Virtual Wi-Fi and IoT protocols)
- **ESP32:** [Wokwi ESP32](https://wokwi.com/projects/new/esp32) (Dual-core FreeRTOS + Wi-Fi + Logic Analyzer)
- **STM32 Cortex-M4:** [Wokwi STM32](https://wokwi.com/) & [QEMU](https://www.qemu.org/) (Bare-metal startup & GDB)

👉 Read our complete [**Simulation-First Workflow Guide (`guides/simulation-first-workflow.md`)**](guides/simulation-first-workflow.md).

---

## 🛠️ Personal Coding Workspace (`my-code/`)

A dedicated space in this repository for you to write, build, and save your own peripheral drivers and projects:
- [**`my-code/pic16f877a/`**](my-code/pic16f877a/README.md): Write your own PIC16F877A drivers using starter templates.
- [**`my-code/arduino-mega2560/`**](my-code/arduino-mega2560/README.md): ATmega2560 bare-metal AVR and Arduino sketches.
- [**`my-code/esp8266/`**](my-code/esp8266/README.md): ESP8266 Wi-Fi and IoT communication projects.
- [**`my-code/esp32/`**](my-code/esp32/README.md): ESP32 dual-core FreeRTOS and BLE implementations.
- [**`my-code/stm32/`**](my-code/stm32/README.md): STM32 bare-metal C drivers and linker scripts.

👉 Check out the [**Workspace Master Guide (`my-code/README.md`)**](my-code/README.md).

---

## 📂 Repository Structure

```
embedded-engineering-roadmap/
├── curriculum/                       # The 8-Step Progressive Learning Path
│   ├── 01-c-embedded-c/              # Step 1: C, memory model, volatile, advanced pointers
│   ├── 02-electronics-computer-fundamentals/ # Step 2: Circuits, gates, CPU, scopes, DMM
│   ├── 03-stm32-microcontrollers/    # Step 3: Cortex-M, vector table, startup, linkers
│   ├── 04-essential-mcu-peripherals/ # Step 4: GPIO, Timers, PWM, UART, SPI, I2C, ADC
│   ├── 05-engineering-workflow/      # Step 5: GDB, logic analyzers, HardFaults, Git
│   ├── 06-freertos-rtos/             # Step 6: FreeRTOS kernel, queues, mutexes, PIP
│   ├── 07-embedded-linux/            # Step 7: Boot flow, U-Boot, Device Trees, drivers
│   ├── 08-specialization-career/     # Step 8: Automotive, IoT/Security, TinyML, interviews
│   └── reference/                    # Broad 3-Pillar Taxonomy Reference
│       ├── software.md
│       ├── hardware.md
│       └── soft-skills.md
├── pic-mplab-xc8/                    # PIC Microcontroller Track (MPLAB X, XC8, PICSimLab)
│   ├── README.md                     # Overview, toolchain, and 13-step learning sequence
│   ├── datasheet-driver-guide.md     # How to read any datasheet and write custom drivers
│   ├── installation.md               # MPLAB X / XC8 / PICSimLab setup guide
│   ├── beginner.md                   # Steps 1-3: blink, GPIO, timers
│   ├── intermediate.md               # Steps 4-8: ADC, PWM, UART, SPI/I2C, LCD
│   └── advanced.md                   # Steps 9-13: low-power, interrupts, capstone
├── guides/                           # Engineering Guides & Workflows
│   └── simulation-first-workflow.md  # Software simulation guide (PIC, Mega, ESP, STM32)
├── my-code/                          # Personal User Coding Workspace (Write Your Own Code)
│   ├── README.md                     # Workspace guidelines & instructions
│   ├── pic16f877a/                   # PIC16F877A custom driver templates & user drivers
│   ├── arduino-mega2560/             # ATmega2560 sketches & bare-metal AVR projects
│   ├── esp8266/                      # ESP8266 Wi-Fi & IoT sketches
│   ├── esp32/                        # ESP32 FreeRTOS multi-core projects
│   └── stm32/                        # STM32 bare-metal C drivers & linker scripts
├── resources/                        # Quality-Rated Resource Hub
│   ├── books.md
│   ├── courses.md
│   ├── tools-and-software.md
│   ├── youtube-and-blogs.md
│   └── datasheets-and-reference.md
├── projects/                         # Portfolio Projects
│   ├── beginner.md                   # Projects 1 & 2 + PIC Beginner Projects
│   ├── intermediate.md               # Projects 3, 4 & 5 + PIC Intermediate Projects
│   └── advanced.md                   # Projects 6 & 7, CAN Node + PIC Advanced Projects
├── code-examples/                    # Reference code templates & step-by-step breakdowns
│   ├── c/                            # Ring buffer, FSM, debounce, CRC, bit-ops, parser
│   ├── xc8/                          # PIC16F877A XC8 register-level examples (8 files)
│   ├── arduino/                      # Arduino framework blinky
│   ├── stm32/                        # STM32 register-level blinky
│   ├── esp32/                        # ESP32 FreeRTOS Wi-Fi blinky
│   └── verilog/                      # Verilog clock divider & UART TX module
├── cheatsheets/                      # High-density reference cards
│   ├── c-cheatsheet.md
│   ├── bitwise-cheatsheet.md
│   ├── pic-xc8-cheatsheet.md         # PIC16F877A registers, configuration, peripherals
│   ├── protocols-comparison.md
│   ├── cortex-m-faults.md
│   ├── common-i2c-addresses.md
│   ├── interview-checklist.md
│   └── common-mistakes.md
├── .github/                          # CI workflow and issue templates
├── SETUP.md                          # Local preview and toolchain installation guide
├── CONTRIBUTING.md                   # Contributor guidelines
├── CODE_OF_CONDUCT.md               # Contributor Covenant v2.1
├── LICENSE                           # MIT License
├── PREREQUISITES.md                  # Foundational math, physics, architecture & hygiene guide
├── ROADMAP.md                        # Clickable 8-step checklist & 6-month plan
└── README.md                         # This main roadmap document
```

---

## 📜 License

Original written tutorials, project guides, cheatsheets, and code examples are licensed under the [MIT License](LICENSE).
