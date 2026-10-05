# 📘 Step 1: C & Embedded C

> **Pillar:** FOUNDATION (C + Electronics)  
> **Core Rule:** *"Do not learn peripherals only as APIs. Understand the register, electrical signal, timing diagram, protocol transaction and failure modes behind each API call."*  
> **Prerequisites:** Review [**`PREREQUISITES.md`**](../../PREREQUISITES.md) before writing code. If you are an absolute beginner, start with our [**🌱 True Beginner On-Ramp (`beginner-onramp/`)**](../../beginner-onramp/README.md). Look up any confusing term in our [**📖 Beginner Glossary (`cheatsheets/glossary.md`)**](../../cheatsheets/glossary.md). Always construct firmware using the **7-Step Code Construction Workflow** ([`code-examples/README.md`](../../code-examples/README.md)).

---

## 🎯 Learning Objectives

Master the programming language that powers 80%+ of bare-metal firmware and operating system kernels. By the end of this module, you should be able to:
1. Explain how high-level C maps to physical memory segments (`.text`, `.rodata`, `.data`, `.bss`, stack, heap).
2. Write pointer arithmetic and dereference memory-mapped hardware registers directly.
3. Understand the exact semantics of `volatile`, `const`, `static`, and `extern`.
4. Avoid uncontrolled dynamic memory allocation (`malloc`/`free`) and design deterministic static buffers.
5. Adhere to basic **MISRA-C:2012** defensive programming guidelines.

---

## 🧭 Topic Guides in This Module

| Topic Document | Type | Key Concepts |
| :--- | :---: | :--- |
| [**1. Syntax & Pointers**](syntax-and-pointers.md) | `[INTUITION]` | Pointer arithmetic, `void*`, double pointers, function pointer callbacks. |
| [**2. Memory Model & Layout**](memory-model.md) | `[INTUITION]` | Flash vs SRAM, stack frames, heap fragmentation, map files, linker placement. |
| [**3. Structs, Unions & Bitfields**](structs-unions-bitfields.md) | `[INTUITION]` | Struct alignment, padding, `#pragma pack(1)`, type-punning, endianness. |
| [**4. Volatile, Const & Qualifiers**](volatile-const-type-qualifiers.md) | `[INTUITION]` | Compiler optimization barriers, hardware registers, ISR flags, read-only tables. |
| [**5. The C Build Process**](build-process.md) | `[DEEP DIVE]` | Preprocessor $\rightarrow$ Compiler $\rightarrow$ Assembler $\rightarrow$ Linker $\rightarrow$ Map file analysis. |

---

## 🛠️ Hands-on Coding Practice

Before proceeding to Step 2, implement and verify the following programs located in [`code-examples/c/`](../../code-examples/c/):

1. **Circular FIFO Ring Buffer (`circular_buffer.c`):** Lock-free, power-of-two capacity ring buffer using head and tail pointers.
2. **Finite State Machine (`fsm.c`):** State transition table using function pointers for an embedded appliance.
3. **Debounce Logic (`debounce.c`):** Software filtering state machine for mechanical pushbuttons.
4. **CRC-16 / CRC-32 Calculator (`crc.c`):** Polynomial checksum generator for wire protocols.
5. **Bit Manipulation Library (`bit_ops.c`):** Setting, clearing, toggling, field extraction, and byte swapping macros.
6. **Command Parser (`command_parser.c`):** Serial CLI dispatcher matching tokens and dispatching callbacks.

---

## ✅ Step 1 Completion Checklist

- [ ] Can explain the difference between `const char *p` and `char * const p`.
- [ ] Can define what `volatile` does and name 3 specific scenarios where it is mandatory.
- [ ] Can calculate struct padding and alignment on a 32-bit architecture.
- [ ] Can perform bitwise operations to set, clear, toggle, and read specific bitfields.
- [ ] Can read an output `.map` file from GCC and verify Flash and RAM consumption.

➡️ **Next Step:** [Step 2: Electronics & Computer Fundamentals](../02-electronics-computer-fundamentals/README.md)
