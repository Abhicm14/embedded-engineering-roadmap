# 📋 Embedded Engineering Interview Checklist

> Comprehensive self-assessment checklist before entering technical interviews.

---

## 1. Embedded C & Language Mechanics
- [ ] Can explain `volatile` and give 3 distinct embedded scenarios requiring it.
- [ ] Understands the difference between `const char *p`, `char * const p`, and `const char * const p`.
- [ ] Can calculate struct padding, memory alignment rules, and understand `#pragma pack(1)`.
- [ ] Knows how `.data`, `.bss`, `.rodata`, `.text`, stack, and heap map to Flash and RAM.
- [ ] Can explain why dynamic memory allocation (`malloc`/`free`) is forbidden in safety-critical firmware.
- [ ] Can write bit manipulation macros for setting, clearing, toggling, and reading bitfields.
- [ ] Knows how to implement an atomic read-modify-write on ARM using exclusive access (`LDREX`/`STREX`) or `BSRR`.

---

## 2. Microcontroller & Computer Architecture
- [ ] Can draw and explain the ARM Cortex-M power-on boot sequence from Reset vector to `main()`.
- [ ] Knows the difference between Harvard and von Neumann bus architectures.
- [ ] Can explain the role of `_sidata`, `_sdata`, `_sbss`, and `_estack` in a GNU linker script (`.ld`).
- [ ] Understands Flash wait-states and why boosting CPU clock without setting `FLASH_ACR` causes a HardFault.
- [ ] Can describe the hardware stacking mechanism when an interrupt is entered (the 8 pushed registers).
- [ ] Knows how to read `CFSR` and `BFAR` registers to diagnose BusFaults and unaligned memory accesses.

---

## 3. Communication Protocols & Hardware
- [ ] Can compare UART, SPI, I2C, and CAN in terms of line count, topology, speed, and duplex.
- [ ] Can draw the 4 SPI modes (CPOL/CPHA) and identify sampling transitions.
- [ ] Can explain how I2C open-drain lines work and calculate pull-up resistor limits ($R_{p(max)}$).
- [ ] Can describe the 9-clock I2C bus lockup recovery routine.
- [ ] Can explain non-destructive bitwise arbitration on CAN bus (dominant '0' vs recessive '1').
- [ ] Understands the difference between $1\text{X}$ and $10\text{X}$ oscilloscope probes.
- [ ] Knows why decoupling capacitors (100nF) must be placed close to MCU power pins.

---

## 4. Real-Time Operating Systems (RTOS)
- [ ] Can describe what a Task Control Block (TCB) contains.
- [ ] Explains how a context switch executes via `PendSV` and SysTick.
- [ ] Understands the difference between a Binary Semaphore (signaling) and a Mutex (resource locking).
- [ ] Can explain Unbounded Priority Inversion and how Priority Inheritance Protocol (PIP) prevents it.
- [ ] Knows why calling blocking functions or standard FreeRTOS APIs in an ISR causes a crash.
- [ ] Understands how Tickless Idle puts the MCU into low-power sleep during idle periods.

---

## 5. Embedded Linux & System Architecture
- [ ] Can describe the 4-stage boot sequence: ROM Bootloader $\rightarrow$ SPL $\rightarrow$ U-Boot $\rightarrow$ Kernel $\rightarrow$ Init.
- [ ] Knows what a Device Tree Source (`.dts`) is and why hardware description is separated from kernel C.
- [ ] Understands user space vs kernel space memory isolation.
- [ ] Can write a minimal Linux character device driver using `file_operations` and `copy_to_user()`.
- [ ] Knows how to toggle GPIO lines from user space using modern `libgpiod`.

---

## 6. Whiteboard Coding Challenges
- [ ] Can code a lock-free circular FIFO ring buffer in C without syntax errors.
- [ ] Can implement a non-blocking switch debounce state machine.
- [ ] Can write a fast 32-bit integer bit reversal function.
- [ ] Can write a CRC-16 or CRC-32 polynomial computation loop.
- [ ] Can implement a tokenizing serial CLI command parser.
