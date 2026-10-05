# ⚡ ATmega328P / Arduino Uno: Bare-Metal GPIO Register Walkthrough

> **Looking for the deep register-by-register breakdown?**  
> We have a comprehensive, line-by-line guide explaining the silicon registers (`DDRB`, `PORTB`, `PINB`), the 1-cycle hardware toggle trick, and the pull-up dual personality:

👉 [**📘 Open the Full AVR ATmega328P Bare-Metal GPIO Walkthrough (`curriculum/reference/gpio-bare-metal-walkthrough-avr.md`)**](../../curriculum/reference/gpio-bare-metal-walkthrough-avr.md)

---

### 🚀 Quick Register Blueprint for Arduino Pin 13 (PB5)

```c
#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

int main(void) {
    // 1. Set Pin 13 (PB5) as OUTPUT
    DDRB |= (1 << DDB5);

    while (1) {
        // 2. Hardware 1-cycle atomic toggle via PINB register
        PINB = (1 << PINB5);
        _delay_ms(500);
    }
}
```

| Register | Address | Target Bit | Value | What It Does |
| :--- | :---: | :---: | :---: | :--- |
| **`DDRB`** | `0x04` | `DDB5` (Bit 5) | `1` | Configures physical pin PB5 as an **Output** driver. |
| **`PORTB`**| `0x05` | `PORTB5` (Bit 5) | `1` / `0` | Drives output High ($5\text{V}$) or Low ($0\text{V}$). |
| **`PINB`** | `0x03` | `PINB5` (Bit 5) | `1` | Writing `1` triggers a **1-cycle atomic hardware toggle**! |
