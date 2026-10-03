# 🔘 General Purpose Input/Output (GPIO)

> Controlling physical silicon pins, output stages, pull-ups, and atomic register manipulation.

---

## 1. Internal GPIO Pad Architecture

Each STM32 GPIO pin contains 4 configurable output/input stages:

```
                          VDD
                           │
                         [P-MOS] (Pull HIGH)
                           │
Pin Pad ───┬───────────────┴─── Output Stage (Push-Pull vs Open-Drain)
           │               │
           │             [N-MOS] (Pull LOW to GND)
           │               │
           │              GND
           │
           ├─[Internal Pull-Up Resistor: 40k]── VDD
           ├─[Internal Pull-Down Resistor: 40k]─ GND
           │
           └──► Schmitt Trigger ──► Input Data Register (IDR)
```

### Modes (`MODER`):
- **Input (00):** Pin is high-impedance. Schmitt trigger samples voltage into `IDR`.
- **Output (01):** Pin is driven actively HIGH (3.3V) or LOW (0V).
- **Alternate Function (10):** Pin connected directly to internal peripherals (UART, SPI, Timer PWM).
- **Analog (11):** Schmitt trigger disabled to eliminate leakage current; pin routed to ADC or DAC.

### Output Type (`OTYPER`):
- **Push-Pull (0):** Both P-MOS and N-MOS active. Drives actively to 3.3V and 0V. Standard for LEDs, SPI, and UART.
- **Open-Drain (1):** P-MOS permanently disabled. Pin can only pull to Ground (0V) or float (HIGH-Z). Requires external pull-up resistor. Mandatory for I2C and shared multi-device interrupt lines!

---

## 2. Why `BSRR` Over `ODR`? (Atomic Bit Set/Reset)

When modifying a pin via Output Data Register (`ODR`), you must perform a Read-Modify-Write cycle:
```c
// DANGEROUS: Non-atomic!
GPIOA->ODR |= (1 << 5); 
// Disassembles to:
// 1. LDR R0, [GPIOA, #ODR]
// 2. ORR R0, R0, #32
// 3. STR R0, [GPIOA, #ODR]
// If an interrupt fires between lines 1 and 3 and changes pin 6, its write is lost!
```

### The Atomic Solution: `BSRR`
The Bit Set/Reset Register (`BSRR`) is a write-only register:
- Writing `1` to bits `[15:0]` sets the corresponding pin HIGH.
- Writing `1` to bits `[31:16]` clears the corresponding pin LOW.
- Writing `0` has zero effect.

```c
// THREAD-SAFE: Single instruction write, zero race conditions!
GPIOA->BSRR = (1UL << 5);        // Set PA5 High
GPIOA->BSRR = (1UL << (5 + 16)); // Set PA5 Low
```

---

## 🛠️ Starter Exercise
Write a C program that initializes `GPIOC` Pin 13 as output (Push-Pull, Medium Speed) and toggles it every 500 ms using SysTick and the `BSRR` register.
