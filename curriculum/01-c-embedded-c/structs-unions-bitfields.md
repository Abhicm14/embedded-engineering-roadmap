# 📦 Structs, Unions, Bitfields & Memory Alignment

> Data layout in memory, compiler padding rules, packed attributes, and safe type-punning.

---

## 1. Struct Alignment & Padding

Modern 32-bit processors (like ARM Cortex-M) perform memory accesses much faster when data is aligned to natural boundaries:
- 1-byte (`uint8_t`): Can reside at any address.
- 2-byte (`uint16_t`): Must reside at an even address (divisible by 2).
- 4-byte (`uint32_t`): Must reside at an address divisible by 4.

The compiler automatically inserts **padding bytes** to maintain this alignment:

```c
struct SensorConfig {
    uint8_t  enable;  // 1 byte
    // 3 bytes of compiler padding inserted here!
    uint32_t sample_rate; // 4 bytes (must align to 4-byte boundary)
    uint8_t  mode;    // 1 byte
    // 3 bytes of trailing padding inserted here so array elements align!
}; // Total size: 12 bytes! (Not 6 bytes)
```

---

## 2. Packed Structs: When and Why?

When communicating over a serial wire, SPI bus, or network protocol (e.g. CAN frame or IP packet), padding bytes must be eliminated:

```c
struct __attribute__((packed)) WirePacket {
    uint8_t  preamble;    // 1 byte
    uint16_t packet_id;   // 2 bytes
    uint32_t timestamp;   // 4 bytes
}; // Total size: Exactly 7 bytes (zero padding)
```

> ⚠️ **Warning:** Accessing an unaligned 32-bit field inside a packed struct on ARM Cortex-M0 triggers a `HardFault`! On Cortex-M3/M4, it incurs extra clock cycles. Use `memcpy()` to extract fields safely.

---

## 3. Unions & Safe Type-Punning

A `union` shares the same physical memory location across multiple members:

```c
typedef union {
    float    float_val;
    uint32_t raw_bits;
    uint8_t  bytes[4];
} FloatConverter_t;

FloatConverter_t conv;
conv.float_val = 3.14159f;

// Transmit 4 raw bytes over UART without data conversion:
UART_Transmit(conv.bytes, 4);
```

---

## 4. Bit-Fields vs Bitwise Masks

C allows defining bit-fields:
```c
struct StatusReg {
    uint32_t is_ready    : 1;
    uint32_t error_code  : 3;
    uint32_t reserved    : 28;
};
```
> ⚠️ **MISRA-C Recommendation:** Avoid C bit-fields for memory-mapped hardware registers! The C standard does not guarantee bit-ordering (MSB-first vs LSB-first) across different compilers. Use explicit bitwise shift masks instead (`(1UL << 3)`).
