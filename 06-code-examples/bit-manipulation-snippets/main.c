#include <stdio.h>
#include <stdint.h>
#include <assert.h>

/* Bitwise Manipulation Macros */
#define BIT(n)                        (1UL << (n))
#define BIT_SET(reg, n)               ((reg) |= BIT(n))
#define BIT_CLEAR(reg, n)             ((reg) &= ~BIT(n))
#define BIT_TOGGLE(reg, n)            ((reg) ^= BIT(n))
#define BIT_CHECK(reg, n)             (((reg) & BIT(n)) != 0U)
#define FIELD_WRITE(reg, mask, shift, val) \
    ((reg) = (((reg) & ~(mask)) | (((val) << (shift)) & (mask))))
#define FIELD_READ(reg, mask, shift)  (((reg) & (mask)) >> (shift))

/* Count set bits (Hamming weight) using Brian Kernighan's algorithm */
static uint32_t CountSetBits(uint32_t n) {
    uint32_t count = 0;
    while (n) {
        n &= (n - 1);
        count++;
    }
    return count;
}

/* Fast 32-bit Endianness Byte Swap */
static uint32_t ByteSwap32(uint32_t val) {
    return (((val & 0x000000FFUL) << 24) |
            ((val & 0x0000FF00UL) << 8)  |
            ((val & 0x00FF0000UL) >> 8)  |
            ((val & 0xFF000000UL) >> 24));
}

int main(void) {
    printf("=== Testing Bit Manipulation Snippets ===\n");

    uint32_t reg = 0x00000000;

    /* Test bit set */
    BIT_SET(reg, 3);
    assert(reg == 0x00000008);
    assert(BIT_CHECK(reg, 3) == 1);
    assert(BIT_CHECK(reg, 2) == 0);

    /* Test bit toggle */
    BIT_TOGGLE(reg, 3);
    assert(reg == 0x00000000);

    /* Test field write (4 bits at shift 4) */
    uint32_t mask = (0x0F << 4);
    FIELD_WRITE(reg, mask, 4, 0x0A);
    assert(reg == 0x000000A0);
    assert(FIELD_READ(reg, mask, 4) == 0x0A);

    /* Test Hamming weight count */
    assert(CountSetBits(0b10110010) == 4);

    /* Test endian swap */
    uint32_t le_val = 0x12345678;
    uint32_t be_val = ByteSwap32(le_val);
    assert(be_val == 0x78563412);

    printf("All bitwise manipulation assertions PASSED!\n");
    return 0;
}
