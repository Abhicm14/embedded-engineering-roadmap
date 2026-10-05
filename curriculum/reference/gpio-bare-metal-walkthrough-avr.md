# ⚡ Bare-Metal GPIO Walkthrough: Atmel AVR ATmega328P (Arduino Uno)

> **Target Part:** Microchip / Atmel ATmega328P (28-pin DIP / 32-pin TQFP, the microcontroller on the Arduino Uno)  
> **Target Pin:** `PB5` (Port B, Pin 5 / Digital Pin 13 / Onboard Yellow LED / SPI SCK)  
> **Official Documentation to Open:**  
> 1. **Atmel ATmega328P Complete Datasheet (DS40002061)**:  
>    - *Section 14: "I/O-Ports"*  
>    - *Section 14.2: "Ports as General Digital I/O"*  
>    - *Section 14.4: "Register Description (MCUCR, PORTB, DDRB, PINB)"*  
>    - *Section 28.2: "Fuse Bits"*  

---

### 🧠 The Engineering Mindset: The 3-Register AVR Paradigm

Unlike Arduino's slow C++ functions (`digitalWrite()` consumes ~50 CPU clock cycles due to lookup tables and timer-disable checks), bare-metal AVR programming configures the silicon registers directly in **1 single clock cycle** ($62.5\text{ ns}$ at 16 MHz).

The entire AVR GPIO subsystem revolves around **three registers per port**:

```
                       ATmega328P 8-Bit AVR CPU Core (16 MHz Crystal)
                                             │
                            [ High-Speed 8-Bit I/O Bus ]
                                             │
      ┌──────────────────────────────────────┼──────────────────────────────────────┐
      ▼                                      ▼                                      ▼
┌──────────────┐                       ┌──────────────┐                       ┌──────────────┐
│ DDRB (0x04)  │                       │ PORTB (0x05) │                       │ PINB (0x03)  │
├──────────────┤                       ├──────────────┤                       ├──────────────┤
│ Direction    │                       │ Output Drive │                       │ Input Sample │
│ 1 = OUTPUT   │                       │ or Pull-Up   │                       │ & HW Toggle  │
└──────┬───────┘                       └──────┬───────┘                       └──────┬───────┘
       │                                      │                                      │
       ▼                                      ▼                                      ▼
[ Output Buffer Enable ] ──────────────► [ Output Transistors ] ─────────────► Physical Pin PB5
                                                                               (Arduino Pin 13)
                                                                               [ 0V <---> 5V ]
```

---

## 🧭 Step-by-Step Register Configuration

### Step 1: Open the Datasheet & Pinout Diagram
1. Open **Section 14 ("I/O-Ports") of DS40002061**.
2. Verify pin allocation:
   * **`PB5`** is physical Pin 19 on the 28-pin DIP package.
   * On the Arduino Uno board, `PB5` is routed to the onboard yellow LED (marked **"L"**) through an operational amplifier buffer, and breaks out to header socket **Pin 13**.
   * It also serves as the hardware **SCK (Serial Clock)** for the SPI bus.

---

### Step 2: Understand the Clock Architecture
* **Is there a peripheral bus clock enable like STM32's RCC?**  
  **NO!** In AVR architecture, the internal I/O clock domain ($clk_{I/O}$) runs continuously whenever the CPU core is in active or idle sleep modes. There is no software clock gate register required before accessing `DDRB`, `PORTB`, or `PINB`.

---

### Step 3: Configure Direction via the Data Direction Register (`DDRB`)
* **Register:** `DDRB` (*Port B Data Direction Register*)
* **I/O Address:** `0x04` (SRAM Memory-Mapped Address: `0x24`)
* **Target Bit:** **Bit 5 (`DDB5`)**
* **Bit Definitions:**
  * `1`: **OUTPUT** (Enables low-impedance output driver).
  * `0`: **INPUT** (Disables output driver; pin becomes high-impedance).

#### 💡 Contrast: AVR vs PIC Direction Polarity
* **AVR ATmega:** `1` = **Output**, `0` = **Input**. (Intuitive: 1 enables output power).
* **PIC16F:** `0` = **Output**, `1` = **Input**. (Opposite: 0 enables low-impedance driver).

```c
// Configure PB5 as an OUTPUT:
DDRB |= (1 << DDB5); // Sets Bit 5 to 1
```

---

### Step 4: Driving Outputs & Internal Pull-Ups via `PORTB`
The `PORT` register in AVR microcontrollers has an ingenious dual personality depending on the state of `DDR`:

* **Register:** `PORTB` (*Port B Data Register*)
* **I/O Address:** `0x05` (SRAM Memory-Mapped Address: `0x25`)
* **Target Bit:** **Bit 5 (`PORTB5`)**

#### Mode A: When Pin is an OUTPUT (`DDRB5 = 1`):
* Writing `PORTB5 = 1` turns on the internal P-channel MOSFET, driving the pin **HIGH** ($5\text{V}$).
* Writing `PORTB5 = 0` turns on the internal N-channel MOSFET, pulling the pin **LOW** ($0\text{V}$ / Ground).

```c
PORTB |=  (1 << PORTB5); // Turn LED ON (5V)
PORTB &= ~(1 << PORTB5); // Turn LED OFF (0V)
```

#### Mode B: When Pin is an INPUT (`DDRB5 = 0`):
* Writing `PORTB5 = 1` connects an internal **Weak Pull-Up Resistor** ($20\text{ k}\Omega$ to $50\text{ k}\Omega$) between the pin and $5\text{V}$!
* Writing `PORTB5 = 0` leaves the pin in pure high-impedance (tri-state floating) mode.

*Why did Atmel combine Output Drive and Pull-Up into one register?*  
In the 1990s, silicon real estate was expensive. By combining pull-up activation with the `PORT` register, AVR saved register address space and silicon transistor count!

---

### Step 5: Reading Inputs & The Secret Hardware Toggle (`PINB`)
To read incoming physical voltages, read the `PIN` register (not `PORT`!):

* **Register:** `PINB` (*Port B Input Pins Address*)
* **I/O Address:** `0x03` (SRAM Memory-Mapped Address: `0x23`)
* **Bit 5 (`PINB5`):** Returns the digital logic level after passing through the on-chip Schmitt trigger synchronizer.

```c
// Sample physical logic level:
if (PINB & (1 << PINB5)) {
    // Pin is physically HIGH (> 3.0V at 5V VCC)
}
```

#### ⚡ The Secret AVR Hardware Toggle Trick:
On classical microcontrollers, toggling an output pin requires an XOR read-modify-write:
```c
PORTB ^= (1 << PORTB5); // Reads PORTB, XORs bit 5, writes back (3 CPU cycles)
```
On the ATmega328P, Atmel added a secret hardware shortcut: **Writing a logical `1` to a bit in the `PIN` register physically toggles the corresponding bit in the `PORT` latch!**

```c
PINB = (1 << PINB5); // Toggles PB5 in a SINGLE 1-CYCLE CPU INSTRUCTION!
```
*Why is this awesome?* It executes in 1 clock cycle ($62.5\text{ ns}$), uses zero RAM registers, and is 100% atomic (impossible to corrupt during an interrupt).

---

### Step 6: Prerequisite — Fuse Bits & System Clock (`F_CPU`)
To ensure software delay routines (`_delay_ms()`) produce exact millisecond timing, the compiler must know the clock frequency set by the hardware fuse bits:

* **Arduino Uno Fuses:**
  * **Low Fuse (`0xFF`):** External crystal oscillator ($>8\text{ MHz}$), full-swing crystal, slow-rising power.
  * **High Fuse (`0xDE`):** SPI programming enabled, brown-out reset at $2.7\text{V}$, bootloader size 512 words.
  * **Extended Fuse (`0xFD`):** Brown-out detection level 2 ($2.7\text{V}$).
* **Compiler Macro:** You must define `#define F_CPU 16000000UL` before including `<util/delay.h>`.

---

## 📊 Complete ATmega328P PB5 Register Map Summary

| Register | I/O Address | SRAM Address | Target Bit | Value | Engineering Purpose |
| :--- | :---: | :---: | :---: | :---: | :--- |
| **`DDRB`** | `0x04` | `0x24` | `[5]` (`DDB5`) | `1` | Configure PB5 as an **Output** driver. |
| **`DDRB`** | `0x04` | `0x24` | `[5]` (`DDB5`) | `0` | Configure PB5 as an **Input** listener. |
| **`PORTB`**| `0x05` | `0x25` | `[5]` (`PORTB5`)| `1` | If Output: Drive High ($5\text{V}$). If Input: Enable Pull-up. |
| **`PORTB`**| `0x05` | `0x25` | `[5]` (`PORTB5`)| `0` | If Output: Drive Low ($0\text{V}$). If Input: High-Z float. |
| **`PINB`** | `0x03` | `0x23` | `[5]` (`PINB5`) | Read | Sample synchronized physical voltage on pin PB5. |
| **`PINB`** | `0x03` | `0x23` | `[5]` (`PINB5`) | Write `1`| **Atomic Toggle:** Flips PB5 in 1 clock cycle! |

---

## 💻 Full Compilable AVR-GCC Bare-Metal Implementation

This code does **not** use the Arduino framework or `main.cpp` wrapper. It compiles directly with `avr-gcc` into a minimal $\approx 150\text{ byte}$ `.hex` binary (compared to $1000+\text{ bytes}$ for Arduino `blink.ino`!):

```c
/**
 * @file main.c
 * @brief ATmega328P Bare-Metal PB5 (Pin 13) GPIO Output Driver
 */
#define F_CPU 16000000UL // 16 MHz Clock Frequency

#include <avr/io.h>
#include <util/delay.h>

int main(void) {
    // Step 3: Configure PB5 as an OUTPUT (Set bit 5 in DDRB)
    DDRB |= (1 << DDB5);

    // Ensure LED starts turned OFF
    PORTB &= ~(1 << PORTB5);

    // Step 7: Superloop - Atomic Pin Toggle using the PINB hardware register
    while (1) {
        // Writing 1 to PINB toggles the PORTB output latch in 1 clock cycle!
        PINB = (1 << PINB5);

        // Hardware-calibrated delay loop
        _delay_ms(500);
    }

    return 0;
}
```

---

## 🔍 "Why Is Nothing Happening?" Troubleshooting Checklist

1. **Did you write to `DDRB` to set direction?**  
   *Symptom:* If `DDRB` bit 5 is `0`, the pin is an Input. Writing `1` to `PORTB` only enables the weak internal pull-up resistor ($30\text{ k}\Omega$). An LED connected through a $30\text{ k}\Omega$ pull-up will appear completely dark or barely visible in a pitch-black room!
2. **Did you define `F_CPU` before including `<util/delay.h>`?**  
   *Symptom:* If `F_CPU` is missing or defined after `<util/delay.h>`, the compiler defaults to `1000000UL` ($1\text{ MHz}$). Your delays will run **16 times slower than expected** (a 500ms delay will take 8 full seconds!).
3. **Is the Arduino bootloader intact?**  
   *Check:* On a physical Arduino Uno, Pin 13 shares the line with an onboard op-amp and an LED. If you connect an external circuit that pulls high current, it can overload the pin limit ($40\text{ mA}$ absolute maximum per pin; recommend $<20\text{ mA}$).
