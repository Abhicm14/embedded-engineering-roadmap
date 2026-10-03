# 🗺️ Complete 6-Month Embedded Engineering Roadmap (24 Weeks)

> A granular, week-by-week curriculum designed for undergraduate students, freshers, and engineers transitioning into embedded firmware and systems engineering.

---

## 📅 Roadmap Overview at a Glance

```
┌─────────────────────────────────────────────────────────────────────────────────────────────────┐
│ MONTH 1: Embedded C Foundations & Electronics Physics                                           │
│ Weeks 1-4: Bitwise, Pointers, Memory Model, C Standard, Circuit Theory, Probing & Oscilloscopes │
├─────────────────────────────────────────────────────────────────────────────────────────────────┤
│ MONTH 2: Microcontroller Architecture & Bare-Metal STM32                                       │
│ Weeks 5-8: Cortex-M Internals, Reset Vector, Linker Script, Makefiles, RCC Clock Trees, GPIO    │
├─────────────────────────────────────────────────────────────────────────────────────────────────┤
│ MONTH 3: Hardware Peripherals, Serial Buses & DMA                                               │
│ Weeks 9-12: UART with Ring Buffers, SPI Engines, I2C State Machines, Timers, PWM, NVIC & DMA    │
├─────────────────────────────────────────────────────────────────────────────────────────────────┤
│ MONTH 4: Real-Time Operating Systems (RTOS Internals)                                           │
│ Weeks 13-16: Scheduler Internals, TCB, Context Switching, Queues, Mutexes, Semaphores, FreeRTOS │
├─────────────────────────────────────────────────────────────────────────────────────────────────┤
│ MONTH 5: Embedded Linux & System Architecture                                                   │
│ Weeks 17-20: Cross-Toolchains, Bootloaders (U-Boot), Device Trees, Kernel Modules, Sysfs, POSIX │
├─────────────────────────────────────────────────────────────────────────────────────────────────┤
│ MONTH 6: Advanced Industry Specializations                                                      │
│ Weeks 21-24: Automotive CAN/AUTOSAR, Secure IoT & OTA, TinyML on Cortex-M, Technical Interviews │
└─────────────────────────────────────────────────────────────────────────────────────────────────┘
```

---

## 🟢 Month 1: Advanced Embedded C & Electronics Foundations

### Week 1: Low-Level C Mechanics & Bitwise Operations
- **Theory & Deep Concepts:**
  - Integer representation, two's complement, sign extension, and integer promotion rules (C99 / C11).
  - Bitwise operators: AND (`&`), OR (`|`), XOR (`^`), NOT (`~`), Left Shift (`<<`), Right Shift (`>>`).
  - Creating masks, clearing bits, setting bits, toggling bits, testing bits, extracting bitfields.
  - Endianness (Little-Endian vs. Big-Endian), byte-swapping algorithms, network byte order (`htons`, `ntohl`).
  - Bit-fields in structs vs. explicit bitwise masks: why bit-fields are often non-portable across compilers.
- **Hands-on Lab:**
  - Build a pure C virtual register emulator (`06-code-examples/bit-manipulation-snippets/`).
  - Implement MAC/IP checksum calculator and endianness conversion macros.
- **Deliverable:**
  - Tested header file containing rock-solid bitwise manipulation macros used across all firmware projects.

### Week 2: Pointers, Memory Layout & Type Qualifiers
- **Theory & Deep Concepts:**
  - Process memory segments: `.text` (Flash), `.rodata`, `.data` (RAM initialized), `.bss` (RAM zeroed), `.stack`, and `.heap`.
  - Pointer arithmetic, generic pointers (`void*`), multi-dimensional arrays, function pointers for callbacks and jump tables.
  - Type qualifiers:
    - `volatile`: Compiler optimization barriers, hardware register access, shared variables in ISRs, signal flags.
    - `const`: Flash placement of read-only lookup tables, defensive API contracts.
    - `restrict`: Informing compiler of pointer aliasing for aggressive SIMD/register optimization.
  - Memory alignment and struct packing: `#pragma pack(1)` vs `__attribute__((packed))`, memory padding penalties on 32-bit buses.
- **Hands-on Lab:**
  - Write a circular FIFO ring buffer using pointers with zero data copying.
  - Inspect compiled ELF file sections using `objdump -h` and `size`.
- **Deliverable:**
  - Ring buffer implementation with unit tests in C (`06-code-examples/ring-buffer-uart/`).

### Week 3: Electrical Engineering Foundations for Firmware Engineers
- **Theory & Deep Concepts:**
  - Ohm’s Law ($V = IR$), Kirchhoff’s Current & Voltage Laws (KCL / KVL).
  - Passive components: Resistors (pull-up, pull-down, current-limiting for LEDs), Capacitors (decoupling/bypass, filter, charging curves $\tau = RC$), Inductors.
  - Active components: Diodes (forward drop, Schottky, Zener protection), BJT vs. N-channel / P-channel MOSFETs (high-side vs low-side switching, gate threshold $V_{GS(th)}$).
  - Op-Amps: Inverting/Non-inverting amplifiers, voltage followers, comparators.
  - Power supplies: Linear Low-Dropout Regulators (LDOs) vs. Switched-Mode Power Supplies (Buck/Boost converters), thermal dissipation, ripple.
- **Hands-on Lab:**
  - Calculate pull-up resistor values for $100\text{ kHz}$ and $400\text{ kHz}$ I2C lines based on bus capacitance.
  - Design a MOSFET low-side switch circuit to drive a $12\text{V}$ relay from a $3.3\text{V}$ MCU GPIO pin.
- **Deliverable:**
  - Schematic calculation sheet and breadboard circuit verification.

### Week 4: Lab Instruments & Signal Integrity
- **Theory & Deep Concepts:**
  - Digital Multimeter (DMM): Continuity checking, measuring low-value shunt resistors, measuring sleep current ($\mu\text{A}$).
  - Digital Storage Oscilloscope (DSO): Bandwidth rules (5x knee frequency), sampling rate, AC vs DC coupling, probe attenuation ($1\text{X}$ vs $10\text{X}$ capacitance loading), trigger modes (Edge, Pulse Width, runt).
  - Logic Analyzers: Protocol decoding (UART, SPI, I2C), sampling theorem ($F_s \ge 4 \times F_{bus}$).
  - Signal integrity basics: Ringing, ground bounce, crosstalk, decoupling capacitor placement.
- **Hands-on Lab:**
  - Connect a 24MHz logic analyzer clone using open-source **PulseView** (sigrok). Capture and decode a noisy UART transmission.
- **Deliverable:**
  - Protocol capture traces (.sr file) and signal integrity debugging notes.

---

## 🔵 Month 2: Microcontroller Architecture & Bare-Metal STM32

### Week 5: ARM Cortex-M Architecture & Execution Model
- **Theory & Deep Concepts:**
  - ARMv7-M / ARMv8-M architecture: Core registers (`R0-R12`, `SP` / `MSP` / `PSP`, `LR`, `PC`, `xPSR`).
  - Operation modes: Thread Mode vs Handler Mode; Privileged vs Unprivileged execution.
  - Vector Table layout: Initial Stack Pointer address, Reset Handler, NMI, HardFault, MemManage, BusFault, UsageFault, SVCall, PendSV, SysTick.
  - The Boot Sequence: Reset signal $\rightarrow$ load MSP $\rightarrow$ load PC with `Reset_Handler` $\rightarrow$ copy `.data` from Flash to RAM $\rightarrow$ zero `.bss` $\rightarrow$ call `main()`.
- **Hands-on Lab:**
  - Write a bare-metal assembly / minimal C startup file (`startup.c`) without any ST HAL or CMSIS headers.
- **Deliverable:**
  - Working startup file initializing stack and calling `main()` on an STM32F401/F411.

### Week 6: Toolchains, Linker Scripts & Makefiles
- **Theory & Deep Concepts:**
  - The GNU Arm Embedded Toolchain: `arm-none-eabi-gcc`, `arm-none-eabi-as`, `arm-none-eabi-ld`, `arm-none-eabi-objcopy`, `arm-none-eabi-gdb`.
  - Linker Script (`.ld`) syntax:
    - `MEMORY` command (defining `FLASH` origin/length and `RAM` origin/length).
    - `SECTIONS` command (mapping `.text`, `.rodata`, `.data`, `.bss`, `_sdata`, `_edata`, `_sbss`, `_ebss`, `_estack`).
    - Symbols vs Variables in linker scripts.
  - Writing clean, modular `Makefile` for embedded targets with automated dependency generation (`-MMD -MP`).
- **Hands-on Lab:**
  - Compile, link, and generate a `.bin` and `.hex` firmware binary completely from command line using Make.
- **Deliverable:**
  - Clean `Makefile` and `linker.ld` template in `06-code-examples/baremetal-stm32f4-blinky/`.

### Week 7: Memory-Mapped I/O & Clock Trees (RCC)
- **Theory & Deep Concepts:**
  - The Bus Matrix: AHB (Advanced High-performance Bus) vs APB (Advanced Peripheral Bus), bridge prescalers.
  - STM32 Reset and Clock Control (RCC): Internal RC oscillator (HSI), External crystal (HSE), Phase-Locked Loop (PLL) multipliers and dividers.
  - Memory-mapped peripheral registers: Base addresses, offset registers, peripheral memory boundary calculation.
  - Writing header files with `struct` memory maps and pointer dereferencing: `#define GPIOA ((GPIO_TypeDef *) 0x40020000UL)`.
- **Hands-on Lab:**
  - Configure the PLL from scratch to boost the STM32F4 core clock from 16 MHz HSI to 84 MHz / 100 MHz.
  - Read Flash latency wait-states requirement in the reference manual and configure `FLASH_ACR`.
- **Deliverable:**
  - Working bare-metal clock initialization driver.

### Week 8: Bare-Metal GPIO & Project 1
- **Theory & Deep Concepts:**
  - GPIO register internals: `MODER` (Input, Output, Alternate Function, Analog), `OTYPER` (Push-Pull vs Open-Drain), `OSPEEDR`, `PUPDR` (Pull-up, Pull-down, Floating), `BSRR` (Atomic bit set/reset) vs `ODR`.
  - SysTick timer: 24-bit down-counter, calibration, reload register, tick interrupt calculation.
  - Finite State Machine (FSM) architecture for switch debouncing (Moore vs Mealy).
- **Hands-on Lab:**
  - Complete **Project 1**: [Bare-Metal GPIO & SysTick FSM Driver](04-portfolio-projects/project-01-baremetal-gpio-systick/).
- **Deliverable:**
  - Fully tested Project 1 running on STM32 hardware or Wokwi simulator.

---

## 🟡 Month 3: Embedded Communication Protocols & Peripherals

### Week 9: UART / USART with Circular DMA
- **Theory & Deep Concepts:**
  - Asynchronous serial framing: Start bit, 8 data bits, Parity bit, Stop bit(s), Baud rate generation ($USARTDIV$).
  - Overrun errors, framing errors, noise detection.
  - Polling vs Interrupt-driven RX/TX vs DMA (Direct Memory Access).
  - Ring buffer (circular queue) data structure for non-blocking stream processing.
  - Hardware Flow Control: RTS / CTS line signaling.
- **Hands-on Lab:**
  - Build an interactive command-line interface (CLI) over UART with command parsing (`help`, `led on`, `status`).
- **Deliverable:**
  - Circular buffer DMA UART driver handling high-speed bursts without dropping bytes.

### Week 10: Serial Peripheral Interface (SPI)
- **Theory & Deep Concepts:**
  - Synchronous 4-wire bus: MOSI, MISO, SCK, CS/SS.
  - Clock Polarity (`CPOL`) and Clock Phase (`CPHA`) - The 4 SPI Modes (Mode 0, 1, 2, 3).
  - Daisy chaining vs independent slave select lines.
  - Timing constraints: setup time, hold time, maximum clock frequency.
  - Interfacing with SPI Flash memories (Winbond W25Qxx) and SPI Displays (ST7789 / ILI9341).
- **Hands-on Lab:**
  - Interface an SPI peripheral (or flash memory), read JEDEC manufacturer ID over SPI.
- **Deliverable:**
  - Production SPI master driver in pure C with transaction abstraction.

### Week 11: Inter-Integrated Circuit (I2C) & Bus Arbitration
- **Theory & Deep Concepts:**
  - Synchronous 2-wire open-drain bus: SDA and SCL.
  - Pull-up resistor dimensioning: rise time constraints ($t_r \le 1000\text{ ns}$ for standard mode, $300\text{ ns}$ for fast mode).
  - Bus conditions: START, STOP, REPEATED START, 7-bit / 10-bit addressing, ACK / NACK.
  - Clock stretching, multi-master arbitration, bus lockup conditions and the 9-clock-cycle recovery sequence.
- **Hands-on Lab:**
  - Complete **Project 2**: [I2C Sensor Driver & DMA UART Shell](04-portfolio-projects/project-02-i2c-sensor-uart-dma/) reading temperature/pressure registers from BMP280.
- **Deliverable:**
  - Complete Project 2 with driver error recovery and bus unlock routine.

### Week 12: Timers, Input Capture, PWM & Nested Vectored Interrupt Controller (NVIC)
- **Theory & Deep Concepts:**
  - Advanced vs General Purpose vs Basic Timers: Prescalers, Auto-Reload Registers (`ARR`), Counter Modes (Up, Down, Center-aligned).
  - Pulse Width Modulation (PWM) generation: Duty cycle, frequency, dead-time insertion for motor H-bridges.
  - Input capture: measuring pulse width and frequency of external signals.
  - ARM NVIC: Priority grouping (Preemption Priority vs Subpriority), tail-chaining, late arrival, interrupt latency.
  - Interrupt Service Routine (ISR) design rules: reentrancy, minimizing ISR execution time, deferred processing.
- **Hands-on Lab:**
  - Generate smooth PWM for LED breathing and motor speed; capture external square wave frequency with timer input capture.
- **Deliverable:**
  - Timer and NVIC lab firmware with interrupt latency benchmark.

---

## 🟣 Month 4: Real-Time Operating Systems (FreeRTOS)

### Week 13: RTOS Fundamentals & Scheduler Architecture
- **Theory & Deep Concepts:**
  - Why RTOS? Superloop (Bare-metal) limitations vs Deterministic multi-tasking.
  - Preemptive vs Cooperative scheduling; Round-Robin and Priority-based scheduling.
  - Task Control Block (TCB), Task Stacks, Stack Overflow detection methods.
  - The Context Switch: Saving CPU registers to task stack via `PendSV` exception, loading next task's stack pointer.
  - SysTick timer role in RTOS time slicing; Task States: Running, Ready, Blocked, Suspended.
- **Hands-on Lab:**
  - Port FreeRTOS kernel to STM32; configure `FreeRTOSConfig.h` memory allocations and tick rate ($1000\text{ Hz}$).
- **Deliverable:**
  - Two concurrent blinking tasks with different priorities demonstrating preemption.

### Week 14: Inter-Process Communication (IPC): Queues & Ring Buffers
- **Theory & Deep Concepts:**
  - Queue mechanics: pass-by-copy vs pass-by-reference.
  - Blocking with timeouts (`portMAX_DELAY`), queue peek, queue from ISR (`xQueueSendFromISR`).
  - Producer-Consumer design pattern in embedded architectures.
  - Memory isolation and thread-safe data pipelines.
- **Hands-on Lab:**
  - Build an asynchronous event broker: Sensor sampling task produces data packets $\rightarrow$ Queue $\rightarrow$ Processing task logs to serial.
- **Deliverable:**
  - Fully decoupled producer-consumer implementation (`06-code-examples/freertos-producer-consumer/`).

### Week 15: Synchronization: Semaphores, Mutexes & Priority Inversion
- **Theory & Deep Concepts:**
  - Binary Semaphores (event signaling) vs Counting Semaphores (resource counting).
  - Mutexes (mutual exclusion) with ownership.
  - The Classic Hazard: **Priority Inversion** and how Mars Pathfinder suffered from it.
  - Solution: **Priority Inheritance Protocol (PIP)** and Ceiling Priority Protocol.
  - Deadlocks, livelocks, and recursive mutexes.
- **Hands-on Lab:**
  - Replicate priority inversion in code, observe the starvation, and resolve it using FreeRTOS mutex with priority inheritance.
- **Deliverable:**
  - Code experiment and timing diagram illustrating priority inheritance in action.

### Week 16: Advanced RTOS Features & Project 3
- **Theory & Deep Concepts:**
  - Event Groups (bit flags for multi-event synchronization: wait for ALL or wait for ANY).
  - Direct-to-Task Notifications: zero-RAM-overhead lightweight signaling (faster than semaphores).
  - Software Timers: Daemon task execution, one-shot vs auto-reload.
  - FreeRTOS Memory Management models: `heap_1.c` through `heap_5.c` (fragmentation risks).
  - Low Power: Tickless Idle mode (`configUSE_TICKLESS_IDLE`).
- **Hands-on Lab:**
  - Complete **Project 3**: [Multitasking Environmental Data Logger](04-portfolio-projects/project-03-freertos-data-logger/).
- **Deliverable:**
  - Complete multi-threaded data logger code with OLED display and SD card SPI writing.

---

## 🟠 Month 5: Embedded Linux & System Architecture

### Week 17: Embedded Linux Architecture & Cross-Compilation
- **Theory & Deep Concepts:**
  - Microcontroller vs Microprocessor (MMU, caches, virtual memory).
  - Embedded Linux boot sequence: ROM Bootloader $\rightarrow$ SPL (Secondary Program Loader) $\rightarrow$ U-Boot $\rightarrow$ Linux Kernel $\rightarrow$ Root Filesystem (`init` / `systemd`).
  - Toolchains: Cross-compilers (`arm-linux-gnueabihf-gcc`, `aarch64-linux-gnu-gcc`), C standard libraries (glibc vs musl vs uClibc).
  - Root Filesystem structures: Buildroot vs Yocto Project concepts.
- **Hands-on Lab:**
  - Setup a Linux cross-compilation environment and build a minimal custom rootfs with BusyBox for QEMU ARM.
- **Deliverable:**
  - Bootable minimal Linux kernel image running inside QEMU.

### Week 18: U-Boot & The Flattened Device Tree (FDT)
- **Theory & Deep Concepts:**
  - U-Boot environment variables, boot commands (`bootm`, `bootz`, `tftp`, `mmc read`).
  - What is a Device Tree? Separation of hardware description from kernel C source.
  - Device Tree Source (`.dts`), Includes (`.dtsi`), and Device Tree Blob compiler (`dtc`).
  - Nodes, properties, address cells, size cells, interrupt mapping, `compatible` strings.
- **Hands-on Lab:**
  - Write a custom DTS node for an external I2C temperature sensor and compile it to `.dtb`.
- **Deliverable:**
  - Documented Device Tree overlay and verification log.

### Week 19: Linux Kernel Modules & Character Device Drivers
- **Theory & Deep Concepts:**
  - User space vs Kernel space: virtual memory isolation, system call interface (`open`, `read`, `write`, `ioctl`, `close`).
  - Linux Kernel Module (LKM) architecture: `module_init()`, `module_exit()`, `MODULE_LICENSE("GPL")`.
  - Major and Minor device numbers; `cdev` structure and `file_operations` struct.
  - Data transfer across boundary: `copy_to_user()` and `copy_from_user()`.
  - Kernel concurrency: spinlocks, mutexes, atomic variables.
- **Hands-on Lab:**
  - Write, cross-compile, and insert a custom character device driver `/dev/my_device` and test it with a user-space C app.
- **Deliverable:**
  - Functional character driver code with Makefile (`04-portfolio-projects/project-05-embedded-linux-driver/`).

### Week 20: Hardware Control: Sysfs, GPIO, PWM & Project 5
- **Theory & Deep Concepts:**
  - Modern Linux GPIO subsystems: legacy sysfs vs `libgpiod` (character device `gpiochip`).
  - Linux Industrial I/O (IIO) subsystem for ADCs, DACs, and IMUs.
  - Kernel PWM subsystem.
  - User-space POSIX programming: `pthreads`, condition variables, POSIX queues, UNIX domain sockets.
- **Hands-on Lab:**
  - Complete **Project 5**: [Embedded Linux Custom Device Driver](04-portfolio-projects/project-05-embedded-linux-driver/).
- **Deliverable:**
  - Full kernel module and user-space control application.

---

## 🔴 Month 6: Advanced Specializations & Portfolio Polish

Choose one or explore all three industry tracks:

### Week 21: Track A - Automotive Embedded Systems (CAN, OBD-II, AUTOSAR)
- **Theory & Deep Concepts:**
  - Controller Area Network (CAN 2.0A/B & CAN-FD): Differential signaling (CAN_H, CAN_L), recessive vs dominant bits.
  - Arbitration by identifier, non-destructive bitwise arbitration.
  - CAN frame types: Data Frame, Remote Frame, Error Frame, Overload Frame.
  - Bit stuffing, CRC calculation, ACK delimiter.
  - Hardware acceptance filtering and mask registers.
  - Diagnostic protocols: OBD-II PIDs and UDS (Unified Diagnostic Services, ISO 14229).
  - AUTOSAR architecture overview: Classic vs Adaptive platform, MCAL, BSW, RTE, and Application layer.
  - Functional Safety: ISO 26262 ASIL levels (A through D), MISRA-C 2012 compliance.
- **Hands-on Lab:**
  - Complete **Project 4**: [Automotive CAN Bus Gateway & Telemetry](04-portfolio-projects/project-04-automotive-can-node/).
- **Deliverable:**
  - Functional CAN transceiver node parsing simulated vehicle telemetry.

### Week 22: Track B - IoT, Wireless & Embedded Security
- **Theory & Deep Concepts:**
  - Wireless protocols: Wi-Fi (802.11 b/g/n), BLE (GATT server/client, advertising, services, characteristics), LoRaWAN.
  - Application protocols: MQTT (QoS 0, 1, 2, keep-alive, Last Will and Testament), CoAP, HTTP/REST.
  - Embedded Cryptography & Security:
    - Symmetric (AES-128/256-GCM) vs Asymmetric (RSA, ECC / Curve25519).
    - Hashing (SHA-256) and Message Authentication Codes (HMAC).
    - TLS 1.3 handshake on constrained microcontrollers (mbedTLS).
    - Hardware Root of Trust, Secure Boot, Flash encryption.
    - Robust Over-The-Air (OTA) updates: Dual-bank A/B partitioning with automatic rollback on boot failure.
- **Hands-on Lab:**
  - Complete **Project 6**: [Secure Edge IoT Client with MQTT & OTA](04-portfolio-projects/project-06-secure-iot-mqtt-ota/).
- **Deliverable:**
  - Secure IoT client publishing encrypted sensor data with verified OTA rollback.

### Week 23: Track C - Edge AI / TinyML on Microcontrollers
- **Theory & Deep Concepts:**
  - Why Machine Learning on edge MCUs? Privacy, zero latency, offline operation, sub-milliwatt power.
  - Model compression pipeline: Quantization (Float32 $\rightarrow$ INT8), Pruning, Knowledge Distillation.
  - Frameworks: TensorFlow Lite for Microcontrollers (TFLM), Edge Impulse, ARM CMSIS-NN kernels.
  - DSP preprocessing: Fast Fourier Transforms (FFT), Mel-Frequency Cepstral Coefficients (MFCC), sliding window filters.
  - Tensor arena memory allocation on microcontrollers without dynamic RAM (`malloc`).
- **Hands-on Lab:**
  - Complete **Project 7**: [TinyML Real-Time Motion/Gesture Classifier](04-portfolio-projects/project-07-tinyml-gesture-classifier/).
- **Deliverable:**
  - Quantized neural network classifying motion gestures in real-time on Cortex-M4.

### Week 24: Engineering Portfolio, Git Best Practices & Interview Readiness
- **Theory & Practice:**
  - Packaging your 7 projects with clear READMEs, system block diagrams, logic analyzer captures, and build instructions.
  - Git hygiene: Atomic commits, feature branches, semantic versioning, pull request etiquette.
  - Technical Interview Drill:
    - 50+ embedded core questions: [Interview Prep Guide](03-resource-hub/interview-prep.md).
    - Whiteboard coding: circular buffers, bit manipulation, custom `memcpy`, linked list reversal in C.
    - System design interview: Designing a flight controller or medical infusion pump.
- **Deliverable:**
  - Polished GitHub portfolio and readiness for top-tier embedded engineering technical interviews.
