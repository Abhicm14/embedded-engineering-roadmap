# 🎛️ Project 4: The Volume Knob (Analog Signals & ADC)

> **Goal:** Connect a rotating knob (potentiometer), read smooth continuous voltage levels, and measure real physical millivolts on your microcontroller.  
> **Prerequisites:** Project 1 (Blink an LED) & Project 3 (Serial Telemetry).

---

### 🧠 The Real-World Intuition: Digital vs Analog

In Project 1 and 2, our pins operated strictly in **Digital Mode**:
- `HIGH` ($5\text{V}$) or `LOW` ($0\text{V}$). There was no in-between.
- That's like a basic light switch on a wall: it is either 100% ON or 100% OFF.

```
Digital Signal:     5V ──┐       ┌──┐       ┌──
                         │       │  │       │
                    0V   └───┘   └──┘   └───┘
```

However, the real physical world is **Analog**! Temperature doesn't jump instantly from 0°C to 100°C; it smoothly glides through 21.3°C, 21.4°C, 21.5°C. Sound waves, battery drain curves, and volume knobs are all smooth, continuous analog signals.

```
Analog Signal:      5V        ╭─────╮
                            ╭─╯     ╰─╮     ╭─╮
                            │         ╰─────╯ │
                    0V ─────╯                 ╰──
```

### 📏 What is an ADC? (The Voltage Ruler)
An **Analog-to-Digital Converter (ADC)** is a tiny digital ruler built into your chip.  
If your microcontroller runs at $5.0\text{V}$, and has a **10-bit ADC**:
- A 10-bit ruler has $2^{10} = 1024$ microscopic tick marks (from `0` up to `1023`).

```
  Real Voltage:    0.0 Volts ────────── 2.5 Volts ────────── 5.0 Volts
                       │                     │                    │
                       ▼                     ▼                    ▼
  ADC Reading:      0 Counts            512 Counts           1023 Counts
```

To convert the raw digital number back into real Volts in your code:
$$\text{Voltage} = \frac{\text{ADC Count} \times 5.0\text{ Volts}}{1023}$$

---

## 🌐 Track A: Run in Browser with Wokwi (Zero Install)

### Step 1: Open Wokwi
Open [https://wokwi.com](https://wokwi.com) $\to$ Select **Arduino Uno**.

### Step 2: Add a Potentiometer to the Board
1. Click the **"+" (Add part)** button $\to$ Select **Potentiometer**.
2. Wire the potentiometer's 3 pins:
   - **GND terminal (left):** Drag wire to Arduino **GND**.
   - **Signal/Wiper terminal (middle):** Drag wire to Arduino **A0** (Analog Input 0).
   - **VCC terminal (right):** Drag wire to Arduino **5V**.

### Step 3: Paste the Code into `sketch.ino`

```cpp
// Project 4: Reading an Analog Potentiometer Knob
const int POT_PIN = A0;  // Analog input pin connected to wiper
const int LED_PIN = 13;  // Built-in indicator LED

void setup() {
  Serial.begin(9600);
  pinMode(LED_PIN, OUTPUT);
  Serial.println("🎛️ Analog-to-Digital Converter (ADC) Demo Ready!");
}

void loop() {
  // 1. Read the raw 10-bit ADC count (gives a number between 0 and 1023)
  int rawValue = analogRead(POT_PIN);

  // 2. Calculate the real voltage (0.0V to 5.0V)
  float voltage = (rawValue * 5.0) / 1023.0;

  // 3. Print the results to the Serial Monitor
  Serial.print("Raw ADC Count: ");
  Serial.print(rawValue);
  Serial.print(" | Measured Voltage: ");
  Serial.print(voltage, 2); // Print with 2 decimal places
  Serial.println(" Volts");

  // 4. Fun feature: Make the LED blink faster as you turn the knob up!
  digitalWrite(LED_PIN, HIGH);
  delay(rawValue / 4 + 20); // Faster blink when knob is near 0, slower when high
  digitalWrite(LED_PIN, LOW);
  delay(rawValue / 4 + 20);
}
```

### Step 4: Run and Test
1. Click the **Serial Monitor** tab at the bottom, then click **Play (▶)**.
2. Click and drag the white knob on the potentiometer left and right.
3. Watch the voltage smoothly update from `0.00 V` up to `5.00 V` in real time, while the LED changes its blinking speed!

---

## 🖥️ Track B: Run in PICSimLab (Microchip PIC16F877A)

On the PIC16F877A, Potentiometer 1 on the PICGenios board is wired to **Pin RA0 / AN0**.

### Paste the Code into `main.c`:

```c
// PIC16F877A - Project 4: Hardware ADC Measurement
#define _XTAL_FREQ 4000000

#include <xc.h>
#include <stdio.h>

#pragma config FOSC = XT, WDTE = OFF, PWRTE = ON, BOREN = ON, LVP = OFF

void ADC_Init(void) {
    TRISAbits.TRISA0 = 1;     // Configure RA0 as an INPUT
    ADCON1 = 0b10001110;      // Right-justified, AN0 is Analog, VDD/VSS references
    ADCON0 = 0b01000001;      // Fosc/8 clock, Channel 0 (AN0) selected, ADC module ON
}

unsigned int ADC_Read(void) {
    __delay_us(30);           // Wait for internal sample-and-hold capacitor to charge
    ADCON0bits.GO_nDONE = 1;  // Start conversion!
    while (ADCON0bits.GO_nDONE); // Wait until hardware finishes conversion
    
    // Combine 10-bit result from high and low byte registers
    return ((unsigned int)ADRESH << 8) | ADRESL;
}

void UART_Init(void) {
    TRISCbits.TRISC6 = 0;
    SPBRG = 25;
    TXSTAbits.BRGH = 1;
    TXSTAbits.TXEN = 1;
    RCSTAbits.SPEN = 1;
}

void UART_Write_String(const char *str) {
    while (*str) {
        while (!TXSTAbits.TRMT);
        TXREG = *str++;
    }
}

void main(void) {
    ADC_Init();
    UART_Init();
    UART_Write_String("=== PIC16F877A ADC Voltmeter Initialized ===\r\n");

    char buffer[50];
    while (1) {
        unsigned int raw = ADC_Read();
        // Convert to millivolts without using slow floating-point math:
        // (raw * 5000 mV) / 1023
        unsigned long millivolts = ((unsigned long)raw * 5000) / 1023;

        sprintf(buffer, "ADC Count: %4u | Voltage: %lu.%03lu V\r\n", 
                raw, millivolts / 1000, millivolts % 1000);
        UART_Write_String(buffer);

        __delay_ms(300);
    }
}
```

### Run in PICSimLab:
1. Load the `.hex` file.
2. In the top menu, open **Modules $\to$ Serial Terminal** (9600 baud).
3. Find the blue potentiometer dial marked **POT1** on the PICGenios virtual board. Rotate it with your mouse and observe the voltage readings update on the terminal!

---

## 🚀 Beginner Experiments & Challenges

1. **The Over-Voltage Alarm:** Modify the code so that if the measured voltage exceeds $3.5\text{V}$, a warning message prints: `🚨 WARNING: OVER-VOLTAGE DETECTED!` and the LED turns solid red.
2. **A Digital Dimmer (PWM):** In Wokwi, instead of changing the delay, try using `analogWrite(LED_PIN, rawValue / 4);` on Pin 9. This directly dims and brightens the LED as you turn the knob!

---

➡️ **Next Project:** [**Project 5: Interactive Terminal Controller (Button + LED + Serial Combined)**](05-button-led-serial.md)
