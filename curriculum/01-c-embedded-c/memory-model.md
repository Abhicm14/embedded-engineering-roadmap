# 🧠 The Embedded C Memory Model & Segments

> How source code translates into Flash and SRAM sections, stack frames, and why heap allocation is restricted.

---

## 1. Physical Memory Map: Flash vs SRAM

Unlike desktop PCs where code and data reside in unified virtual RAM, microcontrollers feature physical Harvard or modified Harvard separation:

```
FLASH (Non-Volatile / Read-Only at runtime):
┌──────────────────────────────────────┐  0x08000000 (Typical STM32 Flash Base)
│ .isr_vector (Interrupt Vector Table) │
├──────────────────────────────────────┤
│ .text       (Machine Instructions)   │
├──────────────────────────────────────┤
│ .rodata     (const variables/strings)│
├──────────────────────────────────────┤
│ .data initializers (LMA)             │  Copied to SRAM during startup
└──────────────────────────────────────┘

SRAM (Volatile / Read-Write):
┌──────────────────────────────────────┐  0x20000000 (Typical STM32 SRAM Base)
│ .data (Initialized globals/statics)  │  (VMA)
├──────────────────────────────────────┤
│ .bss  (Zero-initialized variables)   │  Zeroed out by startup.c
├──────────────────────────────────────┤
│ Heap  (Grows upwards toward Stack)   │  Controlled by malloc / sbrk
│   │                                  │
│   ▼                                  │
│   ▲                                  │
│   │                                  │
│ Stack (Grows downwards from top)     │  Managed by MSP / PSP
└──────────────────────────────────────┘  0x20020000 (Top of 128KB SRAM)
```

---

## 2. Memory Segments Explained

| Segment | Physical Location | Contents | Example in C |
| :--- | :--- | :--- | :--- |
| **`.text`** | Flash | Compiled machine instructions | `void foo(void) { ... }` |
| **`.rodata`** | Flash | Constant variables and string literals | `const uint32_t baud = 115200;` |
| **`.data`** | Flash (LMA) $\rightarrow$ RAM (VMA) | Global/static variables initialized to non-zero | `uint32_t counter = 42;` |
| **`.bss`** | RAM | Global/static variables initialized to zero/uninitialized | `uint8_t rx_buffer[256];` |
| **Stack** | RAM | Function local variables, call parameters, return addresses | `void func(void) { int temp; }` |
| **Heap** | RAM | Dynamic runtime allocation | `malloc(128)` (Discouraged in bare-metal) |

---

## 3. Why Avoid Dynamic Allocation (`malloc`/`free`)?

In embedded firmware, dynamic heap allocation introduces three severe risks:
1. **Heap Fragmentation:** Over time, allocating and freeing varying block sizes leaves holes of free memory that are individually too small to satisfy future requests, causing `malloc()` to fail unexpectedly even when total free RAM seems adequate.
2. **Non-Deterministic Execution Time:** Finding a free block can take variable time ($O(N)$), violating real-time deadlines.
3. **Memory Leaks & HardFaults:** Losing a pointer causes irreversible loss of RAM on a system designed to run continuously for years without rebooting.

**Rule:** Pre-allocate all memory statically at compile-time as global arrays or use fixed-size block pool allocators!
