# 🗺️ Embedded Engineering Roadmap & 6-Month Study Plan

> A clickable, progressive 8-step checklist merged with a structured 6-month study schedule.

---

> ### 🛑 THE CORE RULE OF EMBEDDED ENGINEERING
> **"Do not learn peripherals only as APIs. Understand the register, electrical signal, timing diagram, protocol transaction and failure modes behind each API call."**

---

## 🏛️ The 4 Pillars & 8 Steps Architecture

```
┌─────────────────────────────────────────────────────────────────────────────────────────────────┐
│ PILLAR 1: FOUNDATION (C Programming & Electronics Physics)                                      │
│   Step 1: C & Embedded C (Pointers, memory layout, volatile, bitwise manipulation, MISRA-C)     │
│   Step 2: Electronics & Computer Fundamentals (Circuits, pull-ups, MOSFETs, CPU registers, lab) │
├─────────────────────────────────────────────────────────────────────────────────────────────────┤
│ PILLAR 2: FIRMWARE (STM32 & Silicon Peripherals)                                                │
│   Step 3: STM32 & Microcontrollers (Cortex-M4, vector table, startup, linkers, Makefiles, RCC)  │
│   Step 4: Essential MCU Peripherals (GPIO, Timers, PWM, NVIC, UART, SPI, I2C, ADC, DMA)        │
│   Step 5: Engineering Workflow (SWD/JTAG, GDB, logic analyzers, HardFaults, Git, CI/CD)         │
├─────────────────────────────────────────────────────────────────────────────────────────────────┤
│ PILLAR 3: SYSTEMS (RTOS & Embedded Linux)                                                       │
│   Step 6: FreeRTOS & RTOS Internals (TCB, context switch, queues, mutexes, PIP, other RTOSes)  │
│   Step 7: Embedded Linux (Boot flow, U-Boot, Device Trees, Kernel modules, character drivers)   │
├─────────────────────────────────────────────────────────────────────────────────────────────────┤
│ PILLAR 4: CAREER (Projects, Specializations & Interviews)                                       │
│   Step 8: Specialization Tracks (Automotive CAN/AUTOSAR, Secure IoT & OTA, TinyML Edge AI)     │
│           + Portfolio Packaging & Technical Interview Mastery                                   │
└─────────────────────────────────────────────────────────────────────────────────────────────────┘
```

> [!TIP]
> **Alternative 8-Bit Architecture Path: PIC Microcontrollers**  
> If your curriculum or industry focus starts with 8-bit Harvard microcontrollers before 32-bit ARM Cortex-M, explore our dedicated [**PIC Microcontroller Track (`pic-mplab-xc8/`)**](pic-mplab-xc8/README.md). It covers the Microchip PIC16F877A, MPLAB X IDE, XC8 compiler, and PICSimLab simulation across 13 progressive register-level steps.

---

## 📋 The Clickable 8-Step Progress Checklist

Use this interactive checklist to track your progress through the curriculum:

- [ ] ### [Step 1: C & Embedded C](curriculum/01-c-embedded-c/README.md)
  - [ ] Pointers, memory addresses, and function pointer callbacks ([Guide](curriculum/01-c-embedded-c/syntax-and-pointers.md))
  - [ ] Memory model: Flash vs RAM, stack, heap, and map files ([Guide](curriculum/01-c-embedded-c/memory-model.md))
  - [ ] Struct padding, alignment, and `#pragma pack(1)` ([Guide](curriculum/01-c-embedded-c/structs-unions-bitfields.md))
  - [ ] Volatile, const, static, and extern semantics ([Guide](curriculum/01-c-embedded-c/volatile-const-type-qualifiers.md))
  - [ ] Build process: Preprocess, compile, assemble, link ([Guide](curriculum/01-c-embedded-c/build-process.md))
  - [ ] Coding: Implement lock-free circular buffer ([`code-examples/c/circular_buffer.c`](code-examples/c/circular_buffer.c))
  - [ ] Coding: Implement table-driven FSM ([`code-examples/c/fsm.c`](code-examples/c/fsm.c))

- [ ] ### [Step 2: Electronics & Computer Fundamentals](curriculum/02-electronics-computer-fundamentals/README.md)
  - [ ] Ohm's Law, pull-up resistors, and capacitor decoupling ([Guide](curriculum/02-electronics-computer-fundamentals/circuit-basics-and-components.md))
  - [ ] N-channel MOSFET low-side switching with flyback diode
  - [ ] Digital logic gates, D flip-flops, and setup/hold times ([Guide](curriculum/02-electronics-computer-fundamentals/digital-logic-and-gates.md))
  - [ ] CPU core: Registers, ALU, PC, SP, and endianness ([Guide](curriculum/02-electronics-computer-fundamentals/cpu-and-memory-architecture.md))
  - [ ] Signal rise-time, slew rate, ringing, and ground bounce ([Guide](curriculum/02-electronics-computer-fundamentals/signals-and-noise.md))
  - [ ] Workbench tools: Multimeter, 10X oscilloscope probe, and logic analyzer ([Guide](curriculum/02-electronics-computer-fundamentals/lab-instruments-and-probing.md))

- [ ] ### [Step 3: STM32 & Microcontrollers](curriculum/03-stm32-microcontrollers/README.md)
  - [ ] ARM Cortex-M architecture, core registers, and operating modes ([Guide](curriculum/03-stm32-microcontrollers/cortex-m-core-and-registers.md))
  - [ ] Boot sequence: Reset signal to `Reset_Handler` to `main()` ([Guide](curriculum/03-stm32-microcontrollers/clock-tree-and-reset.md))
  - [ ] Authoring bare-metal `startup.c` and vector table
  - [ ] Linker scripts: `MEMORY`, `SECTIONS`, LMA vs VMA ([Guide](curriculum/03-stm32-microcontrollers/toolchains-linkers-makefiles.md))
  - [ ] Configuring RCC clock trees, Phase-Locked Loop (PLL), and Flash latency
  - [ ] **Milestone Project:** [Project 1: Bare-Metal GPIO & SysTick FSM](projects/beginner.md#project-1-gpio-control-board)

- [ ] ### [Step 4: Essential MCU Peripherals](curriculum/04-essential-mcu-peripherals/README.md)
  - [ ] GPIO registers: Mode, speed, pull-ups, and atomic `BSRR` ([Guide](curriculum/04-essential-mcu-peripherals/gpio.md))
  - [ ] Hardware Timers & PWM generation ([Guide](curriculum/04-essential-mcu-peripherals/timers-and-pwm.md))
  - [ ] External interrupts (`EXTI`) and ISR design rules ([Guide](curriculum/04-essential-mcu-peripherals/interrupts-and-nvic.md))
  - [ ] UART: Asynchronous framing, baud calculation, and circular DMA ([Guide](curriculum/04-essential-mcu-peripherals/uart.md))
  - [ ] SPI: The 4 clock modes (CPOL/CPHA) and slave selection ([Guide](curriculum/04-essential-mcu-peripherals/spi.md))
  - [ ] I2C: Open-drain, pull-ups, ACK/NACK, and 9-clock recovery ([Guide](curriculum/04-essential-mcu-peripherals/i2c.md))
  - [ ] ADC/DAC: Successive approximation, sampling time, and DMA ([Guide](curriculum/04-essential-mcu-peripherals/adc-dac.md))
  - [ ] **Milestone Project:** [Project 2: UART Command Console](projects/beginner.md#project-2-uart-command-console)

- [ ] ### [Step 5: Engineering Workflow & Debugging](curriculum/05-engineering-workflow/README.md)
  - [ ] GDB on-chip debugging: Breakpoints, hardware watchpoints, and memory dumps ([Guide](curriculum/05-engineering-workflow/debugging-and-gdb.md))
  - [ ] Logic analyzer vs Oscilloscope diagnostic methodology ([Guide](curriculum/05-engineering-workflow/logic-analyzers-and-scopes.md))
  - [ ] HardFault triage: Unstacking `PC`/`LR`, decoding `CFSR`/`BFAR`, and `.map` correlation ([Guide](curriculum/05-engineering-workflow/hard-fault-analysis.md))
  - [ ] Git version control: Clean `.gitignore`, feature branches, atomic commits ([Guide](curriculum/05-engineering-workflow/git-and-version-control.md))
  - [ ] Off-target unit testing with Unity/CMock and automated CI ([Guide](curriculum/05-engineering-workflow/testing-and-builds.md))

- [ ] ### [Step 6: FreeRTOS & RTOS Internals](curriculum/06-freertos-rtos/README.md)
  - [ ] Preemptive scheduling, Task Control Block (TCB), and `PendSV` context switch ([Guide](curriculum/06-freertos-rtos/tasks-and-scheduler.md))
  - [ ] Synchronization: Semaphores, Mutexes, Event Groups, Task Notifications ([Guide](curriculum/06-freertos-rtos/synchronization-primitives.md))
  - [ ] Inter-Task Queues: FIFO pipelines, pass-by-copy vs pointer ([Guide](curriculum/06-freertos-rtos/queues-and-data-passing.md))
  - [ ] Priority Inversion and the Priority Inheritance Protocol (PIP) ([Guide](curriculum/06-freertos-rtos/priority-inversion.md))
  - [ ] Alternative RTOS comparison: Zephyr, RT-Thread, NuttX, ThreadX ([Guide](curriculum/06-freertos-rtos/other-rtos-ecosystems.md))
  - [ ] **Milestone Project:** [Project 5: FreeRTOS Environmental Monitor](projects/intermediate.md#project-5-freertos-environmental-monitor)

- [ ] ### [Step 7: Embedded Linux & System Architecture](curriculum/07-embedded-linux/README.md)
  - [ ] 4-stage boot sequence: ROM Bootloader $\rightarrow$ SPL $\rightarrow$ U-Boot $\rightarrow$ Kernel $\rightarrow$ Rootfs ([Guide](curriculum/07-embedded-linux/linux-architecture-and-boot.md))
  - [ ] Cross-toolchain triplets and building rootfs with Buildroot / Yocto ([Guide](curriculum/07-embedded-linux/cross-compilation-and-rootfs.md))
  - [ ] Flattened Device Tree Source (`.dts`) nodes and `compatible` strings ([Guide](curriculum/07-embedded-linux/device-trees.md))
  - [ ] Linux Kernel Module (LKM) character device drivers and `copy_to_user` ([Guide](curriculum/07-embedded-linux/kernel-modules-and-drivers.md))
  - [ ] User-space POSIX programming: `pthreads`, sockets, and `libgpiod` ([Guide](curriculum/07-embedded-linux/userspace-posix-and-peripherals.md))

- [ ] ### [Step 8: Specialization, Career & Job Readiness](curriculum/08-specialization-career/README.md)
  - [ ] Track A: Automotive CAN/CAN-FD, UDS, AUTOSAR, ISO 26262 ([Guide](curriculum/08-specialization-career/automotive-track.md))
  - [ ] Track B: IoT, BLE, MQTT, TLS 1.3, Secure Boot, dual-bank A/B OTA ([Guide](curriculum/08-specialization-career/iot-wireless-security.md))
  - [ ] Track C: TinyML, IMU DSP, INT8 quantization, CMSIS-NN inferencing ([Guide](curriculum/08-specialization-career/tinyml-edge-ai.md))
  - [ ] Complete Flagship Capstone Portfolio Showcase ([Guide](projects/README.md))
  - [ ] Complete 50+ interview questions drill ([Cheatsheet](cheatsheets/interview-checklist.md))

---

## 📅 The 6-Month Structured Plan

Dedicate 15 to 20 hours per week following the **40 / 40 / 20 Rule**:
- **40% Core Theory & Reading:** Silicon datasheets, textbooks, and computer architecture.
- **40% Hands-On Coding & Hardware:** Building register drivers, soldering, wiring breadboards, and writing firmware.
- **20% Debugging & Documentation:** Probing signals with logic analyzers/scopes, writing clean READMEs, and committing to Git.

```
┌────────────────────────────────────────────────────────────────────────────────────────┐
│ MONTH 1: C, Embedded C, Electronics & Git                                              │
│ - Weeks 1-2: Pointers, memory model, bitwise operations, build process, MISRA-C.        │
│ - Weeks 3-4: Ohm's law, pull-ups, MOSFET switches, DMM, oscilloscope, logic analyzer. │
│ 🎯 Milestone: C Exercises (Circular Buffer, FSM) + [Project 1: GPIO Control Board]     │
├────────────────────────────────────────────────────────────────────────────────────────┤
│ MONTH 2: STM32 Architecture, GPIO, Timers & UART                                      │
│ - Weeks 5-6: ARM Cortex-M4 registers, vector table, startup code, linker scripts.      │
│ - Weeks 7-8: RCC clock trees, PLL configuration, GPIO registers, SysTick, UART.        │
│ 🎯 Milestone: [Project 2: UART Command Console with Ring Buffer & CLI Parser]          │
├────────────────────────────────────────────────────────────────────────────────────────┤
│ MONTH 3: SPI, I2C, ADC, PWM, DMA & Hardware Debugging                                  │
│ - Weeks 9-10: SPI Master transactions (W25Qxx Flash), I2C sensor driver (BMP280).      │
│ - Weeks 11-12: Timers, PWM generation, ADC sampling, DMA streaming, bus recovery.     │
│ 🎯 Milestone: [Project 3: Sensor Data Logger] + [Project 4: PWM Motor Controller]      │
├────────────────────────────────────────────────────────────────────────────────────────┤
│ MONTH 4: FreeRTOS, Concurrency, Synchronization & Faults                              │
│ - Weeks 13-14: Preemptive kernel mechanics, TCB, PendSV context switch, Queues.        │
│ - Weeks 15-16: Mutexes with Priority Inheritance, Semaphores, HardFault analysis.      │
│ 🎯 Milestone: [Project 5: FreeRTOS Multitasking Environmental Monitor]                 │
├────────────────────────────────────────────────────────────────────────────────────────┤
│ MONTH 5: Embedded Linux & Specialization Track                                         │
│ - Weeks 17-18: Embedded Linux boot flow, cross-toolchains, Device Trees, Buildroot.    │
│ - Weeks 19-20: Kernel character drivers + Start Specialization Track (Auto/IoT/TinyML).│
│ 🎯 Milestone: Boot Linux in QEMU, insert custom kernel driver, build track prototype.  │
├────────────────────────────────────────────────────────────────────────────────────────┤
│ MONTH 6: Advanced Portfolio Capstone, Testing & Interview Prep                         │
│ - Weeks 21-22: Complete flagship capstone (Automotive CAN, Secure IoT, or TinyML).     │
│ - Weeks 23-24: Testing, documentation, resume engineering, whiteboard interview prep.  │
│ 🎯 Milestone: Production-ready GitHub portfolio + 3 completed flagship repositories!    │
└────────────────────────────────────────────────────────────────────────────────────────┘
```
