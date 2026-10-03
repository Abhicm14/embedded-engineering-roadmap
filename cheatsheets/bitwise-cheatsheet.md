# ⚡ Bitwise Manipulation Cheatsheet

---

## 1. Bitwise Operators Reference

| Operator | Symbol | Example | Operation |
| :---: | :---: | :---: | :--- |
| **AND** | `&` | `x & 0x0F` | Clears bits where mask is 0. Used for masking. |
| **OR** | `\|` | `x \| (1 << 3)` | Sets bits where mask is 1. |
| **XOR** | `^` | `x ^ (1 << 2)` | Toggles bits where mask is 1. |
| **NOT** | `~` | `~x` | Inverts all bits (one's complement). |
| **Left Shift** | `<<` | `1 << 4` | Multiplies by $2^N$. Moves bits left, fills with 0. |
| **Right Shift** | `>>` | `x >> 2` | Divides by $2^N$. Unsigned fills 0; Signed fills sign bit. |

---

## 2. Standard Bitwise Idiom Macros

```c
#include <stdint.h>

// 1. Bit Mask
#define BIT(pos)                    (1UL << (pos))

// 2. Set Bit (Force to 1)
#define BIT_SET(reg, pos)           ((reg) |= BIT(pos))

// 3. Clear Bit (Force to 0)
#define BIT_CLEAR(reg, pos)         ((reg) &= ~BIT(pos))

// 4. Toggle Bit (Flip state)
#define BIT_TOGGLE(reg, pos)        ((reg) ^= BIT(pos))

// 5. Test Bit (Returns true if bit is 1)
#define BIT_CHECK(reg, pos)         (((reg) & BIT(pos)) != 0U)

// 6. Write Field (Clear mask area and write new value)
#define FIELD_WRITE(reg, mask, shift, val) \
    ((reg) = (((reg) & ~(mask)) | (((val) << (shift)) & (mask))))

// 7. Read Field
#define FIELD_READ(reg, mask, shift) \
    (((reg) & (mask)) >> (shift))
```

---

## 3. High-Performance Bit Tricks

### Clear the Lowest Set Bit:
```c
x &= (x - 1); // Clears the rightmost '1' bit
```

### Is Power of Two?
```c
bool is_power_of_two = (x > 0) && ((x & (x - 1)) == 0);
```

### Endianness Byte Swap (ARM single-cycle instructions):
```c
// GCC intrinsics:
uint32_t be = __builtin_bswap32(le); // Compiles to REV instruction
uint16_t be = __builtin_bswap16(le); // Compiles to REV16 instruction
```
