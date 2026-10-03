/**
 * @file bit_ops.c
 * @brief High-frequency bitwise macros, field extraction, and endian swap in C99.
 */

#include <stdio.h>
#include <stdint.h>
#include <assert.h>

#define BIT(pos)                    (1UL << (pos))
#define BIT_SET(reg, pos)           ((reg) |= BIT(pos))
#define BIT_CLEAR(reg, pos)         ((reg) &= ~BIT(pos))
#define BIT_TOGGLE(reg, pos)        ((reg) ^= BIT(pos))
#define BIT_CHECK(reg, pos)         (((reg) & BIT(pos)) != 0U)

#define FIELD_WRITE(reg, mask, shift, val) \
    ((reg) = (((reg) & ~(mask)) | (((val) << (shift)) & (mask))))

#define FIELD_READ(reg, mask, shift) \
    (((reg) & (mask)) >> (shift))

/* Brian Kernighan's bit counting algorithm */
static uint32_t PopCount(uint32_t n) {
    uint32_t count = 0;
    while (n) {
        n &= (n - 1);
        count++;
    }
    return count;
}

/* Fast 32-bit byte reversal */
static uint32_t EndianSwap32(uint32_t x) {
    return (((x & 0x000000FFUL) << 24) |
            ((x & 0x0000FF00UL) << 8)  |
            ((x & 0x00FF00FFUL) >> 8)  |
            ((x & 0xFF000000UL) >> 24));
}

int main(void) {
    printf("=== Running Bitwise Operations Tests ===\n");

    uint32_t reg = 0x00000000;

    BIT_SET(reg, 4);
    assert(BIT_CHECK(reg, 4) == 1);
    assert(reg == 0x00000010);

    BIT_TOGGLE(reg, 4);
    assert(reg == 0x00000000);

    /* Write 0x07 into 4-bit field at offset 8 (mask: 0x00000F00) */
    FIELD_WRITE(reg, 0x00000F00UL, 8, 0x07);
    assert(reg == 0x00000700);
    assert(FIELD_READ(reg, 0x00000F00UL, 8) == 0x07);

    assert(PopCount(0b11010110) == 5);

    uint32_t swapped = EndianSwap32(0xAABBCCDD);
    assert(swapped == 0xDDCCBBAA);

    printf("Bitwise Operations Tests PASSED successfully!\n");
    return 0;
}
