# 🟢 Beginner Steps: PIC Foundations

> First three steps establish the MPLAB X workflow, basic I/O, and timing fundamentals using PIC16F877A in PICSimLab.

---

## 🧭 Foundational Prerequisites

Before writing any PIC16F firmware, ensure you have reviewed:
1. [**Foundational Prerequisites (`PREREQUISITES.md`)**](../PREREQUISITES.md): Number systems, bitwise manipulation, pull-up resistors, and MMIO concepts.
2. [**Installation Guide (`installation.md`)**](installation.md): MPLAB X IDE, XC8 Compiler, and PICSimLab simulator setup.
3. [**PIC XC8 Cheatsheet**](../cheatsheets/pic-xc8-cheatsheet.md): Register syntax, configuration pragmas, and timing formulas.

---

## Step 1: MPLAB X Project & LED Blink

### Goal
Create your first MPLAB X project, configure the hardware fuses for the PIC16F877A, and blink an LED connected to RB0.

### Concept Explanation

#### Microcontroller Hardware Configuration
We use the **PIC16F877A** as our learning platform because:
- Well-documented with a clean 40-pin DIP datasheet.
- Rich peripheral set (ADC, UART, SPI, I2C, CCP, comparators).
- Inexpensive and simulated in real-time by PICSimLab.

#### Configuration Bits (`#pragma config`)
Before `main()`, we must configure the silicon hardware fuses:
- **Oscillator (`FOSC = HS`)**: High-Speed crystal oscillator (4 MHz to 20 MHz).
- **Watchdog Timer (`WDTE = OFF`)**: Disabled during learning to prevent unexpected resets.
- **Power-up Timer (`PWRTE = ON`)**: Holds chip in reset briefly after power-up for voltage stabilization.
- **Brown-out Reset (`BOREN = ON`)**: Resets MCU if supply drops below safe threshold (4.0V).
- **Low-voltage Programming (`LVP = OFF`)**: Dedicated high-voltage programming on MCLR, releasing pin RB3 for standard I/O.

#### Delay Macro Mechanics
- `__delay_ms()` and `__delay_us()` count CPU instruction cycles.
- On mid-range PICs, 1 instruction cycle ($T_{CY}$) = 4 clock cycles ($4 \times T_{OSC}$).
- At 4 MHz, 1 instruction cycle = $1\,\mu\text{s}$.
- The compiler requires `#define _XTAL_FREQ 4000000` to calculate loop cycles accurately.

---

### 🔨 How to Write This Code Step-by-Step

Do not copy-paste. Construct the program in your editor following these 5 steps:

1. **Step 1: Include the Microchip Hardware Header**
   Write `#include <xc.h>`. The compiler automatically includes `pic16f877a.h` based on your project device settings.
2. **Step 2: Configure Hardware Fuses**
   Specify `#pragma config FOSC = HS, WDTE = OFF, PWRTE = ON, BOREN = ON, LVP = OFF, CPD = OFF, CP = OFF`.
3. **Step 3: Define Master Clock Frequency**
   Add `#define _XTAL_FREQ 4000000` so that `__delay_ms()` can calculate exact cycle counts.
4. **Step 4: Configure Data Direction in `main()`**
   Set `TRISBbits.TRISB0 = 0;` (0 = Output). Initialize pin state with `PORTBbits.RB0 = 0;`.
5. **Step 5: Implement the Superloop**
   Inside `while(1)`, write `PORTBbits.RB0 = 1;`, call `__delay_ms(500);`, write `PORTBbits.RB0 = 0;`, and call `__delay_ms(500);`.

---

### Annotated Reference Implementation
```c
#include <xc.h>

// 1. Hardware Configuration Bits
#pragma config FOSC = HS        // High-Speed Crystal (4 MHz - 20 MHz)
#pragma config WDTE = OFF       // Watchdog Timer disabled
#pragma config PWRTE = ON       // Power-up Timer enabled
#pragma config BOREN = ON       // Brown-out Reset enabled
#pragma config LVP = OFF        // Low-Voltage Programming disabled
#pragma config CPD = OFF        // Data EEPROM code protection off
#pragma config CP = OFF         // Flash program memory code protection off

// 2. Oscillator Frequency for Compiler Delays
#define _XTAL_FREQ 4000000      // 4 MHz

void main(void) {
    // 3. Configure Pin Direction (TRIS: 0 = Output, 1 = Input)
    TRISBbits.TRISB0 = 0;       // Configure RB0 as output
    PORTBbits.RB0 = 0;          // Start with LED off
    
    // 4. Infinite Superloop
    while(1) {
        PORTBbits.RB0 = 1;      // LED On (High voltage)
        __delay_ms(500);        // Block CPU for 500ms
        PORTBbits.RB0 = 0;      // LED Off (Ground)
        __delay_ms(500);        // Block CPU for 500ms
    }
}
```

### PICSimLab Procedure
1. **Create Project** in MPLAB X:
   - File → New Project → Microchip Embedded → Standalone Project
   - Device: `PIC16F877A`, Compiler: `XC8`
2. **Build Project**: Click Hammer icon (or press F11). Verify `BUILD SUCCESSFUL`.
3. **Open PICSimLab**: Select **Board 1 (PICGenios)** with device **PIC16F877A**.
4. **Load Hex**: File → Load Hex → select your project's `.hex` file.
5. **Observe**: LED on RB0 (Pin 33) blinks at 1 Hz.

---

## Step 2: GPIO & Button Control

### Goal
Read a physical pushbutton switch on RB1, eliminate mechanical switch contact bounce in firmware, and control the LED on RB0.

### Concept Explanation

#### The TRIS and PORT Registers
- **TRISB (Tri-State Register)**: Sets electrical direction. `0` = Output, `1` = Input.
- **PORTB**: Reads voltage state on the pin (if input) or drives output latch (if output).

#### Internal Weak Pull-Ups
When a pushbutton is open, an unconnected input pin floats and oscillates.
- Mid-range PICs include internal weak pull-ups on PORTB.
- Enabled by clearing the active-low bit `nRBPU` in `OPTION_REG`:
  ```c
  OPTION_REGbits.nRBPU = 0; // 0 = Internal pull-ups on PORTB enabled
  ```

#### Mechanical Contact Bounce
When a mechanical switch closes, its metal leaves bounce for 5 ms to 20 ms, generating dozens of rapid voltage spikes. Firmware must filter this noise by requiring a stable voltage reading for at least 20 ms.

---

### 🔨 How to Write This Code Step-by-Step

1. **Step 1: Set Direction Registers**
   Set `TRISBbits.TRISB0 = 0;` (LED output) and `TRISBbits.TRISB1 = 1;` (Button input).
2. **Step 2: Enable Internal Pull-Ups**
   Write `OPTION_REGbits.nRBPU = 0;` to pull RB1 HIGH internally without needing an external resistor.
3. **Step 3: Define Debounce State Variables**
   Declare `debounced_state = 1;` (unpressed), `last_sample = 1;`, and `stable_count = 0;`.
4. **Step 4: Implement Non-Blocking Sampling Loop**
   In `while(1)`, sample `PORTBbits.RB1` every 1 ms using `__delay_ms(1);`.
5. **Step 5: Validate Stability**
   If `current_sample == last_sample`, increment `stable_count`. When `stable_count == 20`, update `debounced_state` and drive `PORTBbits.RB0`. If a state toggle occurs, reset `stable_count = 0`.

---

### Annotated Reference Implementation
```c
#include <xc.h>

#pragma config FOSC = HS, WDTE = OFF, PWRTE = ON, BOREN = ON
#pragma config LVP = OFF, CPD = OFF, CP = OFF
#define _XTAL_FREQ 4000000

#define DEBOUNCE_THRESHOLD_MS 20

void main(void) {
    // 1. Direction configuration
    TRISBbits.TRISB0 = 0;       // Output (LED)
    TRISBbits.TRISB1 = 1;       // Input (Button)
    PORTBbits.RB0 = 0;

    // 2. Enable internal weak pull-ups on PORTB
    OPTION_REGbits.nRBPU = 0;

    unsigned char debounced_state = 1;
    unsigned char last_sample = 1;
    unsigned int  stable_count = 0;

    while(1) {
        unsigned char current_sample = PORTBbits.RB1;

        if (current_sample == last_sample) {
            if (stable_count < DEBOUNCE_THRESHOLD_MS) {
                stable_count++;
                if (stable_count == DEBOUNCE_THRESHOLD_MS) {
                    debounced_state = current_sample;
                    // Button pressed pulls pin to GND (active LOW)
                    if (debounced_state == 0) {
                        PORTBbits.RB0 = 1; // Turn LED ON
                    } else {
                        PORTBbits.RB0 = 0; // Turn LED OFF
                    }
                }
            }
        } else {
            stable_count = 0;
            last_sample = current_sample;
        }

        __delay_ms(1);          // 1 ms debounce tick
    }
}
```

---

## Step 3: Timers & Interrupts

### Goal
Replace blocking software delays with a deterministic Timer0 hardware interrupt to achieve non-blocking execution.

### Concept Explanation

#### Timer0 Architecture (PIC16F877A)
- 8-bit hardware up-counter (`TMR0`, 0 to 255).
- Clocked from the internal instruction clock $F_{CY} = F_{OSC}/4 = 1\text{ MHz}$ at 4 MHz ($1\,\mu\text{s}$ per instruction).
- Configured via `OPTION_REG`:
  - `T0CS = 0`: Selects internal clock.
  - `PSA = 0`: Assigns prescaler to Timer0.
  - `PS2:PS0 = 111`: 1:256 prescaler division.
  - Each timer increment = $1\,\mu\text{s} \times 256 = 256\,\mu\text{s}$.
  - Overflow rollover occurs every $256 \times 256\,\mu\text{s} = 65.536\text{ ms}$.

#### Interrupt Mechanics
1. When `TMR0` rolls over from `255` to `0`, hardware sets `INTCONbits.T0IF = 1`.
2. If `INTCONbits.T0IE = 1` and `INTCONbits.GIE = 1`, the CPU suspends the superloop and vectors to `0x0004`.
3. The Interrupt Service Routine (ISR) handles the tick, **manually clears `T0IF = 0`**, and returns.

---

### 🔨 How to Write This Code Step-by-Step

1. **Step 1: Write the Timer0 Hardware Configuration**
   In `OPTION_REG`, write `T0CS = 0`, `PSA = 0`, and `PS2:PS0 = 111` (1:256 prescaler).
2. **Step 2: Enable Interrupt Flags in `INTCON`**
   Clear `INTCONbits.T0IF = 0;`, set `INTCONbits.T0IE = 1;` (Timer0 interrupt enable), and set `INTCONbits.GIE = 1;` (Global interrupt enable).
3. **Step 3: Define Volatile Shared State**
   Declare `volatile unsigned int overflow_counter = 0;` outside all functions.
4. **Step 4: Implement the Interrupt Service Routine**
   Use `void __interrupt() isr(void)`. Check `if (INTCONbits.T0IF && INTCONbits.T0IE)`. **Immediately clear `INTCONbits.T0IF = 0;`**.
5. **Step 5: Accumulate Ticks for Timing**
   Increment `overflow_counter`. When it reaches 8 ($8 \times 65.536\text{ ms} \approx 524\text{ ms}$), toggle `PORTBbits.RB0 ^= 1;` and reset counter.

---

### Annotated Reference Implementation
```c
#include <xc.h>

#pragma config FOSC = HS, WDTE = OFF, PWRTE = ON, BOREN = ON
#pragma config LVP = OFF, CPD = OFF, CP = OFF
#define _XTAL_FREQ 4000000

volatile unsigned int overflow_counter = 0;

void __interrupt() isr(void) {
    // 1. Verify Timer0 interrupt source
    if (INTCONbits.T0IF && INTCONbits.T0IE) {
        INTCONbits.T0IF = 0;    // Crucial: clear flag in software!

        overflow_counter++;
        // 8 overflows * 65.536 ms ~= 524 ms (~0.5 second)
        if (overflow_counter >= 8) {
            overflow_counter = 0;
            PORTBbits.RB0 ^= 1; // Toggle LED
        }
    }
}

void main(void) {
    // 2. Configure Output Pin
    TRISBbits.TRISB0 = 0;
    PORTBbits.RB0 = 0;

    // 3. Configure Timer0 in OPTION_REG: Prescaler 1:256
    OPTION_REGbits.T0CS = 0;    // Internal clock (Fosc/4)
    OPTION_REGbits.PSA = 0;     // Prescaler assigned to Timer0
    OPTION_REGbits.PS2 = 1;
    OPTION_REGbits.PS1 = 1;
    OPTION_REGbits.PS0 = 1;
    TMR0 = 0;

    // 4. Enable Interrupts in INTCON
    INTCONbits.T0IF = 0;
    INTCONbits.T0IE = 1;        // Enable Timer0 interrupt
    INTCONbits.GIE = 1;         // Enable global interrupts

    // 5. Non-blocking Superloop
    while(1) {
        // CPU can do other work here; LED is toggled in the hardware ISR!
    }
}
```

---

## 🏁 Beginner Verification Checklist

- [ ] Configuration pragmas set for high-speed crystal (`FOSC = HS`).
- [ ] LED blinking validated in PICSimLab without compiler errors.
- [ ] Pushbutton debouncer verified: single press produces exactly one toggle without bounce chatter.
- [ ] Timer0 hardware interrupt validated: LED toggles deterministically without blocking `main()`.
- [ ] Proceed to [**Intermediate Steps (Steps 4–8)**](intermediate.md).
