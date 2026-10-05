# 🔌 Bare-Metal GPIO Walkthrough: Microchip PIC16F877A (8-bit Harvard RISC)

> **Target Part:** Microchip PIC16F877A (40-pin DIP or 44-pin TQFP)  
> **Target Pin:** `RB0` (Port B, Pin 0 / Pin 33 on DIP-40)  
> **Official Documentation to Open:**  
> 1. **Microchip PIC16F87XA Datasheet (DS39582C)**:  
>    - *Section 3.2: "PORTB and the TRISB Register"*  
>    - *Section 2.2: "Data Memory Organization (Memory Banks 0, 1, 2, 3)"*  
>    - *Section 11.0: "Analog-to-Digital Converter (ADC) Module & ADCON1"*  
>    - *Section 14.1: "Configuration Bits & Oscillator Selection"*  

---

### 🧠 The Engineering Mindset: 8-Bit Harvard Architecture

Unlike 32-bit ARM Cortex-M processors where all peripherals connect to complex AHB/APB crossbar buses with software clock gating, the **8-bit PIC Harvard architecture** connects CPU registers directly to hardware latches through banked RAM addresses:

```
                  PIC16F877A 8-Bit CPU Core (4 MHz Crystal -> 1 MHz Instruction Clock)
                                                │
                          [ Direct 8-Bit Internal Data Bus ]
                                                │
                 ┌──────────────────────────────┴──────────────────────────────┐
                 ▼                                                             ▼
     ┌───────────────────────┐                                     ┌───────────────────────┐
     │  BANK 1: TRISB (0x86) │                                     │  BANK 0: PORTB (0x06) │
     ├───────────────────────┤                                     ├───────────────────────┤
     │ TRISB0 = 0 (OUTPUT)   │                                     │ PORTB0 = 1 (Set 5V)   │
     │ (Tri-State Direction) │                                     │ (Output Data Pin)     │
     └───────────┬───────────┘                                     └───────────┬───────────┘
                 │                                                             │
                 ▼                                                             ▼
         Output Buffer Enabled ──────────────────────────────────────► Physical Pin RB0
                                                                       [ 0V <---> 5V ]
```

---

## 🧭 Step-by-Step Register Configuration

### Step 1: Open the Datasheet "I/O Ports" Chapter
Open **Section 3.2 of DS39582C**.  
Observe how Port B is defined:
* **`PORTB`**: 8-bit wide, bidirectional port (Pins `RB0` through `RB7`).
* Each pin has a corresponding direction bit in **`TRISB`**.

---

### Step 2: Understand the Clock & Power Architecture
* **Is there a peripheral bus clock enable like STM32's RCC?**  
  **NO!** On 8-bit PIC microcontrollers, the instruction clock ($F_{OSC} / 4$) directly drives the I/O port hardware whenever the microcontroller is active. There is no software clock gate to turn on.
* **CRITICAL GOTCHA — The Analog Input Trap (`ADCON1`):**  
  On microcontrollers where pins can act as both Analog (ADC) and Digital (GPIO), **the silicon defaults to ANALOG INPUT at power-on reset!**  
  *Why?* To prevent floating digital Schmitt triggers from drawing shoot-through current.  
  * On `PORTA` and `PORTE` of the PIC16F877A, you **must** write to `ADCON1` (`PCFG3:PCFG0 = 0110`) to switch pins from Analog to Digital!
  * On `PORTB`, all pins are digital by default (except on parts with 10-bit analog Port B, like PIC18F). However, if you ever work with Port A, forgetting `ADCON1` will leave digital reads returning `0` forever!

---

### Step 3: Configure Direction via the TRIS Register
In Microchip silicon terminology, **TRIS** stands for *Tri-State*. It controls the high-impedance buffer of the output transistor.

* **Register:** `TRISB` (*Port B Tri-State Direction Register*)
* **Memory Location:** Bank 1, File Address `0x86`
* **Target Bit:** **Bit 0 (`TRISB0`)**
* **Bit Definitions:**
  * `1`: **INPUT** (Tri-state buffer is high-impedance / disconnected).
  * `0`: **OUTPUT** (Tri-state buffer is enabled / actively driven).

#### 💡 Why is "1 = Input" the Opposite of What Beginners Expect?
Beginners often assume `1` means "Output" (like turning on a power switch).  
In silicon hardware design:
* Writing `1` puts the output pin into a **High-Impedance ("tri-state")** state, letting external voltage float in so the CPU can listen (`1` = **I**nput).
* Writing `0` enables the low-impedance transistor driver (`0` = **O**utput).
* **The Memory Trick:**  
  * `1` looks like the letter **I** for **Input**!  
  * `0` looks like the letter **O** for **Output**!

```c
// Configure RB0 as an Output
TRISBbits.TRISB0 = 0;
```

---

### Step 4: Driving Outputs & The Read-Modify-Write (RMW) Trap
To output voltage on `RB0`, write to the `PORTB` register.

* **Register:** `PORTB` (*Port B Data Register*)
* **Memory Location:** Bank 0, File Address `0x06`
* **Target Bit:** **Bit 0 (`RB0`)**
* **Values:** `1` outputs $5\text{V}$; `0` outputs $0\text{V}$ (Ground).

```c
PORTBbits.RB0 = 1; // Turn LED ON (5V)
PORTBbits.RB0 = 0; // Turn LED OFF (0V)
```

#### ⚠️ The Infamous Read-Modify-Write (RMW) Silicon Hazard:
When you write in C:
```c
PORTBbits.RB0 = 1;
PORTBbits.RB1 = 1;
```
The XC8 compiler outputs assembly instructions:
```assembly
BSF PORTB, 0   ; Bit Set File: PORTB, bit 0
BSF PORTB, 1   ; Bit Set File: PORTB, bit 1
```
The `BSF` CPU instruction does **not** write to just one bit. It **reads the entire 8-bit physical voltage level on all 8 pins of Port B**, updates the single bit in the CPU ALU, and writes all 8 bits back to the port latch!

* **The Disaster Scenario:**  
  Suppose `RB0` is connected to a circuit with stray capacitance. You execute `BSF PORTB, 0`. Current starts charging the capacitor, but the voltage has only climbed to $1.2\text{V}$ (still below the digital High threshold of $2.0\text{V}$).  
  If the very next instruction immediately executes `BSF PORTB, 1`, the CPU reads the physical pins. It sees `RB0` is still physically below $2.0\text{V}$, **so it reads `RB0` as `0`!** It sets bit 1, writes the byte back, and **accidentally shuts off RB0!**

#### 💡 The Solution:
1. On newer microcontrollers (PIC18F, PIC16F1xxx), Microchip added the **`LATB` (Data Latch)** register. Always write to `LATB` and read from `PORTB`!
2. On the legacy PIC16F877A (which lacks `LAT`), maintain a **Shadow RAM variable** or ensure short time delays between back-to-back port writes:
   ```c
   uint8_t portb_shadow = 0;
   portb_shadow |= (1 << 0);
   PORTB = portb_shadow; // Writes full byte in one clean cycle!
   ```

---

### Step 5: Enable Weak Pull-Up Resistors (`OPTION_REG`)
If you configure a pin as an **INPUT** (e.g. for a pushbutton), you must prevent the pin from floating. Port B features internal silicon pull-up resistors ($\approx 20\text{ k}\Omega$ to $50\text{ k}\Omega$).

* **Register:** `OPTION_REG` (Bank 1, Address `0x81`)
* **Target Bit:** **Bit 7 (`nRBPU` - PORTB Pull-up Enable, Active-Low)**
* **Values:**
  * `0`: Port B weak pull-ups are **enabled** (held High to 5V when idle).
  * `1`: Port B weak pull-ups are **disabled**.

```c
OPTION_REGbits.nRBPU = 0; // Enable internal pull-up resistors on Port B
```

---

### Step 6: Prerequisite — Oscillator Configuration Words (`#pragma config`)
Before any code can execute, the physical clock oscillator, watchdog timer, and electrical voltage supervisor must be configured in silicon Flash ROM:

* `FOSC = XT`: Selects a $4\text{ MHz}$ quartz crystal.
* `WDTE = OFF`: Disables the hardware Watchdog Timer so the chip doesn't spontaneously reboot every 18 milliseconds.
* `PWRTE = ON`: Keeps the chip in reset for 72ms after power-up so the power rail stabilizes.
* `BOREN = ON`: Forces chip reset if supply voltage dips below 4.0V (brown-out).
* `LVP = OFF`: Disables Low-Voltage ICSP Programming so pin `RB3` is freed for regular GPIO use!

---

## 📊 Complete PIC16F877A RB0 Register Map Summary

| Register | Bank | Address | Target Bit | Value | Engineering Purpose |
| :--- | :---: | :---: | :---: | :---: | :--- |
| **`ADCON1`** | Bank 1 | `0x9F` | `[3:0]` | `0110` | Ensure all shared analog pins operate as Digital I/O. |
| **`OPTION_REG`**| Bank 1 | `0x81` | `[7]` (`nRBPU`) | `0` or `1`| Enable/disable internal weak pull-up resistors on Port B. |
| **`TRISB`** | Bank 1 | `0x86` | `[0]` (`TRISB0`)| `0` | Configure pin `RB0` as an **Output** driver. |
| **`PORTB`** | Bank 0 | `0x06` | `[0]` (`RB0`) | `1` | Drive physical pin `RB0` High ($5\text{V}$). |
| **`PORTB`** | Bank 0 | `0x06` | `[0]` (`RB0`) | `0` | Drive physical pin `RB0` Low ($0\text{V}$). |

---

## 💻 Full Compilable XC8 Bare-Metal Implementation

This program compiles cleanly with the Microchip XC8 compiler and runs directly on the PIC16F877A in **PICSimLab**:

```c
/**
 * @file main.c
 * @brief PIC16F877A Bare-Metal RB0 GPIO Output Driver
 */
#define _XTAL_FREQ 4000000 // 4 MHz Crystal Oscillator

#include <xc.h>

// Step 6: Hardware Configuration Bits
#pragma config FOSC  = XT      // Crystal oscillator mode
#pragma config WDTE  = OFF     // Watchdog Timer disabled
#pragma config PWRTE = ON      // Power-up Timer enabled
#pragma config BOREN = ON      // Brown-out Reset enabled
#pragma config LVP   = OFF     // Low-Voltage Programming disabled (frees RB3)
#pragma config CPD   = OFF     // Data EEPROM code protection off
#pragma config WRT   = OFF     // Flash program memory write protection off
#pragma config CP    = OFF     // Flash program memory code protection off

void main(void) {
    // Step 2: Set ADCON1 to ensure Digital I/O mode across ports
    ADCON1 = 0x06;

    // Step 3: Configure RB0 as an OUTPUT (TRIS = 0)
    // Note: The XC8 compiler automatically manages RAM bank switching!
    TRISBbits.TRISB0 = 0;

    // Step 4 & 5: Disable weak pull-ups on Port B (not needed for active output)
    OPTION_REGbits.nRBPU = 1;

    // Ensure LED starts in OFF state
    PORTBbits.RB0 = 0;

    // Step 7: The Superloop - Blink RB0
    while (1) {
        PORTBbits.RB0 = 1;      // Drive RB0 HIGH (5V)
        __delay_ms(500);        // Hardware-calibrated delay macro

        PORTBbits.RB0 = 0;      // Drive RB0 LOW (0V)
        __delay_ms(500);
    }
}
```

---

## 🔍 "Why Is Nothing Happening?" Troubleshooting Checklist

1. **Is Low-Voltage Programming (`LVP = OFF`) disabled?**  
   *Symptom:* If `LVP = ON`, pin `RB3/PGM` is dedicated to hardware programming; normal I/O on Port B can behave erratically or enter programming mode spontaneously!
2. **Did you confuse TRIS direction bits?**  
   *Symptom:* If you wrote `TRISBbits.TRISB0 = 1;` expecting an output, the pin is in high-impedance Input mode and can only source a few microamps, leaving your LED completely dark. Remember: `0` = **O**utput!
3. **Did the Watchdog Timer reset your chip?**  
   *Symptom:* The LED turns on once, but then the chip constantly resets every $\approx 18\text{ ms}$. Ensure `#pragma config WDTE = OFF`.
4. **Did you fall victim to the Read-Modify-Write (RMW) trap?**  
   *Symptom:* Toggling one pin turns off another pin on Port B. Use a shadow variable or insert a small delay between modifying different bits of the same `PORT` register.
