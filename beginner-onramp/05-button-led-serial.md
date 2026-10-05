# 🎛️ Project 5: Interactive Controller (Button + LED + Serial Combined)

> **Goal:** Build an interactive tally counter system: detect button clicks, blink an indicator confirmation light, send telemetry reports over serial, and accept keyboard commands from your computer to reset the count.  
> **Prerequisites:** Projects 1, 2, and 3.

---

### 🧠 The Real-World Intuition: Inputs, Outputs & Telemetry Working Together

Every commercial embedded system—from a smart thermostat to a rocket booster controller—combines three core operations:

```
    ┌────────────────┐         ┌─────────────────────────┐         ┌────────────────┐
    │     INPUT      │ ──────► │   PROCESSING & LOGIC    │ ──────► │     OUTPUT     │
    │ (Button/Sensor)│         │ (Microcontroller Brain) │         │ (LED/Actuator) │
    └────────────────┘         └────────────┬────────────┘         └────────────────┘
                                            │
                                            ▼
                               ┌─────────────────────────┐
                               │   TELEMETRY / COMS      │
                               │ (Serial Data to PC)     │
                               └─────────────────────────┘
```

In this project, we also solve a classic beginner puzzle: **Edge Detection vs Level Detection**.

#### Why Does Holding a Button Down Cause Chaos?
If you write:
```cpp
if (buttonState == LOW) {
    count++; // Uh oh!
}
```
Because the CPU runs millions of loops per second, if you hold your finger down for just half a second, the CPU will loop 15,000 times and add 15,000 to your counter!

Instead, we want **Edge Detection**:
- We only count when the button **transitions** from released to pressed (from `HIGH` $\to$ `LOW`).
- Even if you hold your finger down for 10 minutes, it counts exactly **once**!

---

## 🌐 Track A: Run in Browser with Wokwi (Zero Install)

### Step 1: Open Wokwi
Open [https://wokwi.com](https://wokwi.com) $\to$ Select **Arduino Uno**.

### Step 2: Wire the Pushbutton
- Add a **Pushbutton**.
- Wire one side to **Pin 2**, and the other side to **GND**.

### Step 3: Paste the Code into `sketch.ino`

```cpp
// Project 5: Interactive Tally Counter with Serial Telemetry
const int BUTTON_PIN = 2;
const int LED_PIN    = 13;

int pressCount = 0;
int lastButtonState = HIGH; // Tracks what the button was doing on the previous loop pass

void setup() {
  Serial.begin(9600);
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  Serial.println("==============================================");
  Serial.println("🎮 Interactive Production Counter System Ready");
  Serial.println("👉 Press button to count.");
  Serial.println("👉 Type 'r' in the Serial Monitor and press Enter to reset!");
  Serial.println("==============================================");
}

void loop() {
  // 1. Read the current button state
  int currentButtonState = digitalRead(BUTTON_PIN);

  // 2. EDGE DETECTION: Check if button JUST went from HIGH (released) to LOW (pressed)
  if (currentButtonState == LOW && lastButtonState == HIGH) {
    // Wait 20 milliseconds to debounce mechanical leaf vibrations
    delay(20);

    // Increment tally
    pressCount++;

    // Visual feedback: blink the confirmation LED
    digitalWrite(LED_PIN, HIGH);
    
    // Telemetry feedback: log event to PC screen
    Serial.print("🔔 [EVENT] Pushbutton Pressed! Total Tally = ");
    Serial.println(pressCount);

    delay(80); // Keep LED on briefly for human eyes to notice
    digitalWrite(LED_PIN, LOW);
  }

  // 3. Update last known state for the next cycle
  lastButtonState = currentButtonState;

  // 4. REMOTE COMMAND PARSING: Listen for incoming commands from PC
  if (Serial.available() > 0) {
    char cmd = Serial.read();
    if (cmd == 'r' || cmd == 'R') {
      pressCount = 0;
      Serial.println("🔄 [COMMAND] Counter successfully reset to ZERO.");
    }
  }
}
```

### Step 4: Run and Test
1. Click the **Serial Monitor** tab.
2. Click **Play (▶)**.
3. Click the button several times: notice how the LED flashes once per click and the terminal neatly increments.
4. Type `r` into the text box at the top of the Serial Monitor and press **Enter**. Notice how the counter instantly resets!

---

## 🖥️ Track B: Run in PICSimLab (Microchip PIC16F877A)

### Paste the Code into `main.c`:

```c
// PIC16F877A - Project 5: Button Tally Counter with UART Control
#define _XTAL_FREQ 4000000

#include <xc.h>
#include <stdio.h>

#pragma config FOSC = XT, WDTE = OFF, PWRTE = ON, BOREN = ON, LVP = OFF

void UART_Init(void) {
    TRISCbits.TRISC6 = 0; // TX output
    TRISCbits.TRISC7 = 1; // RX input
    SPBRG = 25;
    TXSTAbits.BRGH = 1;
    TXSTAbits.TXEN = 1;
    RCSTAbits.CREN = 1;   // Enable continuous receive
    RCSTAbits.SPEN = 1;   // Enable serial port
}

void UART_Write_String(const char *str) {
    while (*str) {
        while (!TXSTAbits.TRMT);
        TXREG = *str++;
    }
}

void main(void) {
    TRISBbits.TRISB0 = 1; // Pushbutton SW1 input
    TRISBbits.TRISB1 = 0; // LED D1 output
    OPTION_REGbits.nRBPU = 0; // Enable pull-ups

    UART_Init();
    UART_Write_String("\r\n=== PIC16F877A Interactive Counter Ready ===\r\n");

    int count = 0;
    int lastState = 1;
    char buffer[50];

    while (1) {
        int currentState = PORTBbits.RB0;

        // Falling edge: transitioned from 1 (unpressed) to 0 (pressed)
        if (currentState == 0 && lastState == 1) {
            __delay_ms(20); // Debounce
            count++;
            PORTBbits.RB1 = 1; // LED on

            sprintf(buffer, "[EVENT] Click detected! Count = %d\r\n", count);
            UART_Write_String(buffer);

            __delay_ms(80);
            PORTBbits.RB1 = 0; // LED off
        }
        lastState = currentState;

        // Check if PC sent a character over UART
        if (PIR1bits.RCIF) {
            char rx = RCREG;
            if (rx == 'r' || rx == 'R') {
                count = 0;
                UART_Write_String("[RESET] Count cleared back to 0!\r\n");
            }
        }
    }
}
```

---

## 🚀 Beginner Experiments & Challenges

1. **High-Score Buzzer:** Add a threshold rule: when `pressCount` reaches 10, print `🏆 WINNER: 10 CLICKS REACHED!` and rapidly flash the LED 5 times!
2. **Double Click Detector:** Can you detect if the user clicked the button twice in less than 300 milliseconds?

---

➡️ **Next Project:** [**Project 6: Traffic Light State Machine (Ditching `delay()` Forever!)**](06-blinking-state-machine.md)
