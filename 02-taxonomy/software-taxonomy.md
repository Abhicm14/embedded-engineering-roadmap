# 💻 Software Topic Taxonomy for Embedded Systems

> A detailed taxonomy of software engineering domains, concepts, design patterns, and standards required for embedded firmware and systems development.

---

## 1. Programming Languages & Compilers

### 1.1 Embedded C (C99 / C11)
- **Data Types & Sizes:** Standard fixed-width types (`stdint.h`: `uint8_t`, `int16_t`, `uint32_t`, `uintptr_t`, `size_t`), sign extension, integer overflow/underflow behavior.
- **Type Qualifiers:**
  - `volatile`: Memory-mapped hardware registers, variables shared between ISR and main thread, delay loop counters.
  - `const`: Flash allocation of constant lookup tables, immutable parameters, read-only pointers (`const uint8_t *` vs `uint8_t * const`).
  - `restrict`: Compiler optimization hints for non-overlapping pointers.
  - `static`: File-scope encapsulation, persistent local state in functions.
- **Pointers & Memory Access:**
  - Pointer arithmetic, generic `void*` buffering, function pointers (callbacks, vector tables, state machines).
  - Dereferencing memory-mapped I/O registers: `*(volatile uint32_t *)(0x40020000UL)`.
- **Bitwise Manipulation:** Bit shifting (`<<`, `>>`), bitwise masks, bit toggling (`^=`), clearing (`&= ~`), testing (`if (reg & MASK)`), extraction and insertion macros.
- **Structures, Unions & Bitfields:**
  - Struct alignment, padding, `#pragma pack(1)` vs `__attribute__((packed))`.
  - Type-punning with unions and safe alternative methods (`memcpy`).
  - Limitations of C bit-fields (compiler-dependent ordering and non-atomic access).

### 1.2 Modern Embedded C++ (C++17 / C++20)
- **Zero-Cost Abstractions:** Templates vs Macros, `constexpr` compile-time evaluation, inline functions.
- **Resource Management:** RAII (Resource Acquisition Is Initialization), smart pointers (`std::unique_ptr` with custom deleters), avoiding dynamic memory allocation (`new`/`delete`).
- **Hardware Abstraction:** Class-based peripheral drivers, type-safe register templates, strongly-typed enums (`enum class`).
- **Features to Avoid / Restrict in Bare-Metal:** C++ RTTI (`dynamic_cast`), exceptions (`-fno-exceptions -fno-rtti`), heavy STL containers that use the heap.

### 1.3 Assembly Language (ARM Thumb-2 / RISC-V)
- **Core Registers:** General purpose registers, Program Counter (`PC`), Link Register (`LR`), Stack Pointers (`MSP`/`PSP`).
- **Instruction Categories:** Data processing (`MOV`, `ADD`, `SUB`, `AND`), Memory load/store (`LDR`, `STR`, `LDM`, `STM`), Branching (`B`, `BL`, `BX`), System control (`MSR`, `MRS`, `CPSID`, `CPSIE`, `WFI`, `WFE`).
- **Inline Assembly:** GCC `__asm__ volatile ("..." : output : input : clobbered)` syntax.

### 1.4 Embedded Rust (Emerging Domain)
- **Ownership & Borrowing:** Zero-cost memory safety without a garbage collector, data race prevention at compile time.
- **The `#![no_std]` Ecosystem:** Writing bare-metal firmware without the standard operating system library.
- **Peripheral Access Crates (PAC) & Hardware Abstraction Layers (HAL):** `svd2rust`, `embedded-hal`.

---

## 2. Toolchains, Linkers & Build Systems

### 2.1 The GNU Toolchain (`arm-none-eabi-*`)
- `gcc`: Compiler flags (`-mcpu=cortex-m4`, `-mthumb`, `-mfloat-abi=hard`, `-mfpu=fpv4-sp-d16`, `-Os`, `-Wall`, `-Wextra`, `-Werror`).
- `as` & `ld`: Assembler and Linker.
- `objdump` & `nm`: Inspecting assembly output, symbol tables, and section addresses.
- `size`: Monitoring Flash (`text + data`) and RAM (`data + bss`) consumption.
- `objcopy`: Converting `.elf` output to `.hex` (Intel HEX) and `.bin` (raw binary).

### 2.2 Linker Scripts (`.ld`)
- **Memory Regions:** `MEMORY { FLASH (rx) : ORIGIN = 0x08000000, LENGTH = 512K ... }`.
- **Section Placement:** `.text`, `.rodata`, `.data`, `.bss`, `.stack`, `.heap`.
- **Symbols & Alignment:** `.` (location counter), `ALIGN(4)`, `PROVIDE(_estack = ...)`.
- **Load Memory Address (LMA) vs Virtual Memory Address (VMA):** Relocating initialized data from Flash to RAM during boot.

### 2.3 Build Automation
- **Make:** Targets, prerequisites, recipes, automatic variables (`$@`, `$<`, `$^`), dependency tracking (`-MMD -MP`).
- **CMake:** Cross-compiling toolchain files (`CMAKE_TOOLCHAIN_FILE`), modern target-based CMake (`target_include_directories`, `target_link_libraries`).

---

## 3. Memory Architecture & Management

- **Physical Memory Layout:**
  - Non-Volatile: Internal Flash, External SPI NOR Flash, EEPROM.
  - Volatile: Internal SRAM, CCM (Core Coupled Memory), TCM (Tightly Coupled Memory), External PSRAM/SDRAM.
- **Processor Memory Subsystems:**
  - Cache: Instruction cache (I-Cache), Data cache (D-Cache), Cache coherence, Cache invalidation & clean operations during DMA transfers.
  - Memory Protection Unit (MPU): Defining execution permissions (Privileged vs Unprivileged, Execute Never `XN`, Read-Only vs Read-Write).
  - Memory Management Unit (MMU): Virtual address translation, page tables, TLB (required for Embedded Linux).
- **Embedded Allocation Strategies:**
  - Static Allocation: Global/file-scope structures, zero runtime allocation overhead.
  - Stack Allocation: Frame boundaries, monitoring stack depth, high-water mark tracking (`0xA5` pattern fill).
  - Fixed-Size Block Allocators (Memory Pools): Eliminating fragmentation and guaranteeing $O(1)$ allocation/deallocation time.
  - Dynamic Heap Allocations: Risks of heap fragmentation, why `malloc()` is restricted in safety-critical firmware.

---

## 4. Bare-Metal Architecture & Execution Model

- **Startup Architecture:**
  - Vector Table definition (stack pointer initialization + exception handler addresses).
  - Reset Sequence: Reset pin asserted $\rightarrow$ MSP loaded $\rightarrow$ `Reset_Handler` executes.
  - C Runtime Startup (`crt0`): Copying `.data` from Flash to RAM, zeroing `.bss`, calling static C++ constructors, branching to `main()`.
- **Interrupt Handling & NVIC:**
  - Vector table relocation (Vector Table Offset Register `VTOR`).
  - Preemption priority vs Subpriority.
  - Context saving/restoring hardware behavior (hardware stacking of `R0-R3, R12, LR, PC, xPSR`).
  - Tail-chaining and late-arrival interrupt latency optimizations.
  - Reentrancy and nested interrupt execution rules.
- **Concurrency & Critical Sections:**
  - Interrupt disabling: `__disable_irq()` / `__enable_irq()`.
  - Atomic operations and memory barriers: `__DMB()` (Data Memory Barrier), `__DSB()` (Data Synchronization Barrier), `__ISB()` (Instruction Synchronization Barrier).
  - Exclusive access instructions: `LDREX` / `STREX`.

---

## 5. Real-Time Operating Systems (RTOS)

- **Kernel Concepts:**
  - Real-Time definition: Hard real-time (deterministic deadlines) vs Soft real-time (best effort).
  - Preemptive vs Cooperative multitasking.
  - Task Control Block (TCB), Task State Machine (Ready, Running, Blocked, Suspended).
  - Schedulers: Priority-based, Round-Robin, Rate-Monotonic Scheduling (RMS), Earliest Deadline First (EDF).
- **Synchronization & IPC:**
  - Queues: Thread-safe FIFO data passing, passing values vs passing pointers.
  - Semaphores: Binary semaphores (signaling), counting semaphores (resource counting).
  - Mutexes: Ownership semantics, recursive mutexes.
  - Priority Inversion Hazard: Unbounded priority inversion, Priority Inheritance Protocol (PIP).
  - Event Groups: Multi-bit synchronization flags (wait-for-any, wait-for-all).
  - Direct Task Notifications: Fast, lightweight unblock signals with zero RAM overhead.
- **System Services:**
  - Software Timers: One-shot vs auto-reload, timer daemon task.
  - Memory Management: FreeRTOS `heap_1` to `heap_5`.
  - Low-Power / Tickless Idle: Suppressing SysTick interrupts and sleeping until the next scheduled event.

---

## 6. Embedded Linux & System Architecture

- **The Embedded Linux Stack:**
  - ROM Bootloader $\rightarrow$ SPL $\rightarrow$ U-Boot $\rightarrow$ Kernel (`zImage`/`uImage`) $\rightarrow$ Root Filesystem (`ext4`, `ubifs`, `initramfs`).
- **Device Trees:**
  - Device Tree Source (`.dts`, `.dtsi`), Device Tree Compiler (`dtc`), Device Tree Blob (`.dtb`).
  - Bus nodes (I2C, SPI, UART, PCIe), GPIO controllers, pin multiplexing (`pinctrl`), power management nodes.
- **Kernel Subsystems & Drivers:**
  - Linux Kernel Modules (LKM): `module_init()`, `module_exit()`, `insmod`, `rmmod`, `modprobe`, `lsmod`.
  - Character Device Drivers: Major/minor numbers, `cdev_init()`, `file_operations` struct (`open`, `read`, `write`, `ioctl`, `release`).
  - User-Kernel Memory Boundary: `copy_to_user()`, `copy_from_user()`, `access_ok()`.
  - Locking: Mutexes, Spinlocks, Read-Write locks, RCU (Read-Copy-Update).
  - Linux Subsystems: `libgpiod` (modern GPIO), Industrial I/O (`IIO`), Framebuffer / DRM, V4L2 (Video4Linux).
- **User-Space Systems Programming:**
  - POSIX threads (`pthread_create`, `pthread_mutex`, `pthread_cond`), POSIX message queues.
  - Network sockets: TCP/UDP client/server, non-blocking I/O (`epoll`, `select`).
  - Inter-Process Communication: Shared memory (`shm_open`), pipes, signals.

---

## 7. Communication Protocols & Network Stacks

- **Board-Level Buses:**
  - UART/USART: Framing, baud rate generator, RTS/CTS flow control, RS-232/RS-485 differential signaling.
  - SPI: Master/slave, 4 clock modes (CPOL/CPHA), chip select management, multi-IO (Dual/Quad SPI).
  - I2C / SMBus: Open-drain, pull-ups, 7-bit/10-bit addressing, ACK/NACK, clock stretching, bus arbitration.
- **Field & Industrial Buses:**
  - CAN 2.0A/B & CAN-FD: Bit stuffing, non-destructive arbitration, standard vs extended IDs, error handling (Active, Passive, Bus-off).
  - Modbus: Modbus RTU (serial RS-485) and Modbus TCP.
  - Ethernet: MII/RMII interface, PHY transceiver, MAC controller, TCP/IP stacks (LwIP).
- **IoT & Wireless Networking:**
  - Transport & Application: MQTT (QoS 0/1/2, keepalive, LWT), CoAP, HTTP/REST.
  - Wireless physical layers: BLE (GAP, GATT, BLE mesh), Wi-Fi (802.11 b/g/n), LoRa / LoRaWAN, Cellular (NB-IoT, LTE-M).

---

## 8. Embedded Security & Cryptography

- **Cryptographic Primitives:**
  - Symmetric Encryption: AES-128 / AES-256 (CBC, CTR, GCM modes).
  - Asymmetric Encryption & Key Exchange: RSA, ECC (ECDSA, ECDH on secp256r1 or Curve25519).
  - Hashing: SHA-256, SHA-3, HMAC.
  - Random Number Generation: True Hardware Random Number Generator (TRNG) vs PRNG.
- **Hardware Security Blocks:**
  - Secure Boot: Cryptographic signature verification of firmware image in Flash before execution.
  - Hardware Root of Trust (RoT), Secure Enclaves, ARM TrustZone (Secure vs Non-Secure world).
  - Flash encryption and anti-tamper pins.
- **Transport Security:**
  - TLS 1.2 / 1.3 protocol handshake, client and server certificates (X.509), lightweight stacks (`mbedTLS`).

---

## 9. Firmware Verification, Testing & Quality

- **Unit Testing in C:**
  - Frameworks: Unity, CMock, Ceedling, GoogleTest.
  - Hardware Abstraction Layer mocking: Decoupling business logic from silicon registers for host PC test execution.
- **Static Analysis & Coding Standards:**
  - MISRA-C:2012 / MISRA C++: Essential guidelines for safety-critical systems.
  - Tools: Clang-Tidy, Cppcheck, Flawfinder.
- **CI/CD for Embedded:**
  - Automated compilation checks across toolchains in GitHub Actions.
  - Emulated testing in QEMU / Renode within automated pipelines.
