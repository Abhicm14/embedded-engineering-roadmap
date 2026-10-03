# ⚡ Embedded C Quick Reference Cheatsheet

---

## 1. Type Qualifiers

### `volatile`
- **What it does:** Inhibits compiler optimizations that cache reads/writes in CPU registers.
- **When to use:** Memory-mapped I/O registers, variables modified in ISRs, multi-task flags.

### `const`
- **What it does:** Marks variables immutable. In embedded, places data in Flash (`.rodata`) instead of consuming SRAM!

### Pointer Declarations (Read right to left):
```c
const char *p;        // Pointer to constant char (can move pointer, cannot modify data)
char * const p;        // Constant pointer to char (cannot move pointer, can modify data)
const char * const p;  // Constant pointer to constant char (neither can change)
```

---

## 2. Memory-Mapped I/O Register Pointers

```c
// Direct 32-bit register access
#define REG32(addr)            (*((volatile uint32_t *)(addr)))

// Struct-based peripheral definition
typedef struct {
    volatile uint32_t MODER;   // Offset 0x00
    volatile uint32_t OTYPER;  // Offset 0x04
    volatile uint32_t OSPEEDR; // Offset 0x08
    volatile uint32_t PUPDR;   // Offset 0x0C
    volatile uint32_t IDR;     // Offset 0x10
    volatile uint32_t ODR;     // Offset 0x14
    volatile uint32_t BSRR;    // Offset 0x18
} GPIO_RegDef_t;

#define GPIOA ((GPIO_RegDef_t *) 0x40020000UL)
```

---

## 3. Fixed-Width Integer Types (`<stdint.h>`)

| Type | Size | Range (Signed) | Range (Unsigned) |
| :--- | :---: | :--- | :--- |
| `int8_t` / `uint8_t` | 1 byte | -128 to 127 | 0 to 255 |
| `int16_t` / `uint16_t` | 2 bytes | -32,768 to 32,767 | 0 to 65,535 |
| `int32_t` / `uint32_t` | 4 bytes | $\approx -2.14 \times 10^9$ to $2.14 \times 10^9$ | 0 to 4,294,967,295 |
| `int64_t` / `uint64_t` | 8 bytes | $-9.22 \times 10^{18}$ to $9.22 \times 10^{18}$ | 0 to $1.84 \times 10^{19}$ |
| `uintptr_t` | Pointer-sized | Unsigned integer guaranteed to hold pointer address |

---

## 4. Useful GCC Compiler Attributes

```c
__attribute__((packed))                  // Eliminates struct padding
__attribute__((aligned(4)))              // Forces 4-byte memory boundary
__attribute__((section(".ramfunc")))     // Places function in SRAM instead of Flash
__attribute__((noreturn))                // Informs compiler function never returns
__attribute__((weak))                    // Allows user code to override default handler
```
