# ⚡ Embedded C Quick Reference Cheatsheet

> High-frequency bitwise macros, memory pointer idioms, volatile rules, struct alignment, and defensive C idioms for firmware development.

---

## 1. Bitwise Manipulation Idioms

```c
#include <stdint.h>

// 1. Bit Mask Definition
#define BIT(pos)                    (1UL << (pos))

// 2. Set Bit (Set bit at 'pos' to 1)
#define BIT_SET(reg, pos)           ((reg) |= BIT(pos))

// 3. Clear Bit (Clear bit at 'pos' to 0)
#define BIT_CLEAR(reg, pos)         ((reg) &= ~BIT(pos))

// 4. Toggle Bit (Flip bit at 'pos')
#define BIT_TOGGLE(reg, pos)        ((reg) ^= BIT(pos))

// 5. Test Bit (Returns non-zero if bit at 'pos' is 1)
#define BIT_CHECK(reg, pos)         (((reg) & BIT(pos)) != 0U)

// 6. Write Field (Clear 'mask' bits and set with 'val')
#define FIELD_WRITE(reg, mask, shift, val) \
    ((reg) = (((reg) & ~(mask)) | (((val) << (shift)) & (mask))))

// 7. Read Field
#define FIELD_READ(reg, mask, shift) \
    (((reg) & (mask)) >> (shift))
```

---

## 2. Memory-Mapped I/O Register Access

```c
// Direct Memory-Mapped Register Access
#define HW_REG32(addr)              (*((volatile uint32_t *)(addr)))
#define HW_REG16(addr)              (*((volatile uint16_t *)(addr)))
#define HW_REG8(addr)               (*((volatile uint8_t  *)(addr)))

// Peripheral Struct Mapping
typedef struct {
    volatile uint32_t MODER;    // Mode register, offset: 0x00
    volatile uint32_t OTYPER;   // Output type register, offset: 0x04
    volatile uint32_t OSPEEDR;  // Output speed register, offset: 0x08
    volatile uint32_t PUPDR;    // Pull-up/pull-down register, offset: 0x0C
    volatile uint32_t IDR;      // Input data register, offset: 0x10
    volatile uint32_t ODR;      // Output data register, offset: 0x14
    volatile uint32_t BSRR;     // Bit set/reset register, offset: 0x18
} GPIO_RegDef_t;

#define GPIOA_BASE_ADDR             (0x40020000UL)
#define GPIOA                       ((GPIO_RegDef_t *)GPIOA_BASE_ADDR)

// Atomic Pin Set and Clear using BSRR (Thread-safe without disabling IRQ)
#define PIN_5                       (5U)
GPIOA->BSRR = (1U << PIN_5);        // Set PA5 High
GPIOA->BSRR = (1U << (PIN_5 + 16)); // Set PA5 Low
```

---

## 3. Endianness Conversion & Byte Swapping

```c
// Endianness Swap (Host to Network / Big-Endian to Little-Endian)
#define BSWAP16(val) \
    ((uint16_t)((((uint16_t)(val) & 0x00FFU) << 8) | \
                (((uint16_t)(val) & 0xFF00U) >> 8)))

#define BSWAP32(val) \
    ((uint32_t)((((uint32_t)(val) & 0x000000FFUL) << 24) | \
                (((uint32_t)(val) & 0x0000FF00UL) << 8)  | \
                (((uint32_t)(val) & 0x00FF0000UL) >> 8)  | \
                (((uint32_t)(val) & 0xFF000000UL) >> 24)))

// Fast ARM compiler intrinsic equivalents:
// __REV(x)   -> 32-bit byte reverse (single cycle)
// __REV16(x) -> 16-bit byte reverse (single cycle)
```

---

## 4. Struct Alignment & Padding Control

```c
// Packed Struct (Zero padding, for network packets and wire protocols)
typedef struct __attribute__((packed)) {
    uint8_t  preamble;     // 1 byte
    uint16_t packet_id;    // 2 bytes
    uint32_t payload_len;  // 4 bytes
} ProtocolHeader_t;        // Total size: 7 bytes (without packed, would be 8 bytes)

// Explicitly Aligned Variable (For DMA buffers requiring cacheline alignment)
uint8_t dma_rx_buffer[256] __attribute__((aligned(32)));
```

---

## 5. Defensive C & Compiler Attributes

```c
// Prevent unused variable compiler warnings
#define UNUSED(x)                   ((void)(x))

// Static / Compile-Time Assertion (C11)
_Static_assert(sizeof(uint32_t) == 4, "uint32_t must be exactly 4 bytes");

// GCC Optimization & Placement Attributes
#define WEAK_ALIAS(alias_name)      __attribute__((weak, alias(#alias_name)))
#define SECTION_RAMFUNC             __attribute__((section(".ramfunctions")))
#define NO_RETURN                   __attribute__((noreturn))
```
