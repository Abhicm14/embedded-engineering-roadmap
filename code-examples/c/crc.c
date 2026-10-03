/**
 * @file crc.c
 * @brief Standard CRC-16 (CCITT) and CRC-32 (IEEE 802.3) calculations in C99.
 */

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>

/* CRC-16 CCITT (Polynomial 0x1021, Initial 0xFFFF) */
uint16_t CRC16_CCITT(const uint8_t *data, size_t length) {
    uint16_t crc = 0xFFFF;
    for (size_t i = 0; i < length; i++) {
        crc ^= ((uint16_t)data[i] << 8);
        for (uint8_t bit = 0; bit < 8; bit++) {
            if (crc & 0x8000) {
                crc = (crc << 1) ^ 0x1021;
            } else {
                crc = (crc << 1);
            }
        }
    }
    return crc;
}

/* CRC-32 (Polynomial 0xEDB88320 reversed, Initial 0xFFFFFFFF) */
uint32_t CRC32_Calculate(const uint8_t *data, size_t length) {
    uint32_t crc = 0xFFFFFFFFUL;
    for (size_t i = 0; i < length; i++) {
        crc ^= data[i];
        for (uint8_t bit = 0; bit < 8; bit++) {
            if (crc & 1) {
                crc = (crc >> 1) ^ 0xEDB88320UL;
            } else {
                crc = (crc >> 1);
            }
        }
    }
    return ~crc;
}

int main(void) {
    printf("=== Running CRC-16 & CRC-32 Unit Tests ===\n");

    const char *test_str = "123456789";
    size_t len = strlen(test_str);

    /* Standard test vector check for "123456789" */
    uint16_t crc16 = CRC16_CCITT((const uint8_t *)test_str, len);
    uint32_t crc32 = CRC32_Calculate((const uint8_t *)test_str, len);

    printf("  CRC-16 CCITT for '%s': 0x%04X (Expected: 0x29B1)\n", test_str, crc16);
    printf("  CRC-32       for '%s': 0x%08X (Expected: 0xCBF43926)\n", test_str, crc32);

    assert(crc16 == 0x29B1);
    assert(crc32 == 0xCBF43926UL);

    printf("CRC Unit Tests PASSED successfully!\n");
    return 0;
}
