# ⚡ Embedded Engineering Roadmap — From Fresher to Advanced

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)
[![Curriculum](https://img.shields.io/badge/Curriculum-8--Step%20Structured-blue)](ROADMAP.md)
[![Schedule](https://img.shields.io/badge/Schedule-6--Month%20Plan-green)](ROADMAP.md#-the-6-month-structured-plan)
[![Taxonomy](https://img.shields.io/badge/Taxonomy-3--Pillars-purple)](curriculum/reference/README.md)
[![PRs Welcome](https://img.shields.io/badge/PRs-welcome-brightgreen.svg)](CONTRIBUTING.md)

> A free, student-focused curriculum, taxonomy, and resource hub for aspiring and practicing embedded systems engineers. Designed for undergraduates, career switchers, and engineers leveling up from bare-metal to operating systems.

---

> ### 🛑 THE CORE RULE OF EMBEDDED ENGINEERING
> **"Do not learn peripherals only as APIs. Understand the register, electrical signal, timing diagram, protocol transaction and failure modes behind each API call."**

---

## 🧭 Table of Contents

- [The 3 Pillars of Embedded Systems Taxonomy](#-the-3-pillars-of-embedded-systems-taxonomy)
- [The 4-Pillar, 8-Step Guided Learning Path](#-the-4-pillar-8-step-guided-learning-path)
- [The 6-Month Study Plan](#-the-6-month-study-plan)
- [The 7 Production-Grade Portfolio Projects](#-the-7-production-grade-portfolio-projects)
- [Student Starter Hardware Lab (Under $50)](#-student-starter-hardware-lab-under-50)
- [Curated Resources & Cheatsheets](#-curated-resources--cheatsheets)
- [PIC Microcontrollers Track](#-pic-microcontrollers-with-mplab-x-xc8--picsimlab)
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

## 🛒 Student Starter Hardware Lab (Under $50)

You do **not** need expensive commercial equipment to complete this roadmap. Here is the recommended student workbench:

| Item | Specification | Approx Cost | Purpose |
| :--- | :--- | :--- | :--- |
| **Microcontroller Board** | STM32F401CCU6 "Black Pill" | ~$4.50 | ARM Cortex-M4 with FPU, 84 MHz, 256KB Flash, USB-C |
| **Debug Probe** | ST-Link V2 Clone | ~$3.00 | SWD hardware flashing and GDB step-by-step debugging |
| **Logic Analyzer** | 8-Channel 24 MHz USB (Saleae-compatible) | ~$7.00 | Decoding UART, SPI, and I2C waveforms in PulseView |
| **USB-to-UART Adapter** | CP2102 or CH340 with 3.3V jumper | ~$2.50 | Interactive serial CLI terminal output to host PC |
| **Digital Multimeter** | Auto-ranging DMM (e.g. AN8002) | ~$14.00 | Continuity testing, power rail checks, current measurement |
| **Environmental Sensor** | Bosch BMP280 (I2C + SPI) | ~$2.00 | Real hardware registers and calibration compensation |
| **Motion Sensor** | InvenSense MPU-6050 (6-Axis IMU) | ~$2.50 | I2C motion tracking and TinyML gesture inferencing |
| **Prototyping Kit** | 830-pt breadboard + jumper wire bundle | ~$5.50 | Solderless rapid hardware prototyping |
| **Discrete Components** | Resistors, LEDs, Pushbuttons | ~$4.00 | Current limiting, pull-ups, and debounce circuits |
| **TOTAL** | | **~$45.00** | **Complete hardware development lab!** |

*Note: Every peripheral lab up to Month 4 can also be completed for $0 using the [Wokwi Web Simulator](https://wokwi.com/) or [QEMU](https://www.qemu.org/).*

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

---

## 📂 Repository Structure

```
embedded-engineering-roadmap/
├── curriculum/                       # The 8-Step Progressive Learning Path
│   ├── 01-c-embedded-c/              # Step 1: C, memory model, volatile, bitwise, MISRA
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
│   ├── installation.md               # MPLAB X / XC8 / PICSimLab setup guide
│   ├── beginner.md                   # Steps 1-3: blink, GPIO, timers
│   ├── intermediate.md               # Steps 4-8: ADC, PWM, UART, SPI/I2C, LCD
│   └── advanced.md                   # Steps 9-13: low-power, interrupts, capstone
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
├── code-examples/                    # Commented, runnable code templates
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
├── ROADMAP.md                        # Clickable 8-step checklist & 6-month plan
└── README.md                         # This main roadmap document
```

---

## 📜 License

Original written tutorials, project guides, cheatsheets, and code examples are licensed under the [MIT License](LICENSE).
