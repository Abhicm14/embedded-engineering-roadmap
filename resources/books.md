# 📘 Curated Embedded Systems Books

> Evaluated textbooks categorized by domain and rated using the Roadmap B convention (👶 Beginner-friendly, 💎 In-depth reference).

---

## 1. Embedded C & Firmware Architecture

- 📘 👶 [**Making Embedded Systems**](https://www.oreilly.com/library/view/making-embedded-systems/9781449308889/) by Elecia White (O'Reilly)  
  *Focus:* Architecture patterns, memory constraints, watchdog design, state machines, and hardware-software contracts. The best conceptual bridge from general programming to embedded firmware thinking.
- 📘 💎 [**The C Programming Language (2nd Edition)**](https://en.wikipedia.org/wiki/The_C_Programming_Language) by Brian W. Kernighan & Dennis M. Ritchie  
  *Focus:* The definitive reference for C syntax, pointer mechanics, structs, unions, bitwise operators, and standard library fundamentals.
- 📘 💎 [**Test-Driven Development for Embedded C**](https://pragprog.com/titles/jgade/test-driven-development-for-embedded-c/) by James W. Grenning  
  *Focus:* Writing modular, testable C code using Unity and CppUTest; mocking hardware registers to run fast unit test suites off-target on host PCs.
- 📘 💎 [**Practical UML Statecharts in C/C++ (2nd Edition)**](https://www.state-machine.com/psicc2/) by Miro Samek  
  *Focus:* Active object design pattern, event-driven reactive firmware, and hierarchical state machines for deterministic real-time systems.

---

## 2. Microcontroller Architecture & Bare-Metal STM32

- 📘 💎 [**The Definitive Guide to ARM Cortex-M3 and Cortex-M4 Processors**](https://www.elsevier.com/books/the-definitive-guide-to-arm-cortex-m3-and-cortex-m4-processors/yiu/978-0-12-408082-9) by Joseph Yiu  
  *Focus:* Core registers, NVIC mechanics, exception entry/exit hardware stacking, fault status registers (CFSR/HFSR/BFAR), memory protection unit (MPU), and assembly instructions. Written by a Senior Principal Engineer at ARM.
- 📘 👶 [**Mastering STM32 (2nd Edition)**](https://www.carminenoviello.com/mastering-stm32/) by Carmine Noviello  
  *Focus:* Practical guide to STM32 microcontrollers, GNU arm-none-eabi toolchains, CubeMX, peripheral HAL/LL drivers, FreeRTOS integration, and step-by-step flashing workflows.
- 📘 💎 [**Embedded Systems: Real-Time Interfacing to ARM Cortex-M Microcontrollers**](https://users.ece.utexas.edu/~valvano/) by Jonathan Valvano  
  *Focus:* Rigorous university-level textbook covering ADC conversion, Nyquist theorem, timer capture, analog sensor calibration, and serial bus protocols.

---

## 3. Real-Time Operating Systems (RTOS)

- 📘 👶 [**Mastering the FreeRTOS Real Time Kernel**](https://www.freertos.org/Documentation/RTOS_book.html) by Richard Barry (Official FreeRTOS Guide)  
  *Focus:* Task management, queues, semaphores, mutexes with Priority Inheritance, software timers, event groups, and FreeRTOS memory allocation models (`heap_1` to `heap_5`).
- 📘 💎 [**Real-Time Concepts for Embedded Systems**](https://www.amazon.com/Real-Time-Concepts-Embedded-Systems-Qing/dp/1578201241) by Qing Li with Caroline Yao  
  *Focus:* Deep mathematical and architectural treatment of real-time scheduling (Rate-Monotonic, Earliest Deadline First), context switching, inter-process communication, and unbounded priority inversion.

---

## 4. Embedded Linux & Kernel Drivers

- 📘 💎 [**Linux Device Drivers (3rd Edition)**](https://lwn.net/Kernel/LDD3/) by Jonathan Corbet, Alessandro Rubini, Greg Kroah-Hartman  
  *Focus:* The foundational classic for writing character drivers, memory mapping, locking primitives (spinlocks/mutexes), interrupts, and device abstractions. Available free online.
- 📘 👶 [**Mastering Embedded Linux Programming (3rd Edition)**](https://www.packtpub.com/product/mastering-embedded-linux-programming-third-edition/9781789530384) by Chris Simmonds  
  *Focus:* Cross-toolchains, U-Boot bootloader, Flattened Device Trees (DTS), Linux kernel configuration, Buildroot, Yocto Project (Poky), and debugging with GDB.
- 📘 💎 [**Linux Kernel Development (3rd Edition)**](https://www.amazon.com/Linux-Kernel-Development-Robert-Love/dp/0672329468) by Robert Love  
  *Focus:* Readable breakdown of Linux kernel internal subsystems: process scheduler (CFS), virtual memory management, system calls, and interrupt top/bottom halves.

---

## 5. Electronics & Hardware Design

- 📘 💎 [**The Art of Electronics (3rd Edition)**](https://artofelectronics.net/) by Paul Horowitz & Winfield Hill  
  *Focus:* The bible of electronic circuit design. Transistors, op-amps, passive filters, power regulation, high-speed signal integrity, and low-noise measurements.
- 📘 👶 [**Practical Electronics for Inventors (4th Edition)**](https://www.amazon.com/Practical-Electronics-Inventors-Fourth-Scherz/dp/1259587541) by Paul Scherz & Simon Monk  
  *Focus:* Intuitive, visually rich explanations of passive components, diodes, MOSFETs, regulators, sensors, and practical workbench debugging.
